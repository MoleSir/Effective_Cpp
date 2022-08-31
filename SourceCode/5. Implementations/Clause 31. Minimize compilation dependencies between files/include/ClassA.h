#ifndef __CLASSA_H__
#define __CLASSA_H__
#include <string>

// B 的声明式
class B;

// A 的定义
class A
{
public:
    A(int iA, float fA, const std::string& sA, B* pB);
    ~A();

    void Print() const;

private:
    int _iA;
    float _fA;
    std::string _sA;
    // 内包含 B 的指针
    B* _pB;
};



#endif