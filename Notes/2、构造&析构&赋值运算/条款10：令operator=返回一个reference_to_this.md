# 条款10：令operator=返回一个reference to *this

​		Have assignment operators return a reference to *this

赋值是可以连锁形式的：

````c++
int x, y, z;
x = y = z = 10;
````

此连锁形式被解析为：

````c++
x = (y = (z = 15));
````

所以对我们自己实现的类，也应该有这个特征，这需要让 `operator = ` 返回应该 `reference to *this`

````c++
class Widget{
public:
	...
	Widget& operator = (const Widget& rhs)	
	{
		...
		return *this;
	}
};
````

这个协议也使用与 `+=`、`-=` 等：

````c++
class Widget{
public:
	...
	Widget& operator += (const Widget& rhs)
	{
		...
		retuen *this;
	}
};
````

---





## 请记住

- 令赋值（`assignment`）操作符返回应该 `reference to *this`；