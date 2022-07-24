# 条款32：确定你的public继承塑模出is-a关系

​		Make sure public inheritance models "is-a"

​	如果令 class D("Derived") 以 public 形式继承 class B（”Base“），你便是告诉编译器：每一个类型为 D 的对象同时也是一个类型为 B 的对象，反之不成立；

​	C++ 对于 public 继承严格奉行上述见解，考虑以下代码：

````c++
class Person { ... };
class Student: public Person { ... };
````

​	根据生活经验，每个学生都是人，但并非每个人都是学生，这便是这个继承体系的主张；我们预期，对人成立的每一件事对学生也都成立；但我们并不预期对学生可成立的每一件事，对人也成立；学生是对人的一种特性形式；

​	于是，承上所述，在C++领域，任何函数如果期待获取一个类型为 Person（或 pointer-to-Person或reference-to-Person）的实参，都也愿意接收一个 Student 对象：

````c++
void eat(const Person& p); 			// 任何人都会吃
void study(const Student& s);		// 只有学生才到学习学习
Person p;							// p是人
Student s;							// s是学生
eat(p);								// 没问题，p是人
eat(s);								// 没问题，s是学生，而学生也是（is-a）人
study(s);							// 可以，学生学习
study(p);							// 不可以，p不是学生
````

​	注意这个论点只对 public 继承才成立。当 Student 以public 方式继承 Person，C++ 才有如此行为，而 private 的行为完全不同；



---

​	public 继承和 is-a 之间的等价关系听起来很简单，但有时候你的直觉可能会误导了你。举个例子，企鹅是鸟，但不会飞：

````c++
class Bird
{
public:
	virtual void fly();			// 鸟可以飞
    ...
};
class Penguin: public Bird
{
    ...
};
````

​	这个体系说明企鹅是可以飞的，而我们知道那是不可以的；

​	在这个例子中，我们成为了不严谨语言（人的语言）下的牺牲品。当我们说鸟可以飞的时候，我们真正的意思并不是所有鸟都会飞，而是一般的鸟有飞行能力，如果严谨一些，我们应该承认有些鸟不会飞，如下继承关系：

````c++
class Bird
{
};
class FlyingBrid: public Bird
{
public:
	virtual void fly();
    ...
};
class Penguin: public Bird
{
    ...						// 没有声明fly函数
}
````

​	即便如此，我们仍没有完全处理好这些鸟事，因为对某些软件系统而言，可能不需要区分会飞的鸟与不会飞的鸟。如果你的程序忙着处理鸟的别的事情，完全不在乎飞行，那么之前的双继承体系就相当令人满意。

​	这反映出一个事实，世界上并不存在一个”适合于所有软件“的完美设计；

​	

---

​	另有一种思想派别处理上述企鹅不会飞的办法是：重新定义企鹅的飞行，令其发生错误：

````c++
void error(const std::string& msg); // 定义于另外某处
class Penguin: public Bird
{
public:
	virtual void fly() { error("Attempt to make a penguin fly"); }
	...
};
````

​	但这样写的含义并不是企鹅不会飞，而是企鹅会飞，但是尝试那么做会发生错误；

​	区别就在于：”企鹅不会飞“这一限制是由编译期间强制实施的，但若违背”企鹅尝试飞行，是一种错误“这一规则，需要在允许期间才能检测；

​	而好的接口设计应该使得无效的代码不通过编译，那么我们应该在编译期间就拒绝企鹅飞行的设计；



----

​	现在再看一个别的例子：class Square（正方形）应该 public 继承 class Rectangle（矩形） 吗？

​	考虑这样的 Rectangle:

````c++
class Rectangle
{
public:
	virtual void setHeight(int newHeight);
    virtual void setWidth(int newWidth);
    virtual int height() const;
    virtual int width() const;
    ...
};
void makeBigger(Rectangle& r)
{
    int oldHeight = r.heigt();
    r.setWidth(r.width() + 10);		// 将 r 的宽度加10
    assert(r.height() == oldHeight);	// 判断r的高度是否改变
}
````

​	显然，这个判断一定是真，现在考虑让 Square 继承 Rectangle:

````c++
class Square: public Rectangle {...};
Square s;
...
assert(s.width() == s.height());
makeBigger(s);

assert(s.width() == s.height());
````

​	那么现在出现了问题，我们必须这样调解这几个 assert 判断式：

- 调用 makeBigger 之前，s 的高度与宽度相同；
- 在 makeBigger 函数内， s 的宽度改变，但高度不变；
- makeBigger 返回后，s 的高度再度与其宽度相同；

​	这个例子的困难在于：某些施加在矩形身上的事情（单独改变高，而宽度不变）却不可以施加在正方形上；

​	但 public 类型的继承主张：可以施加在 Base Class 的动作，也应该可以施加在 Derived class 身上；



​	is-a 并非是唯一存在于 classes 之间的关系，另外两个常见的关系是 has-a（有一个）和 is-implemented-in-terms-of（根据某物实现出）；



## 请记住：

- ”public继承“意味着 is-a。使用在 base classes 身上的事情一定适用在 derived 身上；