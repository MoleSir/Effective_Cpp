# 条款39：明智而审慎地使用private继承

​		Use private inheritance judiciously

​	我们已经知道 public 继承意味着 is a，编译器可以自动将 derived 对象转为 base，但对 private 继承不会，并且private 继承而来的一切基类属性都称为子类的 private；

​	那么 private 继承的意义其实是：implemented-in-terms-of （根据某物实现出），继承了实现却不能继承接口，在软件设计上无意义，只体现在实现上；

​	条款38指出，复合的意义也是根据某物实现出，那么如何在 private 继承与复合间取舍：

---> 尽可能使用 复合 ，必要时使用 private 继承，既：1、当 protect 属性与 virtual 函数被牵扯进来，2、或者涉及空间方面的利害关系，才去使用 private 继承；

​	下面分别来看这两个特殊情况：

---



​	Widget需要一个定时器，周期性得审核那份信息，我们有这样一个class：

````c++
class Timer
{
public:
	explicit Timer(int tickFrequency);
    virtual void onTick() const;		// 定时器每次滴答一次，此函数就被调用一次
};
````

​	这是我们拥有的东西，一个 Timer 对象，可以调制为以我们需要任何频率滴答前进，每次滴答就调用某个 virtual 函数，可以重新定义那个 virtual 函数，让后者取出 Widget 的当时状态；

​	那么为了让 Widget 可以重定义 Timer 的virtual 函数，我们需要让 Widget 继承 Timer ，但肯定不能是 public 继承，因为 Widget is not a Timer，所以我们应该使用 private 继承：

````c++
class Widget: private Timer
{
private:
    virtual void onTick() const;
    ...
};
````

​	这样使得 Timer 内的 public onTick 函数成为 Widget 内部的 private 函数，并且可以重新定义 onTick；

​	我们还可以使用这样一种设计：

````c++
class Widget
{
private:
	class WidgetTimer: public Timer
    {
   	public:
        virtual void onTick() const;
	};
	WidgetTimer timer;
    ...
};
````

​	这样的设计避免了 private 继承，但更加复杂；

​	对比这两个设计，设想一下，当 Widget 可能还会被别的 class 继承，那么如果使用 private 继承的设计， Widget 内部的 onTick 函数就可以被其子类重写（虽然子类是不能调用这个父类中的 private 函数）；

​	相反地，如果使用复合 + 继承的设计就可以避免这个问题，因为 子类 对 Widget 的 WidgetTimer 是不可见的；

---



​	再考虑第二个情况，当 base class 是一个没有数据的 class，其中只有一些function、typedefd，而 HoldAnInt 需要使用这个类来实现一些功能，首先考虑使用复合：

````c++
class Empty {};

class HoldAnInt
{
private:
    int x;
    Empty e;
};
````

​	Empty 对象理应是没有实际内存的，但 C++ 会给其填充一个 char ，也就是 e 占一个字节，那么一个 HoldAnInt 对象应该占有 5字节的大小，但这个还没有结束，根据对齐的需要，可能还会对这个对象进行一些填充，使得最后的大小远不止我们期待的一个 int 的大小；

​	但如果 Empty 被继承，也就说这个 class 并非独立：

````c++
class HoldAnit: private Empty
{
private:
	int x;
};
````

​	这样我们也使得 HoldAnit 根据 Empty 实现出，但是其对象的大小只是一个 int；

---



## 请记住：

- private 继承意味着 is-implemented-in-terms of ，它通常比复合的级别低，但是当 derived class 需要访问 protected base class 的成员，或需要重新定义继承而来的 virtual 函数时，这么设计是合理的；
- 和复合不同，private 继承可以造成 empty base 最优化。对致力于“对象尺寸最小化”的程序库开发者来说很重要；

