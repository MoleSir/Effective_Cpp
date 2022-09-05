# Effective C++
Notes and Test Code of Effective C++

## 让自己习惯 C++

### 条款 1

C++同时支持过程形式、面向对象、函数形式、泛型过程、元编程形式。C++ 高效编程守则视情况而定，取决于使用 C++ 的哪个部分；

### 条款 2

对单纯常量，最好以 `const` 对象或 `enum` 替换 `#define`，对于形似函数的宏，最好使用 `inline` 代替 `#define`；

### 条款 3

尽可能使用 `const`，可以帮助编译器侦察出错误；

`mutable` 修饰的成员变量可以在 `const` 成员函数中被修改；

### :star:条款 4

构造函数最好使用初始化列表，并且初始化列表的顺序最好与成员声明对象一致；



## 构造&析构&赋值运算

### :star:条款 5

编译器可以为 class 创建 `default` 构造函数、`copy` 构造函数、`copy assignment` 操作符与析构函数；

### :star:条款 6

为驳回编译器自动提供的机能，可将相应的成员函数声明为 `private`并且不予实现。使用像 `Uncopyable` 这样的 `base class` 也是一种做法；

C++11 后可以使用 `= delete` 关键字；

### :star:条款 7

带多态性质的 base classes 应该声明一个 `virtual` 析构函数；

Classes 的设计目的如果不是作为 base classes 使用，不应该带 `virtual` 析构

### 条款 8

析构函数不要吐出异常。

如果一个被析构函数调用的函数可能抛出异常，析构函数应该捕获任何异常，然后吞下它们（不传播）或结束程序；

### 条款 9

在构造函数和析构期间不要调用 `virtual` 函数，因为此时子类还没构造；

### :star:条款 10

令赋值操作符返回应该 `reference to *this`；

### :star:条款 11

确保对象自我赋值时 `operator=` 有正确行为；

其中技术包括比较 “来源对象” 和 “目标对象” 的地址、精心周到的语句顺序、以及 `copy-and-swap`；

### :star:条款 12

拷贝函数中不要遗漏赋值基类的成员；



## 资源管理

### 条款 13

为防止资源泄露，使用 RAII 对象，它们在构造函数中获得资源并在析构函数中释放资源；

### 条款 14

复制 RAII 对象必须一并复制其管理的资源，所以资源的 `copying` 行为决定 RAII 对象的 `copying` 行为；

可以：抑制 `copying` 、使用引用计数、深拷贝、转移资源；

### 条款 15

有些操作需要访问原始数据，所以 RAII class 需要提供获取原始数据的方法；

对原始资源的访问可以由两种：

- 显示转换：提供 `get()` 返回指针；
- 隐式转换：重载 `类名()` 操作符；

一般前者更安全，但是后者对于客户更方便；

### :star:条款 16

在 `new` 中使用 `[]`，在 `delete` 中也要使用 `delete []`；

如果没有在 `new` 中使用 `[]`，一定不要使用 `delete[]`；

### 条款 17

以独立语句将 newed 对象保存到智能指针中。如果不这样，一旦异常抛出，可能发生很难察觉的内存泄露；

C++ 编译器不保证一句指向内部的顺序；



## 设计与声明

### 条款 18

好的接口容易被使用，不容易被误用、保证接口一致性、设计类型与内置类型行为一致、建立新类型、限制类型的操作等；

### :star:条款 19

设计 class 犹如设计 type；

### :star:条款 20

用 pass-by-reference-to-const 替换 pass-by-value。前者通常比较高效，并可避免切割问题；

对内置类型，以及 STL 的迭代器和函数对象使用 pass-by-value；

### :star:条款 21

不要返回 pointer 或 reference 指向一个 local stack 对象；

### :star:条款 22

将成员变量声明为 `private`；

### 条款 23

用 `non-member` 、`non-friend` 函数替换 `member` 函数。这样做可以增加封装性、包裹弹性和机能拓展性；

### 条款 24

如果需要为某个函数的所有参数（包括被 this 指针所指的能够隐喻参数）进行类型转换，使用一个 `non-member` 函数；

`member` 函数不允许 this 进行隐式类型转换；

### 条款 25

当 `std::swap` 对自定义类型效率不高时，提供一个 `swap` 成员函数，并且确定其不抛出异常；



## 实现

### 条款 26

尽可能延后变量定义式的出现。这样做可增加程序的清晰度并完善程序效率；

### :star:条款 27

如果可以，尽量避免转型，特别是在注重效率的代码中避免；

C++ 提供四种 `cast`，若干需要转型，使用它们而不是旧式转型；

### :star:条款 28

避免返回 handles（包括 reference、指针、迭代器）指向对象内部；

### :interrobang:条款 29

- 异常安全函数，即使发生异常也不会泄露资源或允许任何数据结构败坏；这样的函数区分为三种可能的保证：基本型、强烈型、不抛出异常型；
- ”强烈保证“往往以 copy-and-swap 实现，但”强烈保证“并非对所有的函数都可实现或具备实现意义；
- 函数提供的”异常安全保证“通常最高只等于其调用之各个函数的”异常安全保证“中的最弱者；

### :interrobang:条款 30

- 将大多数 `inlining` 限制在小型、被频繁使用的函数上，这可使日后调用调试过程和二进制升级更容易秒也可以使得潜在的代码膨胀问题最小化，使得程序的速度提升最大化；
- 不要只因为 `function template` 出现在头文件，就将它们声明为 `inline`；

### :star:条款 31

将文件的间的编译依赖降至最低；

C++ 的两个 classes 直接不可能互相包含对方成员，无法计算大小，违背常理，只能互相包含指针或者引用；



## 继承与面向对象设计

### 条款 32

`public` 继承意味着 is-a。使用在 base classes 身上的事情一定适用在 derived 身上；

### :star:条款 33

如果父类的函数没有加上 `virtual`，那么只要子类有相同名称的函数，父类的函数就会被子类掩盖（不论参数是否相同）；

如果想要使用父类的同名函数，使用 `using::` 调用；

### 条款 34

区别接口继承和实现继承；接口继承不会继承父类的实现；

### 条款 35

- `virtual` 函数的替代方案包括 NVI 手法及 Strategy 设计模式的多种模式。NVI 手法自身是一个特殊形式的 Template Method 设计模式；
- 将机能从成员函数转到 class 外部函数，带来一个缺点：非成员函数无法访问 class 的 `non-public` 成员；
- function 对象的行为就像一般的函数指针。这样的对象可以接纳 “与给定之目标签名式兼容”的所有可调用物；

### 条款 36

绝不重新定义继承而来的 `non-virtual` 函数

### 条款 37

绝不重新定义继承而来的缺省参数值；因为缺省参数值都是静态绑定，而 virtual 函数却是动态绑定；

### 条款 38

- 复合与 `public` 继承意义完全不同；
- 复合-> has-a，`public` 继承-> is-a

### 条款 39

- `private` 继承意味着 is-implemented-in-terms of ，与复合关系类型，但一般不适应 `private` 继承
- 但是 `private` 继承可以造成 empty base 最优化。对致力于“对象尺寸最小化”的程序库开发者来说很重要；

### 条款 40

没事别用多继承；



## 模板与泛型编程

### 条款 41

- classes 和 `template` 都支持接口和多态；
- 对 classes 而言接口都是显示的，以函数签名为中心，多态则是通过 `virtual` 函数发生在运行期；
- 对 `template` 而言接口是隐式的，奠基于有效表达式，多态则是通过 `template` 具现化和函数重载解析发生在编译期；

### :star:条款 42

- 声明 `template` 参数时，前缀 class 与 `typename` 等效；

- 请使用 `typename` 标识嵌套从属类型名称，这很重要，因为编译器不知道模板中的成员是不是类型，这时需要手动告诉编译器；

    ```c++
    typename C::const_iterator iter(container.begin());
    ```

- 但不得在 base class list 或 member initialization list 中以 `typename` 作为 base class 的修饰符，就是编译器肯定知道是类型的地方别用；

### :star:条款 43

在 derived class templates 内通过 `this->` 指涉 base class templates 内的成员名称；使用 `using` 表达式也可以；

否则编译器找不到；

### 条款 44

- 因非类型模板参数而造成的代码膨胀，往往可以消除，做法是以函数参数或 class 成员变量替换 `template` 参数；
- 因类型参数而造成的代码膨胀，往往可以降低，做法是让有完全相同的二进制表达式的具体类型共享实现代码；

### 条款 45

- 使用成员函数模板生成可接受所有兼容类型的函数；
- 如果声明的成员函数模板用于泛化 `copy` 构造，还是需要声明正常的 `copy` 构造；

成员函数模板可以使得指针指针适用于继承体系的多态实现；

````c++
template <typename T>
class SmartPtr
{
public:
	template <typename U>
    SmartPtr(const SmartPtr<U>& other);
    ...
}
````

### :interrobang:条款 46

需要类型转换时请为模板定义非成员函数

编写一个 `class template`，而它所提供之 “于此 `template` 相关的” 函数支持 “所有参数之隐式类型转换” 时，请将那些函数定义为 `class template` 内部的友元函数；

### :star:条款 47

- `traits_classes` 使得类型相关信息在编译期间可用，它们以模板和模板特化实现；
- 整合重载技术之后，traits 有可能在编译器对类型执行 `if else`；

对迭代器类型的萃取就十分常用；

### 条款 48

模板元编程可以将工作从运行期移往编译器，因而得以实现早期错误侦察和更高效的执行效率；



## 定制 new 和 delete

### 条款49

- `set_new_handler` 允许我们指定一个函数，在内存分配无法获得满足时被调用；

- 现代 C++ 默认的 `new` 会在发生失败抛出异常 `std::bad_alloc`，而不是返回 `NULL`  指针；

- 也可以使用 `Nothrow new` ，使得 `new` 在内存不足时不抛出异常，而是返回 `NULL`，但其不能保证构造函数不抛出异常

    ````c++
    Widget* pw1 = new (std::nothrow) Widget();
    ````

    本质上，这个 `new` 调用了一个 `placement new`，`()` 内部传入的是第二个参数；

### 条款50

- 有许多理由需要些个自定义的 new 和 delete，包括改善效能、对 heap 运用错误进行调试、收集 heap 适用信息；

### 条款51

- `operator new` 应该包含一个无穷循环，并在其中尝试分配内存，如果无法满足内存需求，就该调用 `new-handler`。它还应该有能力处理 0 字节申请。Class 专属版本则还应该处理 “比正确大小更大的错误申请”；
- `operator delete` 应该在收到 null 指针是不做任何事情。Class 专属版本应该处理 “比正确大小更大的（错误）申请”；

### :star:条款52

- `placement new` 是 `operator new` 对正常 `operator new` 的重载，其具有除了 `size` 外的多个参数；

- 调用 `placement new` 的方式是在 `new` 后加括号填入实参：

    ````c++
    Widget* pw = new (std::cout) Widget();

- 当写一个 `placement new`，需要保证给出对应的 `placement operator delete`。如果没有这样做，程序可能会发生隐微而时断时续的内存泄露；

-  `placement delete` 只能在 `placement new` 调用成功，但构造函数抛出异常时，由 C++ 自动调用；

- 当声明 `placement new` 和 `placement delete` ，不要无意识得掩盖了它们的正常版本；  
