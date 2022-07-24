#include <iostream>

class A
{
public:
    A(){ std::cout << "A construction!" << std::endl; }
    ~A() { std::cout << "A destruction~" << std::endl; }

private:
    char _a = 'a';
};


int main()
{   
    A *objA = new A;

    A *arrayA = new A[5];

    // loop to delete ,very dangerous
    // delete [] objA;
    delete objA;

    delete  [] arrayA;

    return 0;
}