# 条款25：考虑写出一个不抛异常的swap函数

​		Consider support for a non-throwing swap.

​	std标准库中给出了 swap 函数的定义为：

````c++
namespace std
{
    template<typename T>
    void swap(T& a, T& b)
    {
        T temp(a);
        a = b;
        b = a;
	}
}
````

​	那么只要我们传入的参数对象是有定义 copying 构造以及 copying assignment 操作符，就可以使用这个模板函数；

​	可以看出，std 内部的 swap 函数把交换两个对象的值解释为了两次互相复制的过程；这看起来是比较合理，但也有例外；

​	考虑“以指针指向一个对象，内含真正的数据”那种类型，这种设计的常见表现形式是所谓的"**pimpl手法**"（pimpl 是“pointer to implementation” 的缩写），使用这种手法编写一个类：

````c++
class WidgetImpl				// 针对 Widget 数据设计的 class;
{
public:
    ...
private:
    int a, b, c;				// 可能有很多数据
    std::vector<double> v;		// 意味着复制时间很长
};

class Widget					// 这个class使用pimpl手法
{
public:
    Widget(const Widget& rhs);
    Widget& operator = (const Widget& rhs)
    {
        ...
        *(pImpl) = *(rhs.pImpl);		// 调用 WidgetImpl 的 operator = 复制一份
        ...
	}
    ...
private:
    WidgetImpl* pImpl;			// 指针，所指对象内含有 Widget 的数据
};
````

​	这样的设计合情合理，作为 Widget 的 operator = ，理所应当将 rhs 内部的数据复制一份到 this 里来；

​	但这就出现了我们刚刚说的问题，如果我们就这样将两个 Widget 对象放入 std::swap 中进行交换，swap 会先构造一个临时的对象 temp，这需要复制一整个 WidgetImpl，之后又要调用两次 operator = ，这有是两次大动作；

​	但是其实交换两个 Widget 对象，其实只需要把其中两个 pImpl 指针对换即可；

​	所以我们希望高速 std::swap ：当 Widget 被对换时，不需要构造出一个新的 Widget 对象，只要把内部指针交换即可；实现的做法是：将 std::swap 特化。以下是基本构思，但其无法编译：

````c++
namespace std
{
    template<>							// ”T 是 Widget“ 的特化版本
    void swap<Widget>(Widget& a, Widget& b)
    {
        swap(a.pImpl, b.pImpl);			// 直接调用了私有成员，无法编译
	}
}
````

​	函数一开始的 `template<>` 表示其为 std::swap 的一个全特化版本，函数名称后的 `<Widget>` 表示这个特化版本是针对 "T 是 Widget" 的情况而设计；

​	通常我们不被允许改变 std 命名空间内的任何东西，但是我们可以为标准的模板制作特化版本；



​	但现在这个函数无法编译，有一个办法是在 Widget 内部声明一个 swap 的 public 函数：

````c++
class Widget					// 与前同，唯一的差别是增加 swap 函数
{
public:
    ...
    void swap(Widget& other)
    {
        using std::swap;		
        swap(pImpl, other.pImpl);	// 在 Widget中的swap函数调用std::swap
	}
    ...
};
namespace std
{
    template<>						// 修改后的特化版本
    void swap<Widget>(Widget& a, Widget& b)
    {
        a.swap(b);					// 调用对象a的swap函数
	}
}
````

​	这样的作法不只可以通过编译，而且还与 STL 容器有一致性，因为所有的 STL 容器也都有这样的 public swap成员函数和 std::swap 的特化版本；

​	

​	然而假设 Widget 和 WidgetImpl 都是 class template，而非 class，也许我们可以试试将 WidgetImpl 的数据类型加以参数化：

````c++
template<typename T>
class WidgetImpl{...};

template<typename T>
class Widget{...};
````

​	那么此时再考虑我们之前的方法，在 Widget 中加入 swap 函数去调用 std::swap，以及特化 std::swap 的Widget<T> 版本，那么代码看起来是这样的：

````c++
namespace std
{
    template<typename T>
    void swap< Widget<T> >(Widget<T>& a, Widget<T>& b)
    {
        a.swap(b);
	}
}
````

​	看起来合情合理，但不合法。此时我们写的swap不再是一个全特化版本，而是一个偏特化，C++只允许对 class template 再偏特化，不允许对 function template 偏特化；

​	

​	想要给 function template 增加偏特化的方法是为其添加一个重载版本：

````c++
namespace std
{
	template<typename T>					// 这里没有作任何特化
    void swap(Widget<T>& a, Widget<T>& b)	// 函数参数列表与std::swap同，是一个函数重载
    {
        a.swap(b);
	}
}
````

​	一般来说，这样重载一个函数没有问题，但是 std 是一个特殊的命名空间，我们可以全特其中的内容，但不可以增加新的 classed 或者 functions ；不要添加任何新东西到 std 里面；



​	那么解决办法就是，不使用 std 里的 swap 函数，我们将 Widget 的所有内容都放入到一个命名空间，在这里编写我们的 non-number swap：

````c++
namespace WidgetStuff
{
    ...
    template<typename T>
    class Widget{...};
    ...
    template<typename T>
    void swap(Widget<T>& a, Widget<T>& b)
    {
        a.swap(b);
	}
}
````

​	这样，不管你在哪里交换两个 Widget 对象而调用 swap，C++的名称查找法则（name lookup rules：更确切的说是所谓的 argument-dependent lookup 或者 Koenig lookup 法则）会找到 WidgetStuff 内的 Widget 专属 swap 版本；

​	

​	至此，我们讨论了当 Widget 作为 class 或者 template class 时，应该如何去编写 swap ；

​	对 class 版本，只要在 Widget 中实现 public swap 函数，去调用 `swap(pImpl, other.pImpl);` 对于两个指针，当然 std::swap 可以交换它们；然后再实现对 std::swap 特化 Widget ，完成 `a.swap(b)`，调用成员函数的操作；

​	对 template class 版本，我们不能为 function 编写偏特化，也不能为 std::swap 增加重载版本。那么我们最后将所有的 Widget相关内容放入到 WidgetStuff 命名空间中，之后再这里编写一个 swap完成与 class 版本类似的功能；

​	现在看起来，我们似乎可以像对 class template 一样，也对 class 实行相同的方法，而不是去特化 std::swap；

​	但是其实我们应该为 class 特化 std::swap；

​	因为如果希望 ”class专属版“ swap 可以被尽可能多的语境下使用，需要在 class 所在的命名空间中写一个 non-member 版本以及一个 std::swap 特化版本；



​	下面解释一下原因：

​	假设有一个 function template，需要置换两个对象值：

````c++
template<typename T>
void doSomething(T& obj1, T& obj2)
{	
    ...
    swap(obj1, obj2);
    ...
};
````

​	问题是：应该调用哪个版本的 swap? 是 std::swap 既有的版本？还是某个 std::swap 特版本？还是某个存在于 T 命名空间下的 T 专属版本？我们希望调用 T 专属版本，如果没有存在再去调用  std::swap；

​	那么我们应该这样写：

````c++
template<typename T>
void doSomething(T& obj1, T& obj2)
{	
	using std::swap;
    ...
    swap(obj1, obj2);
    ...
};
````

​	当编译器看到这样 swap 的调用，根据C++的名称查找法，先去 global 作用域或者 T 所在命名空间中的任何关于 T 的专属版本 swap；如果没有就去调用 std::swap；并且编译器会先查看 std::swap 是对 T 全特化，如果有优先调用；



​	但是请不要这样写：

````c++
std::swap(obj1, obj2);
````

​	这样就是强迫编译器只能去 std::swap 中调用，先查看 std::swap 是否有 T 的特化版本，再去调用一般化的  std::swap，这样我们就永远不能调用 T 的专属 swap；

​	而这也就是为什么希望对 class 也希望同时特化 std::swap 又专门在其命名空间中编写专属 swap 的原因：

防止有人像上面这样调用，而万一没有特化 std::swap 就只能调用效率很差的一般版 std::swap；



​	现在来做个总结：

​	首先，如果 std::swap 的默认版本不会影响效率，就不需要作任何多余的动作；

​	齐次，如果 std::swap 默认版本效率不足，尝试以下事情：

- 提高一个 public swap 函数成员，让其高效得置换类型的两个对象值，这个函数不能抛出异常；
- 在 class 或 class template  的命名空间里提供一个 non-member swap，并令其调用上述 swap 成员函数；
- 如果现在针对的是一个 class ，为你的 class 特化 std::swap。并令其调用 swap 成员函数（当然如果针对的是 template class 也写不出这样的 特化 std::swap）；

​	最后，如果调用 swap，请确定包含一个 using 声明，以便让 std::swap 得以使用，然后不加修饰得直接使用 swap，编译器会为你寻找最佳 swap 函数；

​	

​	现在来谈谈为什么成员 swap 不抛出异常：简单来说就是 swap 是提供强烈异常安全性的；（看得不是很懂这里为什么不能抛出异常）；



## 请记住：

- 当 std::swap 对你的类型效率不高，提供一个 swap 成员函数，并且确定其不抛出异常；
- 如果提供了 member swap，也应该提供一个 non-member swap 来调用前者。并且对于 class(非 template)，应该特化 std::swap；
- 调用 swap 时应该针对 std::swap 使用 using ，然后调用 swap 并且不加任何“命名空间修饰符”；
- 不要尝试在 std 中添加新东西，但可以对其中的模板进行特化；
