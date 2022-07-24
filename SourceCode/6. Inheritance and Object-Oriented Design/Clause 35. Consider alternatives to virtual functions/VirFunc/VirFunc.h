#ifndef _VIRFUNC_
#define _VIRFUNC_


namespace VirFunc
{
    // base class
    class GameCharacter
    {
    public:
        virtual int healthValue() const;
    };

    // derived classes
    class KindGuy: public GameCharacter
    {
    public:
        virtual int healthValue() const;
    };

    class NormalGuy: public GameCharacter
    {

    };

    void test();
}



#endif