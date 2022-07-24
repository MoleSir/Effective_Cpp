#ifndef _FUNCOBJ_
#define _FUNCOBJ_

#include <functional>

namespace FuncObj
{
    class GameCharacter;
    
    // defualt function to calc health
    int defaultHealthCalc(const GameCharacter& gc);

    // use function, we can not only use int(const GameCharacter&) function to calc
    // but also can use class obj,member function
    // functor
    struct HealthCalculator
    {
        int operator() (const GameCharacter& gc) const;
    };

    // member func
    class GameLevel
    {
    public:
        float health(const GameCharacter& gc) const;
    };

    // base class
    class GameCharacter
    {
    public:
        // typedef
        typedef std::function<int(const GameCharacter&)> HealthCalcFunc;

        // constrct function
        explicit GameCharacter(HealthCalcFunc hcf = defaultHealthCalc);

        // health value
        int healthValue() const;

        // chaneg way to calc
        void SetHealthCalculator(HealthCalcFunc hcf);

    private:
        HealthCalcFunc _healthFunc;
    };

    // derived classes
    class EvilBadGuy: public GameCharacter
    {
    public:
        explicit EvilBadGuy(HealthCalcFunc hcf = defaultHealthCalc);
    };


    // test function
    void test();
}

#endif