/* 更改自身的 new-handler */
#include <iostream>

class NewHandlerHolder
{
public:
    NewHandlerHolder(std::new_handler handler):
        _handler(handler) {}
    ~NewHandlerHolder()
    {
        // 在析构中，把原处理函数设置回去
        std::set_new_handler(this->_handler);
    }
    // 静止复制移动
    NewHandlerHolder(const NewHandlerHolder&) = delete;
    NewHandlerHolder& operator = (const NewHandlerHolder&) = delete;
    NewHandlerHolder(NewHandlerHolder&&) = delete;
    NewHandlerHolder& operator = (NewHandlerHolder&&) = delete;

private:
    std::new_handler _handler;
};


template <typename T>
class NewHandlerSupport
{
public:
    static std::new_handler set_new_handler(std::new_handler newHandler) throw();
    static void* operator new (std::size_t size) throw(std::bad_alloc); 

private:
    static std::new_handler _currentHandler;
};

template <typename T>
std::new_handler NewHandlerSupport<T>::_currentHandler = nullptr;

template <typename T>
std::new_handler 
NewHandlerSupport<T>::set_new_handler(std::new_handler newHandler) throw()
{
    using self = NewHandlerSupport<T>;
    std::new_handler oldHandler = self::_currentHandler;
    self::_currentHandler = newHandler;
    return oldHandler;
}

template <typename T>
void*
NewHandlerSupport<T>::operator new (std::size_t size) throw(std::bad_alloc)
{
    std::cout << "operator new\n";
    // 先替换 new-handler，旧函数保证在 holder 对象
    NewHandlerHolder holder(std::set_new_handler(NewHandlerSupport<T>::_currentHandler));
    // 申请
    return ::operator new(size);
}

class Widget : public NewHandlerSupport<Widget>
{
public:
    Widget(int a) : _a(a) {}
private:
    int _a;
};


int main()
{
    Widget w(122);
    Widget* pw = new Widget(1);
    return 0;
}
