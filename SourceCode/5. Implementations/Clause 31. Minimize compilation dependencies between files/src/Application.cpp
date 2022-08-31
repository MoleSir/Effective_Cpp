#include "ClassA.h"
#include "ClassB.h"

int main()
{
    
    A a(12, 2.1, "RP", nullptr);
    B b(123, 122.122, "PPPP", &a);

    a.Print();
    b.Print();
    
    return 0;
}