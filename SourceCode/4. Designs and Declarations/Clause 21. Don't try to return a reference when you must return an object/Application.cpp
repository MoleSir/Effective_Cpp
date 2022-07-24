#include <iostream>
#include <string>


// 这里主要演示如何使用右值引用来解决返回对象问题


class Rational
{
public:
	Rational(int numberator = 0, int denominator = 1): n(numberator), d(denominator)
    {
        std::cout << "CONSTRUCTION!!!" << std::endl;
    }
    Rational(Rational&& that): n(that.n), d(that.d)
    {
        std::cout << "RIGHT CONSTRUCTION!!!" << std::endl;
        that.n = 0;
        that.d = 0;
    }
    Rational(const Rational& that): n(that.n), d(that.d)
    {
        std::cout << "LEFT CONSTRUCTION!!!" << std::endl;
    }
private:
	int n, d;
friend 
    Rational&& operator* (const Rational& lhs, const Rational& rhs)
    {
        Rational result(lhs.n * rhs.n, lhs.d * rhs.d);
        return std::move(result);
    }
};

// 顺便复习右值引用
class MyRational
{
public:
    MyRational(const Rational& r): _r(r)
    {
    }
    MyRational(Rational&& r): _r(std::move(r))
    {
    }
private:
    Rational _r;
};

int main()
{   
    Rational a(2,3);
    Rational b(3,4);
    Rational c = a * b;

    MyRational mStr(c);
    return 0;
}