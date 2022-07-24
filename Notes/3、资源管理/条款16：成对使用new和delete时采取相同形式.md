# 条款16：成对使用new与delete采用相同形式

​		Use the same form in corresponding uses of new and delete

​	以下动作有什么错误

````cpp
std::string* stringArray = new std::string[100];
...
delete stringArray;
````

​	stringArray可以作为一个数组的地址，当然也可以仅仅作为一个string指针，那么其他99个string对象的析构可能不会被调用；



​	当使用new时，有两件事情发生：

- 内存被分配（通过名为 operator new 的函数，见条款49与51）;
- 针对这个内存会有一个或多个构造函数被调用；

​	当使用delete时，也有两件事发生：

- 针对此内存会有一个或多个析构函数被调用；
- 内存被释放（通过名为 operator delete的函数）；

​	

​	所以我们在调用 delete 时，我们会希望知道这个指针是数组地址还是单一对象？如果是数组那么其有多少个对象？应该调用多少次析构？

​	通常，单一数据内存布局与数组不相同，数组的头部可能还会记录这元素个数，当然具体实现是看编译器的；

​	所以当我们需要调用 delete，惟一让编译器知道传入的是数组还是单一对象的方法就是——我们直接告诉编译器；使用 delete [] 便可以指定是数组对象：

````cpp
std::string* stringPtr1 = new std::string;
std::string* stringPtr2 = new std::string[100];
...
delete stringPtr1;
delete [] stringPtr2;
````

​	

​	所以规则就是：使用new时使用了[]，就用delete[]；只使用new那么只要delete;



​	这个规则对喜欢 typedef 的人很重要，比如：

````cpp
typedef std::string AddressLines[4];			// 每个人的地址是四个string组
												// AddressLines == std::string  [4]

std::string* pal = new AddressLines;			// 虽然看起来只使用new，但本质上是创建了数组
````

​	那么我们就必须使用数组形式的delete：

````cpp
delete [] pal;
````

​	所以尽量避免对数组形式做 typedef，如果有需要使用容器会更好-->` vector<string>`；



## 请记住

- 在 new 中使用 []，在 delete 中也要使用 delete []；
- 如果没有在 new 中使用 []，一定不要使用 delete[]；