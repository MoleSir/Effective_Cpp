#include "FuncObj.h"
#include <iostream>


namespace FuncObj
{
    // calc functions
    int defaultHealthCalc(const GameCharacter& gc)
    {
        std::cout << "default~~" << std::endl;
        return 1;
    }

    int HealthCalculator::operator() (const GameCharacter& gc) const
    {
        std::cout << "functor~" << std::endl;
        return 1;
    }

    float GameLevel::health(const GameCharacter& gc) const
    {
        std::cout << "member func~" << std::endl;
        return 1.0;
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



    // test func
    void test()
    {
        EvilBadGuy ebg1;

        HealthCalculator healthCalculator;
        EvilBadGuy ebg2(healthCalculator);

        GameLevel currentLevel;

        EvilBadGuy ebg3( std::bind( &GameLevel::health, currentLevel, std::placeholders::_1) );

        ebg1.healthValue();
        ebg2.healthValue();
        ebg3.healthValue();
    }
}