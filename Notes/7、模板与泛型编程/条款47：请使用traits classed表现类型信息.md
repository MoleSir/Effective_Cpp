# 条款 47：请使用 traits classes 表现类型信息

​		Use traits classes for information about types

STL 中的迭代器有五种类型：

- input迭代器：只能向前移动，一次一步，客用只能读取（不能修改）它们指向的对象，并且只能读取一次；C++ 中的 `istream_iterators` 就是这一分类的代表；
- output迭代器：只能向前移动，一次一步，客户可以涂写其指向的东西，但只能涂写一次，`ostream_iterator` 是这一分类的代表；
- forward迭代器：单向移动，但可以对某个对象读写多次；
- bidirectional迭代器：可以向前移动，也可以向后移动，对某个对象读写多次，比如 list 的迭代器；
- random access 迭代器：本质就是指针，除了以上的所有功能，还可以做指针算术；

在 STL 中这五种迭代器被定义为五个结构体，并且存在继承关系：

````c++
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag: public input_iterator_tag{};
struct bidirectional_iterator_tag: public forward_iterator_tag;
struct random_access_iterator_tag: public bidirectional_iterator_tag;
````

---

如果我们希望设计一个迭代器移动函数：

````c++
template <typename IterT, typename DistT>
void advance(IterT iter, DIstT distance); // distance < 0 表示后退
````

当然可以对所有迭代器做自加，但是对 random access 类型的迭代器，完全可以使用使用 += 实现；

所以更好的设计是对不同的类型的迭代器设计不同的实现；

而使用 traits classes 就可以帮助我们区分传入的迭代器是什么类型；

首先，我们需要在各个容器的迭代器中保存其类型，方法就是定义一个 `iterator_category` 的 `typedef` 或 `using `：

````c++
class list_iterator
{
public:
	...
	using iterator_category = bidirectional_iterator_tag;
};
...
````

而 traits calssed 做的，就是把其模板的 `iterator_category` "问" 出来：

````c++
template <typename IterType>
class iterator_trait
{
public:
    using iterator_category = typename IterType::iterator_category;
};
````

需要注意，`random_access` 是指针，不是类，所以对这个情况我们需要实现一个偏特化版本：

````c++
template <typename IterType>
class iterator_trait<IterType*>
{
public:
	using iterator_category = random_access_tag;
};
````

---

现在已经可以知道传入迭代器的类型了，那么如何调用不同版本的实现，使用 `if else` 吗，可以使用函数重载，先给每个类型的迭代器给出实现：

```c++
template <typename IterT, typename DistT>
void doAdvance(IterT iter, DIstT distance, 
               std::random_access_iterator_tag)  // random access 类型的实现
{
    iter += distance;
}
template <typename IterT, typename DistT>
void doAdvance(IterT iter, DIstT distance, 
               std::bidirectional_iterator_tag)  // bidirectional_iterator 类型的实现
{
    if (distance > 0) { while(distance--) ++iter; }
    else { while(distance--)  --iter; }
}
void doAdvance(IterT iter, DIstT distance, 
               std::input_iterator_tag)  // input_iterator 类型的实现
{
    if (distance < 0)
    {
        thorw std::out_of_range("Nefative");
	}
    while (distance--) iterator++; 
}
...
```

---

最后在 `advance` 内部利用函数重载调用到不同的实现即可：

````c++
template <typename IterType, typename DistType>
void Advance(IterType iter, DistType distance)
{
    using iterator_category = typename iterator_trait<IterType>::iterator_category;
    DoAdvance(iter, distance, iterator_category());
}
````

---



## 请记住：

- traits_classes 使得类型相关信息在编译期间可用，它们以模板和模板特化实现；
- 整合重载技术之后，traits 有可能在编译器对类型执行 if else；