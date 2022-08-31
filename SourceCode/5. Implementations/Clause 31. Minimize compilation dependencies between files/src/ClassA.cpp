#include "ClassA.h"
#include <iostream>


A::A(int iA, float fA, const std::string& sA, B* pB) :
    _iA(iA), _fA(fA), _sA(sA), _pB(pB)
{}

A::~A()
{
}

void A::Print() const
{
    std::cout << 
    "IA:" << this->_iA <<
    "FA:" << this->_fA <<
    "SA:" << this->_sA << std::endl; 
}