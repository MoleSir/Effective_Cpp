#ifndef _NVI_
#define _NVI_

namespace NVI
{
    // base class
    class GameCharacter
    {
    public:
        int healthValue() const;

    private:
        virtual int doHealthValue() const;
    };

    // derived class
    class Solider: public GameCharacter
    {
    private:
        virtual int doHealthValue() const;
    };

    class RichGuy: public GameCharacter
    {
    };


    // test function
    void test();
}

#endif