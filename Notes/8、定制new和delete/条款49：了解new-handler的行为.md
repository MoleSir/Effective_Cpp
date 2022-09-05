# 条款 49：了解 new-handler 的行为

​		Understand the behavior of the new-handler

当 `operator new` 无法满足某一内存分配需求时，抛出异常；在旧式编译器还会返回一个 null 指针；

当 `operator new` 抛出异常以反映一个未满足的内存需求之前，会先调用用户指定的错误处理函数，即一个 `new-handler` 。用户可以调用 `set_new_handler` 来指定这个错误处理函数；

````c++
namespace std
{
    typedef void (*new_handler) (void);
    new_handler set_new_handler(new_handler p) throw();
}
````

- 参数：指向 `operator new` 无法分配足够内存时调用的函数；
- 返回值：指向上一个错误处理函数；

比如，可以这样使用：

````c++
void outOfMem()
{
	std::cout << "Unable to satisify request for memory\n";
	std::abort();
}
int main()
{
    std::set_new_handler(outOfMem);
    int* p = new int[10000000L];
    ...
}
````

如果 `operator new` 无法为 1000000 个整数分配空间，就会调用 `outOfMem`，提示用户后让程序退出；



当 `operator new ` 无法满足内存申请时，会不断调用 `new-handler` 函数，直到找到足够的空间。那么一个设计良好的 `new-handler` 函数必须做到：

- **让更多内存可被使用**：这可能使得不断调用 `new-handler` 后，有足够的内存；
- **安装另一个 `new-handler`**：如果目前的 `new-handler` 无法获取更多内存，那么可以在其中调用 `set_new_handler` 安装另一个可以获取内存的 `new_handler` ；那么下次 `operator new` 空间不足调用 `new-handler` 就调用到有获取内存的那个；
- **卸除 `new-handler`**：如果将 NULL 传给 `set_new_handler`，`operator new` 在内存分配不足时没有错误函数可以调用，那么就抛出异常；
- **抛出 `bad_alloc` 的异常**：这样的异常不会被 `operator new` 捕获，因此会转播到内存索求处；
- **不返回**：通常调用 `abort` 或 `exit`；



C++ 支持重载各个类各自的 `operator new`，但不支持 class 专有的 `new-handler`，那么这意味着，如果想给一个类添加独立的 `new-handler`，需要在每次申请此类空间前，先设置全局的 `new-handler`，在创建空间后再换回原来的；

而为了防止忘记更换为原来 `new-handler` 可以使用 RAII 技术来实现：

````c++
class NewHandlerHolder
{
public:
	explicit NewHandlerHolder(std::new_handler nh)
        : handler(nh) {}
    ~NewHandlerHolder()
    {
        std::set_new_handler(handler);
	}
private:
    std::new_handler handler;
    // 静止拷贝
    ...
};
````

- 在构造函数中保存上一个设置的全局 `new_handler`；
- 在析构函数中将上一个全局的 `new_handler` 设置回去；

之后就需要各个类重载 `operator new` ，在其中先更换 `new_handler`，并且将之前的 `new-handler` 放入到 `NewHandlerHolder` 保存，离开 `operator new` 后在更换回去；

显然这个 `new-handler` 函数是每个类独有的，那么可以设计为一个静态的成员保存；

最后可以设计出一个模板类：

````c++
template <typename T>
class NewHandlerSupport
{
public:
    // 模仿全局的 set_new_handler，为 T 更换 new-handler，其实就是改变 currentHandler
    static std::new_handler set_new_handler(std::new_handler p) throw();
    // 重载 operator new
    static void* operator new(std::size_t size) throw(std::bad_alloc);
private:
    // 保存某 T 的 new-handler
    static std::new_handler currentHandler;
};
template <typename T>
std::new_handler 
NewHandlerSupport<T>::set_new_handler(std::new_handler p) throw()
{
    std::new_handler oldHandler = NewHandlerSupport<T>::currentHandler;
    NewHandlerSupport<T>::currentHandler = p;
   	return oldHandler;
}
template <typename T>
void* 
NewHandlerSupport<T>::operator new (std::size_t size) throw(std::bad_alloc)
{
    // 将原来的 new-handler 保存到 RAII 对象中管理
    NewHandlerHoler h(std::set_new_hanler(NewHandlerSupport<T>::currentHandler));
    return ::operator new(size);
}
template <typename T>
std::new_handler NewHandlerSupport<T>::currentHandler = 0;
````

有了模板类后，为某个类比如 Widget 添加更换 new-handler 的功能就很简单，只要让 Widget 继承：

````c++
class Widget: public NewHandlerSupport<Widget>
{
	...
};
````

这样 Widget 就继承了来自 `NewHandlerSupport<Widget>` 的 `currentHandler`、`set_new_handler` 以及 `operator new` ；

可以不要把这个过程理解为一种继承关系，更像是把 `NewHandlerSupport<Widget>` 写好的函数、变量复制到 Widget 中一样；

并且在 `NewHandlerSupport<T>` 中，`T` 其实没有用到，使用模板就是为了对各个不同的类都有各自的一份实现；

像这样拥有一个类去继承一个模板类，并且后者以此类为类型参数的做法成为 "怪异的循环模板模式"（curiously recurring template pattern; CRTP）；



直到 1993 年，C++ 都要求 `operator new` 必须在无法分配内存时返回 NULL，但现在其应该抛出 `bad_alloc` 异常；为了兼容之前根据返回值判断的程序，C++ 也支持一种：分配失败返回 NULL的 `operator new` ；这种形式称为 `nothorw`  形式：

````c++
class Widget{...};
Widget* p1 = new Widget();
if (p1 = NULL) ... 					// 默认抛出异常，而不是返回 0
Widget* p2 = new (std::nothrow) Widget();
if (p2 = NULL) ...					// 分配失败返回 NULL
````

但是 Nothrow new 对异常强制保证性不高；因为它只规定申请内存不抛出异常；

但 `new` 需要完成两件事，首先申请内存，之后调用构造函数；所以构造函数中还是可能抛出异常，导致异常传播；所以别用 Nothrow new，尽可能使用检查 `std::bad_alloc` 来判断 `new` 的执行；



## 请记住

- `set_new_handler` 允许客户指定一个函数，在内存分配无法获得满足时被调用；
- Nothrow new 是一个颇为局限的工具，因为它只适用于内存分配；后续的构造函数调用还是可能抛出异常；