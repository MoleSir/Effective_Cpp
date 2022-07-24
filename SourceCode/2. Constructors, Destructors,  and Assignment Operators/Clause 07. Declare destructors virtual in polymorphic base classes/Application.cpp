#include <string>

#define CLAUSE_07 "Clause 07. Declare destructors virtual in polymorphic base classes"
// 为多态基类声明virtual析构函数



//====================================================//
//    a base class whith non-virtual destruct         //
//====================================================//

// there are lost of ways to record the time, we design a TimeKeeper (base class)
// and some derived classes as the different ways to record the time:

// base class: 
class TimeKeeper
{
public:
    TimeKeeper() {  /* ... */  }
    ~TimeKeeper() { /* ... */  }
    // ...
};

// derived classes:
class AtomicClock: public TimeKeeper { /*  ...  */  }; 
class WaterClock: public TimeKeeper { /*  ...  */  };   
class WristClock: public TimeKeeper { /*  ...  */  };


void test0()
{
    // and we choose an way to record the time: 
    // for example atomic:
    TimeKeeper* ptk = new AtomicClock();

    // ...


    delete ptk;

    // in this case, this very dangerous:
    // a derived class's pointer delete as a base class's pointer:
    // delete an AtomicClock pointer by TimeKeeper's destruct 
    // when delete be carried, call the TimeKeeper's destruct which is no virtual
    // so carry this destruct function.
    // of course, the heap data in TimeKeeper will be free, but these in AtomicClock
    // won't.  
}





//====================================================//
//     a base class whith virtual destruct            //
//====================================================//

// So how to deal the problem in test0s:
// add the virtual in Timekeeper's destrcut.
// base class: 
class TimeKeeper1
{
public:
    TimeKeeper1() {  /* ... */  }
    virtual ~TimeKeeper1() { /* ... */  }
    // ...
};

// derived classes:
class AtomicClock1: public TimeKeeper1 { /*  ...  */  }; 
class WaterClock1: public TimeKeeper1 { /*  ...  */  };   
class WristClock1: public TimeKeeper1 { /*  ...  */  };


void test0()
{
    // and we choose an way to record the time: 
    // for example atomic:
    TimeKeeper1* ptk = new AtomicClock1();

    // ...


    delete ptk;

    // When delete happened: because of virtual of destruct
    // the AtomicClock1's destruct will be called, so it is safe.
}






//====================================================//
//     a common class whith virtual destruct          //
//====================================================//

// if a class do not be used as a base class, giving it's a virtual destruct is not a good iead.
// for example: this class:
class Point
{ 
public:
    Point(int xCoord, int yCoord);
    ~Point();

private:
    int x;
    int y;
};

// for this class, a Point object can be contained within a 64-bits space.
// Also, it can be considered as a "64-bit" to pass to other language such C and FORTRAN.
// But if you add a virtual desrtruct for it, it's object size will incread bacause of the 
// virtual table (vtbl) which stores the virtual functions' pointer.
// In 64 bit system, a pointer will have 64-bit space and it lets the waste of memory.





//====================================================//
//                              P   S                 //
//====================================================//

// some class do not have virtual destruct such as std::string and some other containters
// in STL. If you use them as a base class:

class SpecialString: public std::string
{ /*  ...  */};


void test2()
{ 
    // and you do this:
    std::string* ps = new SpecialString();

    delete ps;

    // it dangerous!

    // so do not use this class as base class
}






int main()
{   


    return 0;
}

