#include <iostream>


namespace Space1
{
    class Timer
    {
    public:
        Timer(int tickFrequency): _tickFrequency(tickFrequency)
        {}

        virtual void onTick() const
        {
            std::cout << "every " << this->_tickFrequency << "second " << " do something~" << std::endl;
        }

        int getFrequency() const
        {
            return this->_tickFrequency;
        }
    private:
        int _tickFrequency;
    };

    // private inheriate
    class Widget: private Timer
    {
    public:
        Widget(int tickFrequency): Timer(tickFrequency)
        {}
    private:
        virtual void onTick() const
        {
            std::cout << "every " << this->getFrequency() << "second " << " record Widget~" << std::endl;
        }
    
    public:
        void show() const 
        {
            this->onTick();
            std::cout << "show widget" << std::endl;
        }
    };

    void test()
    {
        Widget w(10);
        w.show();
    }

};

namespace Space2
{
    class Timer
    {
    public:
        Timer(int tickFrequency): _tickFrequency(tickFrequency)
        {}

        virtual void onTick() const
        {
            std::cout << "every " << this->_tickFrequency << "second " << " do something~" << std::endl;
        }

    protected:
        int _tickFrequency;
    };

    // public + 复合
    class Widget
    {
    public:
        Widget(int tickFrequency): _widgetTimer(tickFrequency)
        {}
        void show() const 
        {
            this->_widgetTimer.onTick();
            std::cout << "show widget" << std::endl;
        }
    private:
        class WidgetTimer: public Timer
        {
        public:
            WidgetTimer(int tickFrequency): Timer(tickFrequency)
            {}
            virtual void onTick() const
            {
                std::cout << "every " << this->_tickFrequency << "second " << "record widget info~" << std::endl;
            }
        };

        WidgetTimer _widgetTimer;
    };

    void test()
    {
        Widget w(10);
        w.show();
    }

};

namespace Space3
{
    class Functions
    {
    public: 
        void func1(){}
        void func2(){}
        void func3(){}
    };

    class C1
    {
    public:
        int _a;
        Functions _functions;
    };

    class C2: private Functions
    {
    public:
        int _a;
    };

    void test()
    {
        std::cout << sizeof(Functions) << std::endl; // 1
        std::cout << sizeof(C1) << std::endl; // 8 
        std::cout << sizeof(C2) << std::endl; // 4
    }
}


namespace Space4
{
    class Animal
    {
    public:
        void eat() 
        {
            this->doEat();
        }
    private:
        virtual void doEat() 
        {
            std::cout << "..." << std::endl;
        }
        int _weight;
        void baseFunc(){}
    };

    class Wolf: public Animal
    {
    private:
        virtual void doEat()
        {
            std::cout << "eat meat" << std::endl;
        }
    };

    class Sheep: public Animal
    {
    private:
        virtual void doEat()
        {
            std::cout << "eat cao" << std::endl;
        }
    public:
        void func()
        {
            // 可以在子类中重载甚至 调用父类的私有虚函数
            doEat();
            // 但普通变量与函数不可以
            //this->_weight = 1;
            //baseFunc();
        }
    };

    void test()
    {
        Animal a;
        Wolf w;
        Sheep s;

        a.eat();
        w.eat();
        s.eat();

        s.func();
    }
}

int main()
{
    Space4::test();
    return 0;
}