# 条款14：在资源管理类中小心copying行为

​		Think carefully about copying behavior in resource-managing classes.

​	条款13导入这样的概念：“资源取得时机便是初始化时机”（Resource Acquisition Is Initialization; RAII），并以次作为“资源管理类”的脊柱，也描述了 auto_ptr 和 `tr1::shared_ptr`如何将这个表现在 heap-based 资源上。然而并非所有资源都是heap-based，对那种资源而言，像`auto_ptr`和`tr1::shared_ptr`这样的智能指针往往不适合作为资源管理者（resource handlers）。既然如此，有可能偶而你会发现，你需要建立自己的资源管理类。

​	例如，假设我们试验 C API 函数处理类型为 Mutex 的互斥器对象（mutex objects），共有 `lock` 和`unlock`两函数可用：

```cpp
void lock(Mutex* pm); 			// 锁定 pm 所指的互斥器
void unlock(Mutex* pm);			// 将互斥器解除锁定
```

​	为确保绝对不会忘记将一个锁住的`Mutex`解锁，你可能会希望建立一个 class 来管理机锁。这样的class的基本结构由 RAII 守则支配，也就是“资源在构造期间获得，在析构间释放”：

````cpp
class Lock
{
public:
	explicit Lock(Mutex* pm) : mutexPtr(pm)
	{	
        lock(mutexPtr);  // 获取资源
    }
	
	~Lock() 
    {	
        unlock(mutexPtr);	// 释放资源
    }
	
private:
	Mutex *mutexPtr;
};
````

​	客户对 Lock 的用法符合 RAII 方式：

````cpp
Mutex m; 		// 定义你需要的互斥器
...
{				// 建立一个区块来定义critical section
Lock m1(&m);    // 锁定互斥器
    ...			// 执行 critial section 内的操作
}				// 在区块的最末尾，自动解锁互斥器
````

​	这很好，但如果 Lock 对象被复制，会发生什么事？

````cpp
Lock ml1(&m);	// 锁定m
Lock ml2(ml1);	// 将ml1复制到ml2身上，发生什么？
````

​	这是某个比较一般化问题的特定例子。那个一般化问题是每一位 RAII class 作者都一定需要面对的：“当一个 RAII 对象被复制，会发生什么？”大多数时候你会选择两种可能：

- **禁止复制**：许多时候允许 RAII 对象被复制时不合理的。对一个像 Lock 这样的class 是这可能的，因为很少能够合理拥有“同步化基础器物”的复本。如果复制动作对 RALL class 并不合理，你便应该禁止。条款6告诉你这么做，将copying 设置为 private：

    ````cpp
    class Lock: private Uncopying{
    public:
    	....
    };
    ````

- **对底层资源祭出“引用计数法”**：有时候我们希望保有资源，直到它的最后一个引用者被销毁。这种情况下复制 RAII 对象时，应该将资源的“被引用数”递增。`trl::shard_ptr`就是如此。

​	通常只要含有一个`trl::shared_ptr`成员变量，RAII classes 便可以实现出引用计数行为。如果前述的 Lock 打算使用引用计数，它可以改变mutexPtr的类型，将它从` Mutex* `改为 `trl::shared_ptr<Mutex>`。但是很不幸`trl::shared_ptr`的缺省行为是“当引用计数次数为0时删除其所指物”，那不是我们要的行为，当我们使用一个Mutex时，我们希望的动作是解锁而非删除。

​	幸运的是`trl::shared_ptr`允许指定所谓的删除器，那是一个函数或者函数对象，当引用计数为0时调用（此机能不存在于 auto_ptr）。删除器对`trl::shard_ptr`构造函数而言是可有可无的第二个参数，所以可以这样：

````cpp
class Lock
{	
public:
	explicit Lock(Mutex* pm): mutexPtr(pm, unlock) // 以某个Mutex对象初始化shard_ptr
	{											   // 并且以unlock函数为其函数器
		lock(mutexPtr.get());					   // 条款15提到“get”
	}
private:
	std::trl::shared_ptr<Mutex> mutexPtr;		 	// 使用 shared_ptr
};
````

​	这个类不需要写析构，因为类对象被销毁时，自动调用成员的析构函数，也就是 mutexPtr 的析构，而其析构函数会在引用计数为0时自动调用删除器。

- **复制底部资源**：复制带有指针的对象时，复制其所包含的资源；就像string复制时，不能只复制其字符串地址，应该开辟一个新空间，只是新空间跟原来的资源是一样的，这就是深拷贝。
- **转移底部资源的拥有权**：复制时，将资源的拥有权力从被复制物移动到目标上，原来的对象失去资源。



## 请记住：

- 复制 RAII 对象必须一并复制其管理的资源，所以资源的copying行为决定 RAII 对象的copying 行为；
- 普遍常见的 RAII class copying 行为是：抑制 copying 、使用引用计数、深拷贝、转移资源；