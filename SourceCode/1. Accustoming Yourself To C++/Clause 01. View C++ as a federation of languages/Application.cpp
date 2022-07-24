#include <iostream>
using namespace std;


int main()
{
    cout << "如今的C++是一个多重范型的编程语言" << endl;
	cout << "一个同时支持过程形式（procedural）、面向对象（objected-oriented）、函数形式（functional）、泛型形式（generic）、元编程形式（metaprogramming）的语言" << endl;
	cout << "将C++分成主要的次语言：" << endl;
	cout << "① C" << endl;
	cout << "② Object-Oriented C++" << endl;
	cout << "③ Templace C++" << endl;
	cout << "④ STL" << endl;
	cout << "使用不同次语言时，需要改变策略。比如对内置类型（也就是C-like）类型而言pass-by-value通常比pass-by-reference高效"
		<< "但对Object-OrinentedC++中，用户自定义的类型由于构造函数、析构函数的存在，pass-by-reference 更高效" << endl;

	

    return 0;
}