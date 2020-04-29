#ifndef STACK_VECTOR_H
#define STACK_VECTOR_H

#include "Vector.h"

template<typename T>
class Stack:public Vector<T>
{
    public:
        void push(T const& e){insert(e);}//入栈
        T pop(){return remove(this->size()-1);}//出栈
        T& top(){return (*this)[this->size()-1];}//取顶
};


#endif//STACK_VECTOR_H