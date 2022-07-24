#include <iostream>


// test for one object has two more address:
namespace test1
{
class Base
{
public:
    Base(int a): _a(a)
    {}
protected:
    int _a;
};

class Derived_L1: public Base
{
public:
    Derived_L1(int a, int b): Base(a), _b(b)
    {}
    virtual void vfunc()
    {
        std::cout << "level 1" << std::endl;
    }
protected:
    int _b;
};

class Derived_L2: public Derived_L1
{
public:
    Derived_L2(int a, int b, int c): Derived_L1(a, b), _c(c)
    {}
    virtual void vfunc()
    {
        std::cout << "level 2" << std::endl;
    }
protected:
    int _c;
};

void testFunc()
{
    test1::Derived_L1 d1(1, 2);
    std::cout << &d1 << std::endl; // print: 0x61ff0
    test1::Base* pb = &d1;
    std::cout << pb << std::endl;  // print: 0x61ff4
}

}


// test of cast copy
namespace test2
{
class Window
{
public:
    Window(int w, int l): _width(w), _length(l)
    {

    }
    // change the value in Window
	virtual void onResize() 
    {
        _width = 4;
        _length = 5;
        std::cout << "Window::onRezie()" << std::endl;
    }
    // getter
    int width()const { return _width; }
    int length() const { return _length; }
protected:
    int _width, _length;
};

class SpecialWindow: public Window
{	
public:
    SpecialWindow(int w, int l, int c): Window(w, l), _color(c)
    {}
    virtual void onResize() {
        // use cast to call Window::onResize()
        static_cast<Window>(*this).onResize(); 
        _color = 0;								   
    }
    // getter
    int color() const { return _color; }
private:
    int _color;
};

void testFunc()
{
    SpecialWindow w(10,20,5);
    std::cout << w.color() << "," << w.width() << "," << w.length() << std::endl; 
    // print: 5,10,20

    w.onResize();
    std::cout << w.color() << "," << w.width() << "," << w.length() << std::endl;
    // print: 0, 10, 20
    // -> this's Window::onResize() no call
}

}

int main()
{
    test2::testFunc();
    return 0;
}