#include <iostream>

namespace ErrorCode
{
    class Shape
    {
    public:
        enum ShapeColor { Red, Green, Blue };
        // 所有形状都需要绘制
        virtual void draw(ShapeColor color = Red) const = 0;
    };

    class Rectangle: public Shape
    {
    public:
        // 赋予不同的缺省参数值，很糟糕！！！
        virtual void draw(ShapeColor color = Green) const
        {
            std::cout << "Draw " << color << " Rectangle" << std::endl;
        }
    };

    class Circle: public Shape
    {
    public:
        // 不指定缺省参数
        // 当以对象调用此函数，必须指定参数，因为静态绑定下这个函数并不能从base继承缺省参数
        // 但是若以指针（或引用）调用此函数，可以不用指定，因为动态绑定下这个函数会从其base继承缺省参数
        virtual void draw(ShapeColor color) const
        {
            std::cout << "Draw " << color << " Circle" << std::endl;
        }
    };


    void test()
    {
        Shape* pc = new Circle;
        Shape* pr = new Rectangle;
        pc->draw();
        pr->draw();

        Circle c;
        Rectangle r;

        //c.draw(); --> error
        r.draw();

        delete pc;
        delete pr;
    }
}

namespace NVIDesign
{
    class Shape
    {
    public:
        enum ShapeColor { Red, Green, Blue };
        void draw(ShapeColor color = Red) const
        {
            doDraw(color);
        }
    private:
        virtual void doDraw(ShapeColor color) const = 0;
    };

    class Rectangle: public Shape
    {
    private:
        virtual void doDraw(ShapeColor color) const = 0;
    };
}

int main()
{
    ErrorCode::test();

    return 0;
}