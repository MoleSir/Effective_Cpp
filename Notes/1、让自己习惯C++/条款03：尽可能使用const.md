# 条款03：尽可能使用const

​		Use const whenever possible

​	`const ` 有许多作用，可以在 classes 外部修饰 global 或 namespace 作用域的常量，或修饰文件、函数、或区块作用域（block scope）中被声明为 static 的对象。还可以修饰 classes 内部的 static 和 non-static  成员变量。面对指针，可以可以指定指针自身、指针所指之物，或两者都（或都不）是 const；

````c++
char greeting[] = "Hello";
char* p = greeting;					// non-const pointer, non-const data
const char* p = greeting;			// non-const pointer, const data
char* const p = greeting;			// const pointer, non-const data
const char* const p = greeting;		// const pointer, const data
````

​	规律就是：关键字 const 出现在星号左边，表示被指物是常量，就是指向的那个对象是不可以修改的；如果出现在星号右边，表示指针自身是常量，也就是说这个不能再修改指向；

​	所以 `const char * ` 与  `char const *` 其实是等价的；

​	**STL** 中的迭代器根据指针塑造，那么给一个迭代器加 const 意味着这个迭代器不得再指向别的东西，而 const_iterator 表示的是指向的对象是不可以修改的：

````c++
std::vector<int> vec;
...
const std::vector<int>::iterator iter = vec.begin();
*iter = 10;				// 没有问题
++iter;					// 错误
std::vector<int>::const_iterator cIter = vec.begin();
*cIter = 10;			// 错误
++cIter;				// 正确	
````

---



​	const 最重要的用法是对函数声明时的应用，在一个函数声明式中，const 可以和函数返回值、参数、函数本身产生关联；

​	有时令函数返回一个常数，可以降低因用户错误导致的意外。比如在一个计算两个复数之和的函数返回一个常量：

````c++
class Rational {...};
const Rational operator* (const Rational& lhs, const Rational& rhs);
````

​	避免了用户写出这样的代码：

````c++
Rational a, b, c;
...
(a * b) = c;
````

