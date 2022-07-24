# 条款42：了解typename的双重意义

​		Understand the two meanings of typename

​	在 template 声明式中，使用 class 与 typename 意义完全相同；

````c++
template<class _Ty> class Widget;
template<typename _Ty> calss Widget;
````

​	但有时必须使用 typename；	

​	首先来看看在 template 可能涉及到的**两种名称**，举个小例子：

````c++
template<typename C>
void Print2nd(const C& containter)
{
    if (containter.size() >= 2)
    {
        // 所有的 std 容器都有这样一个迭代器类型：
        C::const_iterator iter(container.begin());
        ++iter;
        int value = *iter;
        std::cout << value;
	}
}
````

​	这个程序希望将打印出传入容器的出第二个值，我们来分析一下：

​	所以函数内部 `iter` 这个变量的类型是必须取决于参数 C 到底是什么，而模板内部出现的名称如果依赖于某个模板参数，称之为 从属名称；

​	并且如果从属名称在 class 内程序嵌套，我们称其为 嵌套从属名称，而 `C::const_iterator `就是这样一个名称；

​	而对 `value` 呢，它就是 int 类型，成为 非从属名称;

​	好了，这段代码看似没什么问题，但是其是不可以通过编译的，作为程序员，我们当然很自然得从函数名称、传入的类型名称等多个细节看出这个函数希望传入一个容器。但请在编译器的角度想想，它只知道 C 是一个类型名称，而  `C::const_iterator `到底是什么？它不知道；这导致了解析困难！！

​	也许这样不是很明显，那么看这样一个例子：

````c++
template<typename C>
void Print2nd(const C& containter)
{
    C::const_iterator* x;
    ...
}
````

​	正如我们前面所说，编译器看不懂我们给类型取的是什么名字，`C::const_iterator` 是类型吗，它不知道；如果是类型，那么这句话应该定义了一个指针。但如果`C::const_iterator` 是一个静态成员呢？那么这句话是不是成为两个变量相乘？编译器无法得知，所以它报错；

​	所以我们需要做点什么，让编译器知道 `C::const_iterator` 是一个类型，方式就是 使用 typename：

````c++
template<typename C>
void Print2nd(const C& containter)
{
    if (containter.size() >= 2)
    {
        // 明确告诉编译器，这是一个类型
        typename C::const_iterator iter(container.begin());
		...
	}
}
````

​	一般性规则很简单，任何时候你想要在 template 中指涉一个嵌套从属类型名称，就必须在紧跟它的前方防止一个 typename；

​	

​	但也不需要什么都加 typename：

```c++
template<typename C>						// 使用 class 和 typename 都可以			
void f(const C& container,					// 不能用 typename
      typename C::iterator iter);			// 必须用  typename 
```

​	编译器已经知道 C 是一个类型，那么就在定义 C 类型对象时，就不能加 `typename`；

---



​	“typename 必须作为嵌套从属类型名称的前缀词” 这一规则的例外：

​	typename 不可以出现在 base classes list 内的嵌套从属类型名称之前，也不可以出现在成员初始化列表作为 base class 的修饰符：

````c++
template<typename T>
class Derived: public Base<T>::Nested // base class list 不允许使用 typename 
{
public:
	explicit Derived(int x)
    : Base<T>::Nested(x)			// 不可以在 成员初始化列表作为 base class 的修饰符
    {
        typename Base<T>::Nested temp;	// 这里就必须加
	}
};
````

​	还是比较好理解，因为继承肯定是继承一个类型，所以在 base class list 中不允许，之后 成员初始化列表 也是同理；



## 请记住：

- 声明 template 参数时，前缀 class 与 typename 等效；
- 请使用 typename 标识嵌套从属类型名称；但不得在 base class list 或 member initialization list 中以 typename 作为 base class 的修饰符；





 