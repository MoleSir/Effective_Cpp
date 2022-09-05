# 条件 52：写了 placement new 也要写 placement delete

​		Write placement delete if you write placement new.

执行 

````c++
Widget* pw = new Widget();
````

首先使用 `operator new` 申请内存空间，之后调用 `Widget` 的构造函数。如果申请空间成功，但是构造函数发生异常，那么申请来的空间就必须释放，否则导致内存泄漏；

但 `pw` 此时还没有获得指针，所以客户没有办法归还内存。那么这个任务就需要由 C++ 运行系统完成；

C++ 会在这种情况下，主动调用申请空间用的 `operator new` 对应的 `operator delete`；如果没有改变重载 `Widget` 的 `operator new` 一切正常，C++ 知道正常的 `operator delete` 是哪个；

> 对正常的 `operator new`，其函数签名为：
>
> ````c++
> void* operator new (std::size_t size) throw(std::bad_alloc);
> ````
>
> 对正常的 `operator delete`，其函数签名为：
>
> ````c++
> // global 作用域正常的签名式
> void operator delete (void* rawMemory) throw();
> // class 内部典型签名式
> void operator delete (void* rawMemory, std::size_t size);
> ````
>
> 而如果 `operator new` 接受的参数除了 `size` 之外还有其他，称为 `placement new`；大多情况下 `placement new` 指的是：
>
> ````c++
> void* operator new (std::size_t size, void* pMemory) throw();
> ````
>
> 这个版本的重载；
>
> 而与 `placement new` 对应的 `placement delete`，其实就是参数列表相同的一个重载版本；

但是若重载了类专属的 `operator new`，并且还接受一个 `ostream` 记录分配信息，但没有写出其对应的  `operator delete`：

````c++
class Widget{
public:
	...
    // 非正常形式的 class 的 operator new
    static void* operator new (std::size_t size, std::ostream& logStream)
        throw(std::bad_alloc);
    // 正常形式的 class 的 operator delete
   	static void operator delete (void* pMemory, std::size_t size);
};
````

并且在 `new` 时使用 `placement new`：

````c++
// 调用 operator new 并且传递 cerr 为其 ostream 实参
Widget* pw = new (std::cerr) Widget();
````

还是那个情况，如果申请内存成功，构造函数异常，C++ 不知道应该调用哪个 `placement delete`；

所以，既然提供了 `Widget` 的 `placement new`，也需要提供对应的 `placement delete`：

````c++
class Widget{
public:
    ...
    static void* operator new (std::size_t szie,std::ostream& logStream) 
        throw(std::bad_alloc);
    static void operator delete (void* pMemory) throw();
    // 对应的 placement delete
    static void operator delete (void* pMemory, std::ostream& logStream) throw();
};
````

这样对应的 `placement delete` 会自动调用；

而 `placement delete` 只有在 “伴随 `placement new` 调用而触发的构造函数” 出现异常时才会被调用；对一个指针指执行 `delete`： 

`````c++
delete pw;
// 不能调用
delete (std::cerr) pw;
`````

这样调用的还是正常的 `operator delete`；

所以还是不能忘记提供正常的 `operator delete`；



由于成员函数的名称会掩盖其外围作用域的相同名称；如果一个基类，声明了唯一的 `placement new`，那么这个基类正常形式的  `new` 无法被调用；因为这个 `operator new` 掩盖了全局的 `operator new`；

```c++
class Base{
public:
    static void* operator new(std::size_t size, std::ostream& logStream) 
        throw(std::bad_alloc)
    { ... }
    ...
};
Base* pb = new Base;		// 错误，正常的 operator new 被掩盖
Base* pb = new (std::cerr) Base; // 正确
```

而如果这个基本继续被继承，子类中的 `operator new` 也会掩盖基类的；

````c++
class Derived: public Base {
public:
	...
    // 子类重新声明了正常形式的 new
    static void* operator new(std::size_t size) throw(std::bad_alloc){...}
    ...
};
Derived* pd = new (std::clog) Derived;		// 错误，基类的new掩盖
Derived* pd = new Derived;					// 正确
````

在缺省情况下，C++ 在 global 作用域内提供了三个 `operator new`：

````c++
void* operator new (std::size_t) throw(std::bad_alloc);
void* operator new (std::size_t, void*) throw();
void* operator new (std::size_t, const std::nothrow_t&) throw();
````



## 请记住

- 当你写一个 `placement new`，请保证也也出了对应的 `placement operator delete`。如果没有这样做，你的程序可能会发生隐微而时断时续的内存泄露；
- 当你声明 `placement new` 和 `placement delete` ，请确定不要无意识得掩盖了它们的正常版本； 