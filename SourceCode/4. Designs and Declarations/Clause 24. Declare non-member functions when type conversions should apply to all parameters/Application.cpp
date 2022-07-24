#include <iostream>

class Rational
{
public:
	Rational(int numerator = 0, int denominator = 1): n(numerator), d(denominator)
    {

    } 	
	int numerator() const
    {
        return n;
    }
	int denominator() const
    {
        return d;
    }
    const Rational operator* (const Rational& rhs)
    {
        std::cout << "member function" << std::endl;

        return Rational(n*rhs.numerator(), d*rhs.denominator());
    }
private:
	int n, d;
};

std::ostream& operator << (std::ostream& os, const Rational& r)
{
    os << r.numerator() << "/" << r.denominator() << std::endl;
    return os;
}

const Rational operator* (const Rational& lhs, const Rational& rhs)
{
    std::cout << "non-member function" << std::endl;

	return Rational(lhs.numerator() * rhs.numerator(), 
					lhs.denominator() * rhs.denominator());
}

int main()
{
    Rational oneEight(1, 8);
    Rational oneHalf(1, 2);
    
    Rational result = oneHalf * oneEight; 		// 很好

    result = result * oneEight;					// 很好
    std::cout << result;

    result = 2 * result;
    std::cout << result;

    return 0;
}