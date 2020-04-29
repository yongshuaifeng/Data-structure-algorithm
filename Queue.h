#ifndef QUEUE_H
#define QUEUE_H

#include "./List.h"


template<typename T> 
class Queue:public List<T>//队列模板类（继承List原有接口）
{
public:
    void enqueue(T const& e){insertAsLast(e);}
    T dequeue() {return remove(this->first());}
    T& front() {return this->first()->data;}
};
#endif//QUEUE_H