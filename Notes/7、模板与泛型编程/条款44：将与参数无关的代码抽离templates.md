# 条款 44：将与参数无关的代码抽离 templates

​		Factor parameter-independent code out of templates 

通常使用模板 Templates 是节省时间和避免重复代码的方式；

但又是使用 templates 可能会导致代码膨胀：其二进制代码带有重复（或几乎重复）的代码、数据，或两者。其结果可能导致源码看起来合身而整齐，但目标码却不是；

比如这样的例子，为固定尺寸的正方形矩阵编写一个 template，该矩阵支持逆矩阵操作：

````c++
template <typename T, std::size n>	// template 支持 n × n 矩阵，类型为 T
class SquareMatrix
{
public:
    ...
    void invert();					// 求逆矩阵函数
};
````

这个模板类除了接收一个类型参数 T，还有一个非类型参数 n；

那么对同一个类型的，不同大小的矩阵，编译器会为它们实现不同的实例，即 `invert` 这个函数会重复：

````c++
SquareMatrix<double 5> sml1;
SquareMatrix<double 10> sml2;

sml1.invert();
sml2.invert();
````

上面两个 `invert` 函数完完全全重复的两份代码；

这就是 template 引出代码膨胀的典型例子；

好的方法是，使用一个带有参数的 `invert` 函数：

````cpp
template<typename T>
class SquareMatrixBase
{
protected:
	...
    void invert(std::size_t matrixSize);		// 给定尺寸求逆矩阵
    ...
};
template <typename T, std::size_t n>
class SquareMatrix: private SquareMatrixBas<T>
{
private:
    using SquareMatrixBase<T>::invert;
public:
   	...
    inline void invert() { this->invert(n); }
};
````

这样对同样类型，但不同大小的矩阵，调用到的就是同一个 `invert(n)`；

注意这里的子类 `invert` 是 inline 的；

---

上述是非类型参数可能带来的代码膨胀问题，有时类型参数也可能导致这样的结果；

比如 `vector<int>` 与 `vector<long>` 的成员函数在某些平台（int 与 long 都是 32 位）会产生完全相同的二进制代码；

或者对指针的，大多数平台上，所有指针都有相同的二进制表达，比如`list<const int *>`，`list<char *>` 在某些情况下有着相同的实现；

所以好的方式是，对模板中有指针 `T*` 的，应该令它们调用一个无类型指针 `void*` 的函数；

---



## 请记住：

- Template 生成多个 classes 和多个函数，所以任何 template 代码都不该与 某个造成膨胀的 template 参数产生依赖关系；
- 因非类型模板参数而造成的代码膨胀，往往可以消除，做法是以函数参数或 class 成员变量替换 template 参数；
- 因类型参数而造成的代码膨胀，往往可以降低，做法是让有完全相同的二进制表达式的具体类型共享实现代码；



