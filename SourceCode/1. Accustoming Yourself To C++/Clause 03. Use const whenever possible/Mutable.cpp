/* test of mutable */
#include <iostream>
#include <string>

class Person
{
public:
    Person(int age, const std::string& name): 
        _age(age), _name(name) 
    {}
    void NoChangeName() const
    {
        this->_age = 18;
    }
private:
    mutable int _age;
    std::string _name;
};


int main()
{   
    Person Yc(20, "YeChen");
    Yc.NoChangeName();
    return 0;
}