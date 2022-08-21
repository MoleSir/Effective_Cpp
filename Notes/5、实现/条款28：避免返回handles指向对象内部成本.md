# c条款28：避免返回handles指向对象内部成分

​		Avoid returning "handles" to object internals

​	假设一个呈现涉及矩形，每个矩形由其左上角与右下角表示。为了使得一个 Rectangle 尽量小，使用一个辅助的结构体：

````c++
class Point	// 使用这个 class 表示点
{
public:
    Point(int x, int y);
    ...
    void setX(int newVal);
    void setY(int newVal);
    ...
};
````

​	使用这个点结构体表示一个矩形

````c++
struct RectData
{
	Point ulhc;			// ulhc = "upper left-hand corner"
    Point lrhc;			// lrhc = "lower right-hand corner"
};

class Rectangle
{
 public:
    ...
 private:
    std::trl::shared_ptr<RectData> pData;		
};
````

​	Rectangle 的客户必须能够计算其范围，所以这个 class 提供 upperLeft 函数与 lowerRight 函数，返回 const reference 

````c++
class Rectangle
{
public:
    ...
    const Point& upperLeft() const { return pData->ulhc; }
    const Point& lowerRight() const { return pData->lrhc; }
    ...
};
````

​	这当然可以完成任务，但其实这两个函数返回了对象内部的 handles（指针、引用），可能会在其他场合带来问题。更明确地说：可能导致 dangling handles（空悬的号码牌）：

​	考虑到某个函数返回 GUI 对象的外框，这个外框采用矩形形式：

````c++
class GUIObject {...};								
const Rectangle boundlingBox(const GUIObject& obj);	// 以by value 方式返回一个矩形
````

​	现在，客户有可能这样使用：

````c++
GUIObject* pgo;					// 以by value方式返回一个矩形
...
const Point* pUpperLeft = &( boundlingBox(*pgo).upperLeft() ); // 获取一个外壳坐上节点
````

​	对 boundlingBox 的调用获得一个新的、暂时的 Rectangle 对象，并且将这个临时对象的左上顶点的Point 对象的引用返回；

​	但是这个语句结束后，这个临时变量就释放，那么 pUpperLeft 指向的对象就会悬空；

​	

​	这就是为什么函数返回 handle 代表对象内部成分“总是危险的原因”。



## 请记住

- 避免返回 handles（包括 reference、指针、迭代器）指向对象内部。遵守这个条款可以增加封装性，帮助 const 成员函数的行为像 const ，并发送“虚吊号码牌”的可能性将至最低；