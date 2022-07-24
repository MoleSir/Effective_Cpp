# 条款17：以独立语句将newed对象置入智能指针

​		Store newed objects in smart pointers in standalone statements

​	假如我们有个函数用来揭示处理程序的优先级，另一个函数用来在某个动态分配所得的 Widget 上进行某些带有优先权的处理：

````cpp
int priority();
void processWidget(std::trl::shared_ptr<Widget> pw, int priority);
````

​	现在来调用这个函数：

````cpp
processWidget(std::trl::shared_ptr<Widget>(new Widget),priority());
````

​	令人惊讶的是，虽然我们使用“对象管理式资源”，上述调用却可能导致内存泄露；稍后再解释；



​	编译器在产出一个 processWidget 调用码之前，需要先核算各个实参。上述第二个参数只是对 priority() 函数的调用得到一个 int，但是第一个参数由两个部分组成：

- 执行“new Widget”表达式；
- 调用 trl::shared_ptr 构造函数；

于是调用 processWidget 之前，编译器需要做三件事：

- 调用 priority()；
- 执行“new Widget”表达式；
- 调用 trl::shared_ptr 构造函数；

C++ 编译器以什么样的顺序做这三件事是不确定的，只能说 new Widget 一定发生在 trl::shared_ptr 之前；

那么如果编译器根据各方面考虑，最后得到了这样的顺序：

​	1、执行“new Widget”表达式；

​	2、调用 process()；

​	3、调用 trl::shared_ptr 构造函数；

​	那么如果在2中出现了异常，导致 new 出来的 Widget 对象没有放入指针指针，而也没有被释放，导致内存泄漏的发生；

​	为了避免这个现象很简单，只要分量语句，先创建好智能指针对象：

````cpp
std::trl::shared_ptr<Widget> pw(new Widget);

processWidget(pw, priority());
````

​	

## 请记住

- 以独立语句将 newed 对象保存到智能指针中。如果不这样，一旦异常抛出，可能发生很难察觉的内存泄露；