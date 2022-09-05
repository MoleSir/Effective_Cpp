/* test placement delete */
#include <iostream>
#include <string>
#define THROW_EXCEPTION

class Widget
{
public:
    Widget(int data) : _data(data) 
    {
#ifdef THROW_EXCEPTION
        throw std::string("construct function throw a exception!\n");
#endif
    }
    // placement new 
    static void* operator new (std::size_t size, std::ostream& logStream) throw(std::bad_alloc);
    // 正常的 operator new
    static void* operator new (std::size_t size) throw(std::bad_alloc);
    // placement delete
    static void operator delete (void* pMemory) throw();
    // 正常的 operator delete
    static void operator delete (void* pMemory, std::ostream& logStream) throw();

private:
    int _data;
};

void* Widget::operator new (std::size_t size) throw(std::bad_alloc)
{
    std::cout << "operator new\n";
    return ::operator new(size);
}

void* Widget::operator new (std::size_t size, std::ostream& logStream) throw(std::bad_alloc)
{
    logStream << "placement new\n";
    return ::operator new(size);
}

void Widget::operator delete (void* pMemory) throw()
{
    std::cout << "operator delete\n";
    ::operator delete(pMemory);
}

void Widget::operator delete (void* pMemory, std::ostream& logStream) throw()
{
    logStream << "placement delete\n";
    ::operator delete(pMemory);
}


int main()
{
    Widget* pw = nullptr;
    try
    {
        Widget* pw = new (std::cout) Widget(12);
    }
    catch(std::string message)
    {
        std::cout << message << std::endl;
        exit(0);
    }

    delete pw;

    return 0;
}