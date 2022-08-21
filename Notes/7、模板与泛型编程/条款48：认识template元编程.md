# 条款 48：认识 template 元编程

​		Be ware of template metaprogramming

Template metaprogramming（TMP，模板元编程），是编写 template-base C++ 程序并执行于编译器的过程。

模板元编程是以 C++ 写成、执行于 C++ 编译器内的程序。一旦 TMP 程序结束执行，其输出，也就是从 template 具现出的若干 C++ 源码，便会一如往常地被编译；

将工作从运行期转移到编译器；

正如条款 47 实现的 `advance` 方法，我们使用函数重载配合 trait，可以把判断传入参数的类型这个部分工作交给编译器，而不是运行期，提高速度；

---

TMP 的起手程序是编译期间计算阶乘：

````c++
template <unsigned n>
struct Factorial
{
    enum { value = n * Factorial<n - 1>::value };
};
template <>
struct Factorial<0>
{
    enum { value = 1 };
};
````

使用方法：

````c
int main()
{
	std::cout << Factorial<5>::value;
	std::cout << Factorial<10>::value;
}
````

---



## 请记住

- 模板元编程可以将工作从运行期移往编译器，因而得以实现早期错误侦察和更高效的执行效率；
- TMP 可被用于生成基于政策选择组合的客户定制代码，也可用用来避免生成对某些特殊类型并不合适的代码；

