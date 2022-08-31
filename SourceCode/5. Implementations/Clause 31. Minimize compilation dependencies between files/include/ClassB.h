#ifndef __CLASSB_H__
#define __CLASSB_H__
#include <string>

// A 的声明式
class A;

// B 的定义
class B
{
public:
    B(int iB, float fB, const std::string& sB, A* pA);
    ~B();

    void Print() const;

private:
    int _iB;
    float _fB;
    std::string _sB;
    // 内包含 B 的指针
    A* _pA;
};

#endif