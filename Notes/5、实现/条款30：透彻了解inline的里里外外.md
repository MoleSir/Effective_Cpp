# 条款30：透彻了解inline的里里外外

​		Understand the ins and outs of inline

​	Inline 函数可以免除函数调用的成本，可以使编译器对其进行最优化，但这可能增加目标码的大小，使得程序体积太大；

​	inline 只是对编译器的一个申请，不是强制的命令，这项申请可能隐式提出，也可以明确提出。隐喻方式是将函数定义于 class 定义式内：

````c++
class Person
{
public:
	...
    int age() const { return theAge; }
    ...
private:
	int theAge;
};
````

​	这样的函数通常是成员函数，而 friend 函数也可被定义于 class 内，如果真是这样，它们也被隐喻声明为 inline;

​	

​	明确声明 inline 函数的做法是在其定义式前面加上关键字 inline。例如标准库中的 max template:

````c++
template<typename T>
inline const T& std::max(const T& a, const T& b)
{ return a < b ? b : a; }
````

​	"max 是个 template“ 带出一项观察结果：我们发现 inline 函数和 template 两者通常被定义于头文件中。但 function template 不是必须为 inline:

​	Inline 函数通常一定被置于头文件中，因为大多数建置环境，在编译过程中进行 inlining 