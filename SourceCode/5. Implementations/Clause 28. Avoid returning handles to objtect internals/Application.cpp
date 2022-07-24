#include <iostream>
#include <memory>

namespace shape
{
    // point
    class Point
    {
    public:
        Point(int x, int y): _x(x), _y(y)
        {}
        Point(const Point&) = default;
        // setter
        void SetX(int newValue)
        { this->_x = newValue; }
        void SetY(int newValue)
        { this->_y = newValue; }
        // getter
        int GetX() const 
        { return this->_x;}
        int GetY() const
        { return this->_y; }
    private:
        int _x, _y;
    };

    // RectData
    struct RectData
    {
        Point _ulhc;
        Point _lrhc;
        RectData(const Point& ulhc, const Point& lrhc) :
            _ulhc(ulhc), _lrhc(lrhc)
            {}
    };

    // Rect 
    class Rectangle
    {
    public:
        Rectangle(const Point& ulhc, const Point& lrhc) : _rectData(ulhc, lrhc)
        { }
    public:
        const Point& upperLeft() const { return _rectData._ulhc; }
        const Point& lowerRight() const { return _rectData._lrhc; }
    private:
        RectData _rectData;
    };
}

int main()
{
    return 0;
}