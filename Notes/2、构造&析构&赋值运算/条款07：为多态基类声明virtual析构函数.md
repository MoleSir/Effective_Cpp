# 条款07：为多态基类声明virtual析构函数

​		Delete destructors virtual in polymorphic base classes

​	对这样一个例子：

````c++
class TimeKeeper{
public:
	TimeKeeper();
	~TimeKeeper();
	...
private:
	...
};
class AltomicClock: public TimeKeeper {...}; 
class WaterClock: public TimeKeeper {...};
class WristWatch: public TimeKeeper {...};
````

​	当我们使用工厂函数得到不同的钟表：

````c++
TimeKeeper* getTimeKeeper();
````

​	那么如果我们希望将得到的这些对象 `delete`，在现在的例子中，只能通过 `TimeKeeper` 的析构函数来销魂堆区数据，但如果子类也存在堆区数据，就出现了父类资源被释放，子类的却没有的情况；

​	解决方法很简单，给父类的析构函数加上 `virtual`：

````c++
class TimeKeeper{
public:
	TimeKeeper();
	virtual ~TimeKeeper();
	...
private:
	...
};
````

​	但注意，如果一个类不是基类，那么就不要给其析构函数加上 `virtual`，因为会带来虚函数表的额外开销；

​	这里还衍生出另一个话题，STL 中的容器的析构都是不带 `virtual ` 的，所以不要将其作为基类使用！

---



## 请记住

- polymprphic （带多态性质的）base classes 应该声明一个 virtual 析构函数。如果 classes 带有任何的 virtual 函数，其就应该拥有一个 virtual 析构函数；
- Classes 的设计目的如果不是作为 base classes 使用，不应该带 virtual 析构；