#include <iostream>

class Top {
public:
    virtual void Print()
    {
        std::cout << "Top\n";
    }
};
class Middle: public Top {
public:
    virtual void Print()
    {
        std::cout << "Middle\n";
    }
};
class Buttom: public Middle {
public:
    virtual void Print()
    {
        std::cout << "Buttom\n";
    }
};


namespace bad_example
{
    template <typename T>
    class SmartPtr
    {
    public:
        SmartPtr(T* heldPtr) : _heldPtr(heldPtr)
        {}
        ~SmartPtr() 
        {
            delete this->_heldPtr;
        }
        SmartPtr(const SmartPtr&) = default;

        T* Get() const {  return this->_heldPtr; }

    private:
        T* _heldPtr;
    }; 

    void test()
    {
        //SmartPtr<Top> ptr = SmartPtr<Buttom>(new Buttom());
    }

}



namespace good_example
{
    template <typename T>
    class SmartPtr
    {
    public:
        SmartPtr(T* heldPtr) : _heldPtr(heldPtr)
        {
            std::cout << "construction\n";
        }
        ~SmartPtr() 
        {

            std::cout << "free\n";
            delete this->_heldPtr;
        }
        template <typename U>
        SmartPtr(const SmartPtr<U>& other) : _heldPtr(other.Get())
        {
            std::cout << "member function template, copy\n";
            other.SetPtr(nullptr);
        }
        template <typename U>
        SmartPtr(SmartPtr<U>&& other) : _heldPtr(other.Get())
        {
            std::cout << "member function template, move\n";
            other.SetPtr(nullptr);
        }

        T* Get() const {  return this->_heldPtr; }
        void SetPtr(T* ptr) { this->_heldPtr = ptr; }
    private:
        T* _heldPtr;
    }; 


    void test()
    {
        SmartPtr<Top> ptr = SmartPtr<Buttom>(new Buttom());
        Top* realPtr = ptr.Get();
        realPtr->Print();
    }

}



int main()
{
    // bad_example::test(); --> error
    std::cout << "test\n";
    good_example::test();
}