#include <iostream>
#include "Strategy.h"

namespace Strategy
{
    // obj
    HealthCalcFunc defaultHealthCalc;
    BadGuyCalcFunc badGuyHealthCalc;
    
    int HealthCalcFunc::calc(const GameCharacter& gc) const
    {
        std::cout << "default~" << std::endl;
        return 1;
    }

    int BadGuyCalcFunc::calc(const GameCharacter& gc) const
    {
        std::cout << "BadGuy~" << std::endl;
        return 1;
    }

    GameCharacter::GameCharacter(HealthCalcFunc* hcf)
        : _pHealthCalc(hcf)
    {}

    BadGuy::BadGuy(HealthCalcFunc* hcf)
        : GameCharacter(hcf)
    {}

    int GameCharacter::healthValue() const
    {
        return _pHealthCalc->calc(*this);
    }



    void test()
    {
        BadGuy bg1(&defaultHealthCalc);
        BadGuy bg2(&badGuyHealthCalc);

        bg1.healthValue();
        bg2.healthValue();
    }
}