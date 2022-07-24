# 条款03：尽可能使用const

​		Use const whenever possible

​	`const ` 有许多作用，可以在 classes 外部修饰 global 或 namespace 作用域的常量，或修饰文件、函数、或区块作用域（block scope）中被声明为 static 的对象。还可以修饰 classes 内部的 static 和 non-static  成员变量。面对指针，可以可以指定指针自身、指针所指之物，或两者都（或都不）是 const；

````c++
char greeting[] = "Hello";
char* p = greeting;					// non-const pointer, non-const data
const char* p = greeting;			// non-const pointer, const data
char* const p = greeting;			// const pointer, non-const data
const char* const p = greeting;		// const pointer, const data
````

​	规律就是：关键字 const 出现在星号左边，表示被指物是常量，就是指向的那个对象是不可以修改的；如果出现在星号右边，表示指针自身是常量，也就是说这个不能再修改指向；

​	所以 `const char * ` 与  `char const *` 其实是等价的；

​	**STL** 中的迭代器根据指针塑造，那么给一个迭代器加 const 意味着这个迭代器不得再指向别的东西，而 const_iterator 表示的是指向的对象是不可以修改的：

````c++
std::vector<int> vec;
...
const std::vector<int>::iterator iter = vec.begin();
*iter = 10;				// 没有问题
++iter;					// 错误
std::vector<int>::const_iterator cIter = vec.begin();
*cIter = 10;			// 错误
++cIter;				// 正确	
````

---



​	const 最重要的用法是对函数声明时的应用，在一个函数声明式中，const 可以和函数返回值、参数、函数本身产生关联；

​	有时令函数返回一个常数，可以降低因用户错误导致的意外。比如在一个计算两个复数之和的函数返回一个常量：

````c++
class Rational {...};
const Rational operator* (const Rational& lhs, const Rational& rhs);
````

​	避免了用户写出这样的代码：

````c++
Rational a, b, c;
...
(a * b) = c;
````

----



​	将 const 实施于成员函数的目的，是为了确认该成员函数可作用于 const 对象身上。这一类成员函数之所以重要，基于两个理由，第一，它们使 class 接口比较容易理解。这是因为，得知哪个函数可以改动对象内容而哪个函数不行，很是重要。第二，它们使操作 const 对象成为可能；

​	关于 const 有一点容易被忽视：两个成员函数如果只是常量性不同，可以被重载：

````c++
class TextBlock{
public:
	...
	const char& operator[] (std::size_t position) const 
	{ return text[position]; }
	char& operator[] (std::size_t position) 
	{ return text[position]; }
private:
	std::string text;
};
````

​	只要重载 `operator[]` 并对不同版本给予不同的返回类型，就可以令 const 和 non-const TextBlock 获得不同的处理：

```c++
TextBlock tb("Hello");
const TextBlock ctb("Workd");
std::cout << tb[0];
th[0] = 'x';
std::count << ctb[0];
ctb[0] = 'x';
```

​	注意 non-const 版本的返回值需要一个 char&，而不是 char，这样才能修改；

​	对于标识为` const`  的成员函数，如果令其返回一个引用可能会破坏我们期望其保持的不改变类对象的性质：

````c++
class CTextBlock{
public:
	...
	char& operator[] (std::size_t position) const
	{ return pText[position];  }
private:
	char* pText;
};
````

  	虽然在 `operator[]`  中，确实没有对对象的内容做出修改，但是传出的引用就可以；

---



​	可以使用 `mutable` 关键字使得成员变量可以在 `const` 函数中被修改！

````c++
class CTextBlock{
 public:
	...
    std::size_t length() const;
private:
	char* pText;
    // mutbale 修饰的成员变量可以在 const 函数中被改变！
    mutable std::size_t textLength;
    mutable bool lengthIsValid;
};

std::size_t CTextBlock::length() const
{
    if (! lengthIsValid){
		textLength = std::strlen(pText);
        lengthIsValid = ture;
    }
    return textLength;
}
````

---



​	如之前所作的，我们给 `TextBlock` 重载了两个 `operator[]`，可以看出其实里面的内容差别很小，只有在返回时一个带有 `const`，如果我们还给每个函数再多一些检查，那么代码的冗余度会很高，这不是我们想看到的。那么我们可以使用一个转型，只写出一个 `operator[]` ，另一个就使用转型操作，让其去掉 const 属性：

````c++
class TextBlock{
public:
	...
    const char& operator[] (std::size_t position) const
    {
        // 做很多事情 ...
        return text[position];
	}
    char& operator[] (std::size_t position)
    {
        return const_cast<char&>(static_cast<const TextBlock&>(*this)[position])
	}
};
````

 	第一个转型 `static_cast`，我们将一个 `this*` 由原来的 TextBlock 转型为 const TextBlock；第二个转型 `const_cast` 可以将返回值的 `const` 移去；





## 请记住：

- 某些东西声明为 `const` 可以帮助编译器侦察出错误。`const` 可以被施加于任何作用域内的对象、函数参数、函数返回值、函数本体；
- 编译器强制实施 bitwise constness，但你编写程序时应该使用 "概念上的常量性"；
- 当 `const` 和 `non-const` 成员函数有着实质等价的是现实时，令 `non-const` 版本调用 `const` 版本可以避免代码重复； 
