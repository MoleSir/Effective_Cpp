#ifndef _STRATEGY_
#define _STRATEGY_

namespace Strategy
{
    // before declaration
    class GameCharacter;

    // HealthCalcFunc base class
    class HealthCalcFunc
    {
    public:
        virtual int calc(const GameCharacter& gc) const;
    };

    // some derived classes
    class BadGuyCalcFunc: public HealthCalcFunc
    {
    public:
        virtual int calc(const GameCharacter& gc) const;
    };


    // GameCharacter class
    class GameCharacter
    {
    public:
        explicit GameCharacter(HealthCalcFunc* pchf);
        int healthValue() const;

    private:
        HealthCalcFunc* _pHealthCalc;
    };

    // derived class
    class BadGuy: public GameCharacter
    {
    public:
        explicit BadGuy(HealthCalcFunc* pchf);
    };


    // test func
    void test();
}

#endif