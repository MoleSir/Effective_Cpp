# 条款11：在operator= 中处理“自我赋值”

​		Handle assignment to self in operator = 

​	“自我赋值” 发生在对象被赋值给自己时：

````c++
class Widget{...};
Widget w;
...
w = w;
````

​	这是合法的，并且有时不容易发现：

```c++
a[i] = a[j];
*px *py
```

​	这两种写法可能导致不经意的自我赋值；

​	所以我们需要在赋值运算符中处理，特别是一个管理资源的类：

````c++
class Bitmap {...};
class Widget {
	...
private:
    Bitmap* pb;
};
````

​	如果不考虑自我赋值，可能会这样写：

````c++
Widget& Widget::operator = (const Widget& rhs)
{
    delete this->pb;
    this->pb = new Bitmap(*rhs.pb);
    return *this;
}
````

​	如果出现自我赋值，我们先将 `pb` 释放，再用 `pb` 去构建一个 `Bitmap` 显然是不行的，我们可以先进行判断：

````c++
Widget& Widget::operator = (const Widget& rhs)
{
    if (this == &rhs) return *this;
    delete this->pb;
    this->pb = new Bitmap(*rhs.pb);
    return *this;
}
````

​	这样可以，但不够安全，如果 `new` 失败，那么 `pb` 指向的是一个被释放的空间，这不够安全，可以这样写：

````c++
Widget& Widget::operator = (const Widget& rhs)
{
	Bitmap* pOrig = this->pb;
    this->pb = new Bitmap(*rhs.pb);
    delete pOrig;
    return *this;
}
````

​	我们先申请空间，就算申请失败，我们的原数据也没有被 `delete ` ，`pb` 可以正常使用，并且这样也可以处理自我赋值；





## 请记住：

- 确保当前对象自我赋值时 `operator=` 有良好行为。其中技术包括比较 “来源对象” 和 “目标对象” 的地址、精心周到的语句顺序、以及 `copy-and-swap`；
- 确定任何函数如果操作一个以上的对象，而其中多个对象是同一个对象时，行为仍然正确；