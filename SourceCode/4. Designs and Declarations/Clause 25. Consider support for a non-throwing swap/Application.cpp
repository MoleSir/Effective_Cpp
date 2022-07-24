#include <iostream>


namespace WidgetStuff
{
    // pImpl
    class WidgetImpl
    {
    public:
        WidgetImpl(int a, int b): _a(a), _b(b)
        {

        }
        WidgetImpl(const WidgetImpl& rhs)
        {
            _a = rhs._a;
            _b = rhs._b;
        }
        WidgetImpl& operator = (const WidgetImpl& rhs)
        {
            _a = rhs._a;
            _b = rhs._b;
        }
    private:
        int _a, _b;
    };

    // Widget 
    class Widget
    {
    public:
        Widget(const Widget& rhs)
        {
            *(pImpl) = *(rhs.pImpl);
        }
        Widget& operator = (const Widget& rhs)
        {
            *(pImpl) = *(rhs.pImpl);
        } 

        void swap(Widget& that)
        {
            std::cout << "member swap" << std::endl;
            std::swap(pImpl, that.pImpl);
        }
    private:
        WidgetImpl* pImpl;
    };

    void swap(Widget& a, Widget& b)
    {
        std::cout << "WidgetStuff::swap" << std::endl;
        a.swap(b);
    }
}


namespace std
{
    using WidgetStuff::Widget;
    template<>
    void swap<Widget> (Widget& a, Widget& b)
    {
        std::cout << "std::swap()" << std::endl;
        a.swap(b);
    }
};



int main()
{


    return 0;
}