#include <iostream>
using namespace std;


#define CLAUSE_06 "Clause 06. Explicitly disallow the use of compiler-generated functions you do not wnat"
// 若不想使用编译器自动生成的函数， 应该明确拒绝：


// for some class, we do not hope there are two same objects, so we hope the copy construct and 
// assignment operator for these classes can be compilerd.
// but the compiler will write them if we do no write, so how can we stop it?


// way 1: use priavte:
// make the copy construct and copy assignment operator are priavte that we can't use them out of the class:
class HomeForSale
{
public:
    // ... 
    HomeForSale() {}

private:
    // ... 
    HomeForSale(const HomeForSale& that);
    HomeForSale& operator = (const HomeForSale& that);
};

// but the number functions and friend functions can call them





// way 2: use an special base class:
class Uncopyable
{
// protected construct and destruct can be inherited
protected:
    Uncopyable() {}
    ~Uncopyable() {}

// private copy construct and copy assignment operator can't be inherited:
private:
    Uncopyable(const Uncopyable& that);
    Uncopyable& operator = (const Uncopyable& that);
};


class HomeForSale1: private Uncopyable
{
};




// way 3: use c++11 's  delete:
class HomeForSale2
{
public:
    // default -> use compiler giving version
    HomeForSale2() = default;

    // use delete for these two functions:
    HomeForSale2(const HomeForSale2&) = delete;
    HomeForSale2& operator = (const HomeForSale2&) = delete;
};


int main()
{
    HomeForSale h1;
    HomeForSale h2;
    //HomeForSale h3(h1);    -->  wrong!!!
    //h2 = h1;               -->  wrong!!!

    HomeForSale1 h11;
    HomeForSale1 h12;
    //HomeForSale1 h13(h11); --> wrong!!!
    //h12 = h11;             --> wrong!!!

    HomeForSale2 h21;
    HomeForSale2 h22;
    //HomeForSale2 h23(h11); --> wrong!!!
    //h22 = h21;             --> wrong!!!

    return 0;
}