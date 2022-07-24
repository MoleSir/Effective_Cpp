#include <iostream>
#include <string>


class Window
{
public:
    Window(const std::string& name): _name(name) 
    {}
    Window(std::string&& name): _name(name)
    {}
    std::string name() const
    {
        return _name;
    }
   	virtual void display() const
    {
        std::cout << "Window~~" << std::endl;
    }
private:
    std::string _name;
};


class WindowWithScrollBars: public Window
{
public:
    WindowWithScrollBars(const std::string& name): Window(name) 
    {}
    WindowWithScrollBars(std::string&& name): Window(name)
    {}
    virtual void display() const
    {
        std::cout << "WindowWithScrollBars~~" << std::endl;
    }
};


// pass by value
namespace pass_by_value
{
    void printfNameAndDisplay(Window w) 		// 不正确，传入的子类对象切割
    {
    	std::cout << w.name() << std::endl;
    	w.display();
    }
}

// pass by reference
namespace pass_by_reference
{
    void printfNameAndDisplay(const Window& w) 	// 不正确，传入的子类对象切割
    {
	    std::cout << w.name() << std::endl;
	    w.display();
    }
}


int main()
{
    std::string name("window1");
    WindowWithScrollBars w(std::move(name));

    pass_by_reference::printfNameAndDisplay(w);
    pass_by_value::printfNameAndDisplay(w);

    std::cout << name.size();

    return 0;
}