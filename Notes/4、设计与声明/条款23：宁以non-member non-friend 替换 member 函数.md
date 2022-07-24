# 条款23：宁以non-member、non-friend 替换member函数

​		Prefer non-member non-friend function to member function

​	想象有个class来表示网页浏览器。这样的class可能提供众多函数中，有些来清除下载元素缓冲器、清除访问过的 URLs、以及移除系统中所有的 cookies：

````c++
class WebBrowser
{
public:
    ...
    void clearCache();
    void clearHistory();
    void removeCookies();
    ...
};
````

​	许多用户可能会想执行这些所有动作，因此 WebBrowser 也提供这样一个函数：

````c++
class WebBrowser
{
public:
    ...
    void clearEverything(); // 函数内部调用上述三个函数
};
````

​	当然，也可以使用一个 non-member 函数来调用

````c++
void clearBrowser(WebBrowser& wb)
{
   	wb.clearCache();
    wb.clearHistory();
    wb.removeCookies();
};
````

​	那么用哪一个更好呢？



​	面向对象守则要求，数据以及操作数据的那些函数应该被捆绑在一起，这意味着它建议 member 函数是较好的选择。

​	但不幸的是这个建议不准确。这是基于面向对象真是意义的一个误解。

​	面对对象守则要求数据应该尽可能被封装，然而与直观相反，member 函数带来的封装性比 non-member 函数低。此外提供 non-member 函数可以允许对 WebBrowser 相关机能有较大的包裹弹性，而那最终导致较低的编译相依度，我们需要了解其原因：

- 首先讨论封装，越多东西被封装，我们改变那些东西的能力也越大，因为它们对用户不可见。而显然，使用 member 函数对类内部的成员都是可见的，那么其封装性就不如 non-member 函数；

- 并且，这个对于 WebBrowser 来说是 non-member 函数的函数也可以成为别的类内函数，使用一个工具类来调用 WebBrowser 内部的函数，这对一些只能在类定义函数的语言是件好事；

    在  C++ 中，比较自然的是让 clearBrowser 成为一个 non-member 函数并且位于与 WebBrowser 在同一个命名空间里：

````c++
namespace WebBrowserStuff
{
    class WebBrowser {...};
    void clearBrowser(WebBrowser& wb);
    ...
};
````

​	

​	一个像 WebBrowser 的类可能有大量变量函数，有些与书签有关、有的与打印有关等等。大部分的用户可能只对其中一类感兴趣。没道理一个只对书签相关便利函数的客户与打印相关便利函数之间发生编译相依关系。

​	分离它们的方法就是将不同不同类的便利函数（就像clearBrowser）放在不同的头文件中：

```c++
// 头文件 "webbrowser.h" 中
namespace WebBrowserStuff
{
    class WebBrowser {...};
	...				// 核心机能，例如几乎所有用户都需要使用的便利函数
};
// 头文件 "webbrowserbookmarks.h" 中
namespace WebBrowserStuff
{
    ... 			// 与书签相关的便利函数
}
// 头文件 "webbrowsercookiemarks.h" 中
namespace WebBrowserStuff
{
    ... 			// 与cookie相关的便利函数
}
```

​	这正是 C++ 标准程序库的组织方式；





## 请记住：

- 宁可拿 non-member non-friend函数替换 member 函数。这样做可以增加封装性、包裹弹性和机能拓展性；

​	