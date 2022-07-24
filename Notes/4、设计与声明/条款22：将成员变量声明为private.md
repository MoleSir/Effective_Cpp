# 条款22：将成员变量声明为private

​		Declare data members private

​	为什么成员变量需要是 private？

​	**==先从语法一致性开始==**：

​	如果所有成功变量都是 private，那么用户也只能通过成员函数使用它们，这样用户就只需要使用一种擦操作，那一定是函数，不需要去记什么是成员变量，不加()，什么是函数需要加()；

​	并且使用函数操作成员变量可以对你对成员变量的处理有更精确的控制，可以实现出“不准访问”、“只读访问”、“只写访问”、“读写访问”；

````c++
class AccessLevels
{
public:
    ...
    int getReadOnly() const { return readOnly; }
    void setReadOnly(int value) { readWrite = value; }
    int getreadWrite() const {return readWrite; }
    void setWriteOnly(int value) { writeOnly = value; }
private:
    int noAccess;				// 不准访问
    int readOnly;				// 只读访问
    int readWrite;				// 读写访问
    int writeOnly;				// 只写访问
};
````



​	**==依次是封装：==**

​	如果通过函数访问成员变量，今后改以某个计算替换这个变量，而 class 客户一点也不会知道 class 的内部已经发生变化：

​	举个例子，编写自动测速程序，当汽车通过，其速度被计算并且填入速度收集器

````c++
class SpeedDataCollection
{
	...
public:
    void addValue(int speed);			// 添加一笔新数据
    double averageSoFar() const;			// 返回平均速度
};
````

​	对于 averageSoFar()，有这样两种实现：

- 在class 内部设计一个变量保存至今为止所有的平均速度，当函数被调用，直接返回即可；
- 每次调用函数时，重新计算平均值；

​	两种方法一个省时间，一个省空间，对不同的系统可以采用不同的方法；

​	但对用户来说，只是调用一个 average 函数而已，因为我们使用函数对实现进行了封装；



​	并且当一个 public 对象被修改，那么外部代码就需要做出大量修改；



## 请记住：

- 切记将成员变量声明为 private。这可以赋予用户访问数据的一致性、可细微划分访问控制、允许约束条件得到保证、并提供 class 作者以充分的实现弹性。
- protected 并不比 public 更具有封装性；