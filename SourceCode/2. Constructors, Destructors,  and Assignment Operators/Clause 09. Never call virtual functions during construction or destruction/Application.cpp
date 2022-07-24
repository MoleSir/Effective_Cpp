#include <string>

#define CLAUSE_09 "Clause 09. Never call virtual functions during construction or destruction"


//=======================================================//
//  an example: call virtual function in contruction     //
//=======================================================//

namespace test0
{

// base class -->  call a virtual function in contruction 
class Transaction
{
public:
    Transaction();
    virtual void logTransaction() const = 0;

    // ...
};  


Transaction::Transaction()
{
    //...
    logTransaction();
}


// some derived classes:
class BuyTransaction: public Transaction
{
public:
    // definition for base class's virtual function
    virtual void logTransaction() const
    {
        // ...
    }
};

class SellTranscation: public Transaction
{
public:
    // definition for base class's virtual function
    virtual void logTransaction() const
    {
        // ...
    }
};

void test0()
{
    // if we do this: 
    BuyTransaction b;

    // what will happen?
    // it will cause a link error:
    // we define a BuyTransction object, 
    // before the BuyTransaction's construction be called, it's base class Tansaction 
    // will be called firstly. Bacause there is no verived class object now.
    // the virtual function in Transaction's construct is undefined.

    // the same thing happen in destruction:
    // if you call a virtual function in base destruction, 
    // the verived class's destruction will be called fristly, so when the base class's destruction
    // to call the virtual function, it will be an undefined because the verived class was been free.

    // but luckly this behavier will be found by link successfully
}

}




//=======================================================//
//       an example: a more dangerous behavier           //
//=======================================================//

namespace test1
{
    class Transaction
    {
    public:
        // in contruction: call a non-virtual function
        Transaction()
        { init(); }

        virtual void logTransaction() const = 0;

    private:
        void init()
        { 
            logTransaction();
        }
    };


    // some derived classes:
    class BuyTransaction: public Transaction
    {
    public:
        // definition for base class's virtual function
        virtual void logTransaction() const
        {
            // ...
        }
    };

    class SellTranscation: public Transaction
    {
    public:
        // definition for base class's virtual function
        virtual void logTransaction() const
        {
            // ...
        }
    };



    void test()
    {   
        // what will happen?
        SellTranscation s;

        // it hard to find error, because the construct call an non-virtual function
    }
}


namespace test2
{
    // but if we really need this situation:
    // we have a base class and some derived classes.
    // we build different derived classes, hoping to make the base class's 
    // data is different too. 
    // we can use the construct's arguments to pass different information for base class:
    // for example:
    
    class Book  
    {
    public:
        // use construct's arguments
        Book(const std::string& name) : _name(name) {}
    private:
        std::string _name;
        // ... 
    };

    class MathBook: public Book 
    {
    public:
        // derived class pass the arguments for base class:
        MathBook(const std::string& name) : Book(name) {}
    };

    class ChineseBook: public Book
    {
    public:
        // derived class pass the arguments for base class:
        ChineseBook(const std::string& name) : Book(name) {}
    };
}


int main()
{
    //test::test0();
    //test1::test();

    return 0;
}