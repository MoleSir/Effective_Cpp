#include <iostream>
#include <string>



#define CLAUSE_05  "Know what functions C++ silently writes  and calls"
// 了解C++默默编写并且调用了哪些函数


//=========================================================================//
//    when you write an Empty class, C++ will give these 4 functions:      //
//=========================================================================//


class Empty
{
public:
/*
    Empty() { ... }                               ==>  default construct  do nothing
    Empty(const Empty& that) { ... }              ==>  default copy construct just copy evety bits from taht to this
    (virtual) ~Empty() { ... }                    ==>  default destruct do nothing, if the class's base class has a virtual des, this class also will has a virtual
    Empty& operator = (const Empty& that) { ... } ==>  default assignemnt operator: copy each bits from that to this and return *this
*/
};



//================================================================================================//
//   but if you write these four functions, compiler will not add the default version for you     //
//================================================================================================//


template<typename T>
class NamedObject
{
public:
    NamedObject(const char* name, const T& value);
    NamedObject(const std::string& name, const T& value);

    // ... 

private:
    std::string nameValue;
    T objectValue;
};


//================================================================================//
//    but in some case: the compiler won not create default functions for you:    //
//================================================================================//

// case 1: 
template<typename T>
class NamedObject1
{
public:
    // this construct do no accepte a const type, Because now, the data nameValue is an reference
    // and the objectValue is a const T type:
    NamedObject1(std::string& name, const T& value);

    // we do not write a copy assignment
    // ... 

private:
    std::string& nameValue;  // reference
    const T objectValue;     // const
};


// case 2:
// a base class which has a private copy assignment:
class Base
{
private:
    Base& operator = (const Base& that) { return *this; }
};


// a derived class with no written copy assignment:
class Derived: public Base
{ 

};





// PS:  if you write your copy construct, compiler will not give you a default construct
class Book
{
public:
    Book(const Book& book) {}
};

// but write default construct while no copy construct, the compiler will give us an default copy construct version
class Pen
{
public:
    Pen(){}
};

int main()
{
    // NamedObject has ites construct: so NamedObject<int> no3() ==>  it's wrong
    NamedObject<int> no1("Smallest Prime NUmber", 12);

    // but no copy construct so has an default version
    NamedObject<int> no2(no1);  


    ////////////////// test case 1: //////////////
    std::string newDog("Pre");
    std::string oldDog("Satch");

    NamedObject1<int> p(newDog, 2);
    NamedObject1<int> o(oldDog, 36); 


    // p = s;   -->  this is wrong, because the C++ reject give reference and const a copy assignment move
    // if you want a copy assignment operator for a class which has reference or const data, 
    // write it by yourself.
    
    // but copy construct is legal:
    NamedObject1<int> t(p);

    Derived d1;
    Derived d2;

    ///d1 = d2;   ==>  wrong!!



    /////////////// test for PS: ////////////
    // Book b;    -->  wrong !!!  no default construct
    Pen p1;
    Pen p2(p1);

}




