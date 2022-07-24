# 条款02：尽量以const，enum，inline替换#define

​		Prefer consts, enums, and inlines to #defines.

​	这个条款可以描述为：宁以编译器代替预处理器，如果我们使用 `#define` 来定义一个常量：

````c++
#define ASPECT_RATIO 1.653
````

​	那么这个记号 `ASPECT_RATIO` 被预处理器替换，编译器不会看到这个名称，那么其不会进入记号表（symbol table）。所以如果这个数据有问题，你得到的报错信息也不会包含  `ASPECT_RATIO` ，使得寻找问题变得困难。并且对浮点常量的预处理，可能使得 1。653 多出一份。

​	解决方法是使用常量替换宏定义：

```c++
const double AspectRatio = 1.653;
```

---



​	以常量代替 `#define`，有两种特殊情况值得说说。第一是定义常量指针（constant pointers）。由于常量定义通常放在头文件中，因此有必要将指针（而不是指针所指向的对象）声明为 `const`。例如在头文件中定义一个常量的 `char*` 字符串：

````c++
const char* const authorName = "Ye Chen";
````

或者使用 `std::sting`：

````c++
const std::sting authorName("Ye Chen");
````

​	第二个值得注意的是 class 专属常量。为了将常量的作用域限制于 class 内部，必须让其成为 class 的一个成员；并且保证只有一份，可以使用 `static`：

````c++
class GamePlayer{
private:
    // 次为声明式
	static const int NumTurns = 5;
	int scores[NumTurns];
	...
};
````

​	有些编译器可能还要求给出这个变量的定义式，那么就在一个实现文件中：

````c++
// 初值已经有了，不需要再赋
const int GamePlayer::NumTurns;
````

​	旧式的编译器不支持在类内给初值，那么只能：

```c++
class CostEstimate{
privat:
	static const double FudgeFactor;
	...
};

const double CostEstimate::FudgeFactoy = 1.35;
```

​	但这样我们就无法使用这个常量创建数组，那么此时就可以使用枚举：

````c++
class GamePlayer{
private:
	enum { NumTurns = 5 };
	int scores[NumTurns];
};
````

---



​	在来看几个使用 `#define` 的弊端，使用宏作函数：

````c++
// 以a和b的较大值为参数调用函数 foo
#define CALL_WITH_MAX(a, b) foo((a) > (b) ? (a) : (b))
````

​	那么如果出现以下的情况：

````c++
int a = 5, b = 0;
CALL_WITH_MAX(++a, b);			// a 被累加了两次
CALL_WITH_MAX(++a, b + 10);		// a 被累加了一次
````

​	这导致 `++a` 被执行的次数是不确定的；

​	那么就可以使用模板来解决：

````c++
template<typename T>
inline void callWithMax(const T& a, const T& b)
{
	foo(a > b ? a : b);
}
````

​	



## 请记住：

- 对于单纯常量，最好以 `const` 对象或 `enum` 替换 `#define`
- 对于形似函数的宏，最好使用 `inline` 代替 `#define`