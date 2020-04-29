#ifndef LIST_H
#define LIST_H

#include"listNode.h"
#include"stdlib.h"
#include<iostream>

template<typename T> class List{//列表模板类
private:
    int _size;ListNodePosi(T) header;ListNodePosi(T) trailer;//规模、头哨兵、尾哨兵
protected:
    void init();//列表创建时地初始化
    int clear();//清楚所有节点
    T remove(ListNodePosi(T) );//删除节点
    void copyNodes(ListNodePosi(T),int);//复制列表中自位置p起地n项
    void merge(ListNodePosi(T)&,int,List<T>&,ListNodePosi(T),int);//归并
    void mergeSort(ListNodePosi(T)&,int);//对从p开始连续地n个节点归并排序
    void selectionSort(ListNodePosi(T),int);///对从p开始连续地n个节点选择排序
    ListNodePosi(T) selectMax(ListNodePosi(T),int);//从起始于位置p的n个元素中选出最大者
    void insertionSort(ListNodePosi(T),int);///对从p开始连续地n个节点插入排序

public:
//构造函数
    List() {init();}//默认
    List(List<T> const& L);//默认复制列表L
    List(List<T> const& L,Rank r,int n);//复制列表L中自第r项起的n项
    List(ListNodePosi(T) p,int n);//复制列表中自位置p起的n项
//析构函数
    ~List();//释放所有节点

//只读访问接口
    Rank size() const {return _size;}//规模
    bool empty() const {return _size <= 0;}//判空
    T& operator[] (Rank r)const;//重载，支持循秩访问(效率低)
    ListNodePosi(T) first() const {return header->succ;}//首节点位置
    ListNodePosi(T) last() const {return trailer->pred;}//末节点位置
    bool valid(ListNodePosi(T) p)//判断位置p是否对外合法
    {return p && (trailer != p) && (header != p);}//将头尾节点等同于NULL
    ListNodePosi(T) find(T const& e)const //无序列表查找
    {return find(e,_size,trailer);}
    ListNodePosi(T) find(T const& e,int n,ListNodePosi(T) p) const;//无序区间查找
    ListNodePosi(T) search(T const& e)const//有序列表查找
    {return search(e,_size,trailer);}
    ListNodePosi(T) search(T const& e,int n,ListNodePosi(T) p) const;//有序区间查找
    //ListNodePosi(T) selectMax(ListNodePosi(T) ,int );//在p及其n-1个后继中选出最大者
    ListNodePosi(T) selectMax() {return selectMax(header->succ,_size);}//整体最大者

//可写访问接口
    ListNodePosi(T) insertAsFirst(T const& e);//将e当作首节点插入
    ListNodePosi(T) insertAsLast(T const& e);//将e当作末节点插入
    ListNodePosi(T) insertA(ListNodePosi(T) p,T const& e);//将e当作p的后继节点插入
    ListNodePosi(T) insertB(ListNodePosi(T) p,T const& e);//将e当作p的前驱节点插入
    void sort(ListNodePosi(T) p,int n);//列表区间排序
    void sort(){sort(first(),_size);}//列表整体排序
    int deduplicate();//无序去重
    int uniquify();//有序去重
    void reverse();//前后倒置

//遍历
    void traverse(void (*) (T&));//遍历
    template<typename VST>//操作器
    void traverse(VST&);//遍历

};//List


template<typename T>
void List<T>::init()//列表创建时地初始化
{
    header = new ListNode<T>;//创建头哨兵节点
    trailer =  new ListNode<T>;//创建尾哨兵节点
    header->succ = trailer;header->pred  = nullptr;
    trailer->pred = header;trailer->succ = nullptr;
    _size = 0;//记录规模
}

template<typename T>
int List<T>::clear()//清除所有节点
{
    int oldSize = _size;
    while(0<_size) remove(header->succ);//反复删除首节点，直至列表变空
    return oldSize;
}

template<typename T>
T List<T>::remove(ListNodePosi(T) p)//删除节点
{
    T e = p->data;//备份待删除节点
    p->pred->succ = p->succ; p->succ->pred = p->pred;//后继、前驱
    delete p;_size--;
    return e;//返回备份的数值
}

template<typename T>
void List<T>::copyNodes(ListNodePosi(T) p,int n)//复制列表中自位置p起地n项
{
    init();//创建头、尾哨兵节点并初始化
    while(n--){insertAsLast(p->data);p=p->succ;}//依次插入

}

template<typename T>
void List<T>::merge(ListNodePosi(T)& p,int n,List<T>& L,ListNodePosi(T) q,int m)//归并
{
    ListNodePosi(T) pp = p->pred;//借助前驱
    while( 0<m )//在q尚未移除区间之前
        if( (0 < n ) && (p->data <= q->data))//若p仍在区间内且v(p) <= v(q),则
            { if(q == (p = p->succ)) break;n--;}//p归入合并的列表，并替换为其直接后继
        else
        {
            insertB(p,L.remove((q = q->succ)->pred));m--;//将q转移至p之前
        }
    p = pp->succ;//确定归并后区间的起点
}

template<typename T>
void List<T>::mergeSort(ListNodePosi(T)& p,int n)//对从p开始连续地n个节点归并排序
{
    if(n<2) return;
    int m = n>>1;//以中点为界
    ListNodePosi(T) q = p;for(int i =0;i<m;i++) q = q->succ;//均分列表
    mergeSort(p,m); mergeSort(q,n-m);//对前后子列表分别排序
    merge(p,m,*this,q,n-m);//归并

}

template<typename T>
void List<T>::selectionSort(ListNodePosi(T) p,int n)///对从p开始连续地n个节点选择排序
{
    ListNodePosi(T) head = p->pred;ListNodePosi(T) tail = p;
    for(int i = 0;i<n;i++) tail = tail->succ;//待排序区间
    while(1<n){
        ListNodePosi(T) max = selectMax(head->succ,n);
        insertB(tail,remove(max));
        tail = tail->pred;n--;
    }

}

template<typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p,int n)
{
    ListNodePosi(T) max  = p;
    for(ListNodePosi(T) cur = p;1<n;n--)
        if((cur=cur->succ)->data >= (max->data))
            max = cur;
    return max;//返回最大节点的位置
}


template<typename T>
void List<T>::insertionSort(ListNodePosi(T) p,int  n)///对从p开始连续地n个节点插入排序
{
    for(int r = 0;r<n;r++){
        insertA(search(p->data,r,p),p->data);
        p = p->succ;remove(p->pred);
    }


}

template<typename T>
List<T>::List(List<T> const& L)//默认复制列表L
{
    copyNodes(L.first(),L._size);
}

template<typename T>
List<T>::List(List<T> const& L,Rank r,int n)//复制列表L中自第r项起的n项
{
    ListNodePosi(T) p = L.first();
    while( 0 < r--) p = p->succ;
    copyNodes(p,n);
}

template<typename T>
List<T>::List(ListNodePosi(T) p,int n)//复制列表中自位置p起的n项
{
    copyNodes(p,n);
}

template<typename T>
List<T>::~List()//释放所有节点
{
    clear();
    delete header;
    delete trailer;
}

template<typename T>
T& List<T>::operator[] (Rank r)const//重载，支持循秩访问(效率低)
{
    ListNodePosi(T) p = first();
    while(0<r--) p = p->succ;
    return p->data;
}
    
template<typename T>
ListNodePosi(T) List<T>::find(T const& e,int n,ListNodePosi(T) p) const//无序区间查找
{
    while (0 < n--)
    {
        if(e == (p = p->pred)->data) return p;
    }
    return nullptr;
}  

template<typename T>
ListNodePosi(T) List<T>::search(T const& e,int n,ListNodePosi(T) p) const//有序区间查找
{
    do{
        p = p->pred;n--;
    }while((-1<n) && (e<p->data));
    return p;
}


template<typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e)//将e当作首节点插入
{
    _size++;return header->insertAsSucc(e);
}
template<typename T>
ListNodePosi(T) List<T>::insertAsLast(T const& e)//将e当作末节点插入
{
    _size++;return trailer->insertAsPred(e);
}
template<typename T>
ListNodePosi(T) List<T>::insertA(ListNodePosi(T) p,T const& e)//将e当作p的后继节点插入
{
    _size++;return p->insertAsSucc(e);
}
template<typename T>
ListNodePosi(T) List<T>::insertB(ListNodePosi(T) p,T const& e)//将e当作p的前驱节点插入
{
    _size++;return p->insertAsPred(e);
}


template<typename T>
void List<T>::sort(ListNodePosi(T) p,int n)//列表区间排序
{
    switch (rand() % 3)
    {
    case 1: insertionSort(p,n);
        break;
    case 2: selectionSort(p,n);
        break;
    
    default: mergeSort(p,n);
        break;
    }
}

template<typename T>
int List<T>::deduplicate()//无序去重
{
    int oldSize = _size;ListNodePosi(T) p = first();
    for(Rank r  = 0; p != trailer;p = p->succ)
        if(ListNodePosi(T) q = find(p->data,r,p))
            remove(q);
        else r++;
    return oldSize - _size;
}


template<typename T>
int List<T>::uniquify()//有序去重
{
    if(_size < 2) return 0;
    int oldSize = _size;
    ListNodePosi(T) p = first();ListNodePosi(T) q;
    while(trailer != (q = p->succ))
        if(p->data != p->data) p=q;
        else remove(q);
    return oldSize - _size;
}
template<typename T>
void List<T>::reverse()//前后倒置
{
    ListNodePosi(T) p = header;ListNodePosi(T) q = trailer;
    for(int i = 1;i<_size; i+=2)
        std::swap((p = p->succ)->data,(q = q->pred)->data);
}

//遍历
template<typename T>
void List<T>::traverse(void (*visit) (T&))//遍历
{
    for(ListNodePosi(T) p = header->succ;p != trailer;p = p->succ)
    visit(p->data);
}

template<typename T> template<typename VST>//操作器
void List<T>::traverse(VST& visit)//遍历
{
    for(ListNodePosi(T) p =  header->succ; p != trailer; p = p->succ)
        visit(p->data);
}
#endif//LIST_H