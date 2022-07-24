# 条款20：宁以pass-by-reference-to-const替换pass-by-value

​		Prefer pass-by-reference-to-const to pass-by-value

​	缺省情况下C++以 pass-by-value 的方式（一个继承自C的方式）传递对象至（或来自）函数；

​	除非特殊指定，否则函数参数都是以实际实参的复本（副本）为初值，而调用端所获得的也是函数返回值的一个副本；

​	这些复本（副本）系由对象的copy构造函数产出，这可能使得pass-by-value称为昂贵的（费事的）操作；

​	考虑以下继承体系：

````c++
class Person
{
public:
	Person();
    virtual ~Person();
    ...
private:
	std::string name;
    std::string address;
};

class Student: public Person
{
public:
    Student();
    ~Student();
    ...
private:
    std::string schoolName;
    std::string schoolAddress;
};
````

​	现在考虑以下代码，其中调用函数`validateStudent`，后者需要一个 `Student` 实参（by value）并且返回其是否有效：

````cpp
bool validateStudent(Student s);    //函数以by value方式接收学生
Student plato;						
bool platoIsOK = validateStudent(plato); // 调用函数
````

​	对此函数，以 plato 为蓝本将 s 初始化，调用一次构造函数；而在函数执行完毕后，调用 s 的析构函数销毁临时变量；

​	但故事远不及这么简单，Student 类继承自 Person，其中有两个 std::string 来自 Person，两个std::string 来自 Student 本身；

​	那么创建一个 Student 对象，需要先调用一个 Person 构造函数、一个 Student 构造函数、四个 std::string 构造函数；同样销毁时，需要调用一个 Person 析构函数、一个 Student 析构函数、四个 std::string 析构函数；

​	综上，一个 Student 的创建与销毁，总共成本是“六次构造函数与六次析构函数”！！！



​	这是正确并且值得拥有的行为，但是我们有更好的方式来==**回避这些构造与析构行为**==，就是:

​	pass by reference-to-const；

````cpp
bool validateStudent(const Student& s);
````

​	这种传递方式高效很多，没有任何构造函数与析构函数被调用，因为没有任何新对象被创建；

​	修改后的参数声明中 const 是很重要的，因为这样函数内就不能对外部的 Student 对象进行修改；

​	

​	以 by reference 方式传递参数也==**可避免 slicing （对象切割）问题**==：

​	当一个 derived class 对象以 by value 的方式传递并被视为一个 base class 对象，base class 的 copy 构造函数会被调用，而非调用 derived class 的构造函数，那么传入的对象就失去了 derived class 的所有特性，仅仅留下一个 base class 对象；

​	假设有这样的 classed

````cpp
class Window
{
public:
	...
    std::string name() const; 				// 返回窗口名称
   	virtual void display() const;			// 显示窗口和其内容
};
class WindowWithScrollBars: public Window
{
public:
    ...
    virtual void display() const;
};
````

​	所有 `Window` 对象都带有一个名称，可以通过 name 函数取得；

​	所有 `Window` 对象都带有一个显示函数，并且这是一个虚函数，那么就可以使用 Window 类型的指针指向不同的 derived class 对象的指针，实现多态；	

​	而引用的本质其实就是指针，那么我们就可以通过 by reference 的方式实现多态；

````c++
void printfNameAndDisplay(Window w) 		// 不正确，传入的子类对象切割
{
	std::cout << w.name();
	w.display();
}
void printfNameAndDisplay(const Window& w) 	// 不正确，传入的子类对象切割
{
	std::cout << w.name();
	w.display();
}
````

​			

​	窥探C++编译器底层，reference往往以指针实现出来，因此 pass by reference 通常意味着真正传递的是指针

​	一般而言，“pass-by-value并不昂贵“的唯一对象是内置类型以及 STL 的迭代器与函数对象；至于其他任何东西都请遵守本条款的忠告，尽量以 pass-by-reference-to-const 替换 pass-by-value；



## 请记住

- 尽量以 pass-by-reference-to-const 替换 pass-by-value。前者通常比较高效，并可避免切割问题；
- 以上规则并不适用于内置类型，以及 STL 的迭代器和函数对象。对它们而言，pass-by-value往往比较适当；

