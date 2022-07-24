#include <iostream>
using namespace std;


int main()
{
    cout << "当你写下：" << endl;
	cout << " #define NUM 1.653  " << endl;
	cout << "标记NUM，在预处理已经被替换为1.653，编译器也许从没看见过，这个记号NUM没有进入记号表。" << endl;
	cout << "所以当其出错，编译器不会提示你NUM出错，错误信息可能只会提到1,653。而如果这个宏不是你定义的，你很可能找不到错误。" << endl;
	cout << "解决的办法是用常量替代宏" << endl;
	cout << " const double NUM = 1,653" << endl;
	cout << "除了以上的原因，使用宏可能导致预处理器盲目替换导致出现多份1,653占用内存" << endl << endl;
	cout << "使用常量替代#define 时，有两种特殊情况值得说说。" << endl;
	cout << "第一是当定义的是一个常量指针，比如需要一个常量字符串，那么我们需要让指针称为const，保证这个指针无法指向其他的地址：" << endl;
	cout << " const char* const authorName = 'yc'(主要应该使用双引号) " << endl;
	cout << "第二种情况为类内的专属常量，具体注解见头文件的类旁注释" << endl << endl;

    return 0;
}