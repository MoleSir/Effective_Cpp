#include "ClassB.h"
#include <iostream>


B::B(int iB, float fB, const std::string& sB, A* pA) :
    _iB(iB), _fB(fB), _sB(sB), _pA(pA)
{}

B::~B()
{
}

void B::Print() const
{
    std::cout << 
    "IB:" << this->_iB <<
    "FB:" << this->_fB <<
    "SB:" << this->_sB << std::endl;
}