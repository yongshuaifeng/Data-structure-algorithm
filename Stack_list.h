#ifndef STACK_LIST_H
#define STACK_LIST_H

#include "List.h"

template<typename T> class Stack:public List<T>{
public:
    void push(T const& e){insertAsLast(e);}
    T pop() {return remove(this->last());}
    T& top(){return this->last()->data;}
};


#endif//STACK_LIST_H