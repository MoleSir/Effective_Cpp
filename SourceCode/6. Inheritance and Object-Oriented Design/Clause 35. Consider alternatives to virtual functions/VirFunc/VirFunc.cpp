#include <iostream>
#include "VirFunc.h"


namespace VirFunc
{
    // defualt health calc function
    int GameCharacter::healthValue() const
    {
        std::cout << "default!" <<std::endl;
        return 1;
    }


    // special calc function
    int KindGuy::healthValue() const
    {
        std::cout << "KindGuy" << std::endl;
        return 1;
    }


    // test function
    void test()
    {
        VirFunc::KindGuy kindGuy;
        kindGuy.healthValue();

        VirFunc::NormalGuy normalGuy;
        normalGuy.healthValue();
    }
}