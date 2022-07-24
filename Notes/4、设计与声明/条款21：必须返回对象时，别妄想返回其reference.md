# 条款21：必须返回对象时，别妄想返回其reference

​		Don't try to return a reference when you must return object

​	从条款 20 我们知道，pass-by-reference 是高效的，但是传递一些 reference 指向其实不存在的对象，这并不是一件好事：

​	考虑一个用来表现有理数（rational numbers）的class，内含有一个函数用来计算两个有理数的乘积

````c++
class Rational
{
public:
	Rational(int numberator = 0, int denominator = 1);
    ...
private:
	int n, d;
friend 
    const Rational operator* (const Rational& lhs, const Rational& rhs);
};
````

​	这个版本的 operator* 返回了一个 const Rational，这可能导致对象构造与析构成本，因为在函数中需要创建一个Rational 对象，而返回后赋值给另一个 Rational 又将调用一次构造；

​	

​	传递 reference 可以解决构造问题，但是引用只是名称，如果一个对象即将销毁，再返回其 reference 是危险的；比如：

````c++
const Rational& operator* (const Rational& lhs, const Rational& rhs)
{
    Rational result(lhs.n * rhs.n, lhs.d * rhs.d);
    return result;
}
````

​	返回一个即将销毁对象的引用是糟糕的；

​	

​	或者考虑在 heap 上创建对象，返回引用这样其就不会被销毁

````c++
const Rational& operator* (const Rational& lhs, const Rational& rhs)
{
    Rational* result = new Rational(lhs.n * rhs.n, lhs.d * rhs.d);
    return *result;
}
````

​	现在的问题是，由谁对这个对象进行 delete？

​	即使调用者诚实谨慎，能够及时调用 delete，但下面这种情况可能很难阻止内存泄漏的发生：

````cpp
Rational w, x, y, z;
w = x * y * z;
````

​	一句话调用了两次 operator* ，也就是有两个 heap 对象创建，但我们没有办法让调用者能够 delete 掉 x*y 产生的堆区对象；

​	

​	综上，一个“必须返回新对象”的函数的正确写法是：就让函数返回一个新对象：

```cpp
const Rational operator* (const Rational& lhs, const Rational& rhs)
{
    return Rational(lhs.n * rhs.n, lhs.d * rhs.d);
}
```

​	当然这需要多次的构造与析构，但为了安全这是没办法的；

​	但其实编译器可能会在不改变可观测行为情况下，改善我们的代码，也许程序执行起来会比我们预想得快；



## 请记住：

- 绝不要返回 pointer 或 reference 指向一个 local stack 对象，或返回 reference 指向一个 heap-allocated 对象，或返回 pointer 或 reference 指向一个 local static 对象；





## 补充：

​	C++11 引入了右值引用，

````cpp
    Rational&& operator* (const Rational& lhs, const Rational& rhs)
    {
        Rational result(lhs.n * rhs.n, lhs.d * rhs.d);
        return std::move(result);
    }
````



​	
