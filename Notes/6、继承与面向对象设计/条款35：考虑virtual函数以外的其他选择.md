# 条款35：考虑virtual函数以外的其他选择

​		Consider alternatives to virtual functions

​	假设这样的一个情况，为一个游戏软件的认为设计一个继承体系，游戏中的人物被伤害或因其他因素而减低健康状况。因此提供了一个成员函数 healthValue，返回一个整数，表示任务的健康程度。由于不同人物可以以不同方式计算他们的健康指数，将其设计为 virtual 函数：

````c++
class GameCharacter
{
public:
	virtual int healthValue() const;    	// 返回人物的健康指数
    ...										// derived classes 可以重定义
};
````

​	healthValye 并没有声明为 pure virtual，这暗示我们将会有一个计算健康指数的缺省算法；

​	这是很普通的设计，而我们可以使用其他几种方式代替：



## 籍由Non-Virtual Interface手法实现Template Method模式

​	保留 healthValue 为 public 成员函数，但让其成为 non-virtual，并调用一个 private virtual 函数：

````c++
class GameCharacter
{
public:
    int healthValue() const				// derived classes不会重新定义
    {
        ...								// 做一些事先工作
       	int retVal = doHealthValue();	// 做真正的工作
        ...								// 做一些事后工作
        return retVal;
    }
    ...
private:
    virtual int doHealthValue() const 	// derived classes可重新定义
    {
        ...
	}
};
````

​	这一基本设计，也就是“令客户通过 public non-virtual 成员函数间接调用 private virtual 函数“，成为 non-virtual interface（NVI）手法。它是所谓 Template Method 设计模式（与C++ template并无关联）的一个独特表现形式，这个 non-virtual 函数称为 virtual 函数的外覆器（子类继承了基类的私有虚函数）；

​	

## 藉由Function Pointers 实现 Strategy 模式

​	在人物内部加入函数指针，当需要计算健康程度时，实际上是调用这个函数指针去执行：

````c++
// 前置声明
class GameCharacter;					
// 默认计算健康程度函数，接收 一个GameCharacter引用
int defaultHealthCalc(const GameCharacter& gc);

// GameCharacter 类的实现
class GameCharacter
{
public:
    // 规定 int (const GameCharacter&) 类型的函数指针为 HealthCalcFunc
	typedef int (* HealthCalcFunc) (const GameCharacter&);
    // 构造函数接收一个 HealthCalcFunc 函数指针，如果不提供，默认为 defaultHealthCalc
    explicit GameCharacter(HealthCalFunc hcf = &defaultHealthCalc)
        : healthFunc(hcf)
    {}
    // 计算健康程度
    int healthValue() const
    { return healthFunc(*this); }
    ...
private:
	HealthCalcFunc healthFunc;
};
````

​	同一人物类型应该有不同方式的健康计算方式：

````c++
class EvilBadGuy: public GameCharacter
{
public:
    explicit EvilBadGuy(HealthCalFunc hcf = &defaultHealthCalc)
        : GameCharacter(hcf)
    {...}
    ...
};

// 健康指数计算函数1
int loseHealthQuickly(const GameCharacter&);
// 健康指数计算函数2
int loseHealthSlowly(const GameCharacter&);

// 相同类型的人物搭配
EvilBadGuy ebg1(loseHealthQuickly);
// 不同的健康计算方式
EvilBadGuy ebg2(loseHealthSlowly);
````

​	而当人物的健康计算函数可在运算期变更，我们只需要替换 healthFunc 指向的函数即可；

​	这带来的问题是：由于计算健康程度的函数并不属于 GameCharacter 继承体系内的成员函数，这意味这其无法使用 GameCharacter 的 non-public 成分；这就需要声明那个 non-member 函数为 friends，或者为其实现的某一部分提供 public 访问函数；



## 藉由function完成Strategy模式

​	这与上一个方法类似，只是使用 function class 代替 函数指针：

````c++
// 前置声明
class GameCharacter;					
// 默认计算健康程度函数，接收 一个GameCharacter引用
int defaultHealthCalc(const GameCharacter& gc);

// GameCharacter 类的实现
class GameCharacter
{
public:
    // 规定 int (const GameCharacter&) 类型的函数指针为 HealthCalcFunc
	typedef std::function<int (const GameCharacter&)> HealthCalcFunc;
    // using HealthCalcFunc = std::function<int (const GemeCharacter&)>;
    // 构造函数接收一个 HealthCalcFunc 函数指针，如果不提供，默认为 defaultHealthCalc
    explicit GameCharacter(HealthCalFunc hcf = &defaultHealthCalc)
        : healthFunc(hcf)
    {}
    // 计算健康程度
    int healthValue() const
    { return healthFunc(*this); }
    ...
private:
	HealthCalcFunc healthFunc;
};
````

​	使用 std::function 不仅可以接收函数指针，还可以使用函数对象与成员函数：

````c++
// 健康计算函数，返回的并非 int
short calcHealth(const GameCharacter&);		

// 为计算健康而设计的函数对象
struct HealthCalculator
{
    int operator() (const GameCharacter&) const
    {...}
};

// 实现计算健康程度函数
class GameLevel
{
public:
    float health(const GameCharacter&) const;
    ...
};

// derived classes
class EvilBadGuy: public GameCharacter
{
    ...
};

// 另一个人物类型
class EyeCandyCharacter: public GameCharacter
{
    ...
};

EvilBadGuy ebg1(calcHealth);

EyeCandyCharacter ecc1(HealthCalculator());

GameLevel currentLevel;
...
EvilBadGuy ebg2(
					)
````



## 古典的 Strategy 模式

​	使用古典的 Strategy 模式将计算健康函数做成一个分离的继承体系中的 virtual 成员函数：

````c++
// 计算健康程度 继承体系
class GameCharacter;

// 健康计算基类
class HealthCalcFunc
{
public:
    ...
	virtual int calc(const GameCharacter& gc) const;
    ...
};

// 声明一个全局的健康计算基类
HealthCalcFunc defaultHealthCalc;

// GameCharacter 基类
class GameCharacter
{
public:
	explicit GameCharacter(HealthCalcFunc* phcf = &defaultCalc)
       : pHealthCalc(phcf)
    {}
    int healthValue() const
    { return pHealthCalc->calc(*this); }
    ...
private:
	HealthCalcFunc* pHealthCalc;
};
````

​	如果还需要加入一个新的健康计算方法，只需要增加 HealthCalcFunc 的子类即可；



​	本条款的根本忠告是：当你为解决问题而寻找某个设计时，不妨考虑 virtual 函数的替代方案。下面快速复习我们验证过的几个替代方式：

- 使用 NVI 方式；
- 将 virtual 函数替换为 "函数指针成员变量"；
- 以 function 成员替代 virtual 函数；
- 将继承体系内的 virtual 函数替换为另一个继承体系内的 virtual 函数；



## 请记住

- virtual 函数的替代方案包括 NVI 手法及 Strategy 设计模式的多种模式。NVI 手法自身是一个特殊形式的 Template Method 设计模式；
- 将机能从成员函数转到 class 外部函数，带来一个缺点：非成员函数无法访问 class 的 non-public 成员；
- function 对象的行为就像一般的函数指针。这样的对象可以接纳 “与给定之目标签名式兼容”的所有可调用物；
