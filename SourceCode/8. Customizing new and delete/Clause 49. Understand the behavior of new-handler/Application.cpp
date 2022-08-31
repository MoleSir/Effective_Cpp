#include <iostream>

void OutOfMem()
{
    std::cout << "No More Memory~" << std::endl;
}

void test_new_handler()
{
    std::set_new_handler(OutOfMem);
    int* p = new int[1000000000L];
}


int main()
{
    test_new_handler();
    return 0;
}