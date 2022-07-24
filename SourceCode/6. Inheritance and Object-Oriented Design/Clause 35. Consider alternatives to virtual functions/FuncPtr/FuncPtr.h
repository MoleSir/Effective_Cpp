#ifndef _FUNCPTR_
#define _FUNCPTR_

namespace FuncPtr
{
    class GameCharacter;
    
    // defualt function to calc health
    int defaultHealthCalc(const GameCharacter& gc);

    // other way to calc health
    int loseHealthQuickly(const GameCharacter& gc);
    int loseHealthSlowly(const GameCharacter& gc);

    // base class
    class GameCharacter
    {
    public:
        // typedef
        typedef int (* HealthCalcFunc) (const GameCharacter&); 

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