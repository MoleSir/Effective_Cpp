#include <iostream>
#include "FuncPtr.h"


namespace FuncPtr
{
    // calc functions
    int defaultHealthCalc(const GameCharacter& gc)
    {
        std::cout << "default~~" << std::endl;
        return 1;
    }

    // other way to calc health
    int loseHealthQuickly(const GameCharacter& gc)
    {
        std::cout << "Quickly lose~~" << std::endl;
        return 1;
    }

    int loseHealthSlowly(const GameCharacter& gc)
    {
        std::cout << "Slowly lose~~" << std::endl;
        return 1;
    }

    // construction function to give a function pointer
    GameCharacter::GameCharacter(GameCharacter::HealthCalcFunc hcf)
        : _healthFunc(hcf)
    {}

    // healthValue
    int GameCharacter::healthValue() const
    {
        // call _healthFunc to work
        return _healthFunc(*this);
    }

    // set health calculator
    void GameCharacter::SetHealthCalculator(GameCharacter::HealthCalcFunc hcf)
    {
        this->_healthFunc = hcf;
    }

    // derived classes' construction
    EvilBadGuy::EvilBadGuy(GameCharacter::HealthCalcFunc hcf)
        : GameCharacter(hcf)
    {}

    // test function
    void test()
    {
        EvilBadGuy ebg1;
        EvilBadGuy ebg2(loseHealthQuickly);

        ebg1.healthValue();
        ebg2.healthValue();

        std::cout << "ebg2 change" << std::endl;
        ebg2.SetHealthCalculator(loseHealthSlowly);
        ebg2.healthValue();
    }

}