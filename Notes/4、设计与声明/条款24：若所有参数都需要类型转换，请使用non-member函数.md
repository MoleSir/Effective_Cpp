# 条款24：若所有参数皆需类型转换，请为此采用non-member函数

​		Declare non-member function when type conversion should apply to all parameters.

​	通常 class 支持隐式类型转换是糟糕的，当然有时例外，考虑这样的情况：

​	编写一个有理数类，允许整数“隐式类型转”为有理数很合理：

````c++
class Rational
{
public：
	Rational(int numerator = 0, int denominator = 1); // 不使用explict 	
	int numerator() const;
	int denominator() const;
private:
	...
};
````

​	此时还希望支持有理数的运算，我们不妨先使用 member 的运算符重载：

````c++
class Rational
{
public:
	...
	const Rational operator* (const Rational& rhs) const;
};
````

​	

​	那么我们有以下使用方式：

````c++
Rational oneEight(1, 8);
Rational oneHalf(1, 2);
Rational result = oneHalf * oneEight; 		// 很好
result = result * oneEight;					// 很好
````

​	这很好，但如果我们希望使用整数与有理数相乘时：

````c++
result = oneHalf * 2;						// 很好
result = 2 * oneHalf;						// 错误！！！
````

​	来分析一下两个语句的执行过程：

- 对第一句调用 oneHalf 对象的 operator* 函数，并且将 2 隐式转为 Rational(1, 2) 与 oneHalf 计算；
- 但对第二句，编译器只会在参数时帮助我们做类型转换，就像第一句一样；但此时 2 要作为 this对象，编译器是不允许这样的转换发生的；

​	可是，语句2应该也是合理的，所以我们需要使用一个 non-member 函数：

````c++
class Rational
{	
	...
};

const Rational operator* (const Rational& lhs, const Rational& rhs)
{
	return Rational(lhs.numerator() * rhs.numerator(), 
					lhs.denominator() * rhs.denominator());
}

Rational oneEight(1, 8);
Rational result;
result = oneEight * 2;
result = 2 * oneEight;
````

​	注意，这类的 operator* 不需要成为 friend 就可以完成工作，那么就不用声明其为 friend；

​	本条例不适应与所有时候，在 Template C++ 中， Rational 成为一个 class templat ，又会产生一些新的解法；



## 请记住：

- 如果你需要为某个函数的所有参数（包括被 this 指针所指的能够隐喻参数）进行类型转换，使用一个 non-member 函数吧；

