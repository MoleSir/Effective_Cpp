#include <iostream>
#include "NVI.h"

namespace NVI
{
    int GameCharacter::healthValue() const
    {
        // before work
        int retVal = this->doHealthValue();
        // after work

        return retVal;
    }

    int GameCharacter::doHealthValue() const
    {
        std::cout << "default!" << std::endl;
        return 1;
    }

    int Solider::doHealthValue() const
    {
        std::cout << "Solider health~" << std::endl;
        return 1;
    }

    // test function
    void test()
    {
        Solider solider;
        solider.healthValue();

        RichGuy richGuy;
        richGuy.healthValue();
    }
}
