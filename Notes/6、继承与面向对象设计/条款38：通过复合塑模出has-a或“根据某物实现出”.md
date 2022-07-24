# 条款38：通过复合塑模出has-a或“根据某物实现出”

​		Model "has-a" or "is-implemented-in-terms-of" through composition

​	复合是类型之间的一种关系，当某类型的对象包含它类的对象，便是复合：

````c++
class Address {...};
class PhoneNumber {...};
class Person
{
public:
	...
private:
	std::string name;
    Address address;
    PhoneNumber voiceNumber;
    PhoneNumber faxNumber;
};
````

​	"public继承"表示 is-a，而复合意味 has-a；



## 请记住

- 复合与public 继承意义完全不同；
- 复合-> has-a，public继承-> is-a