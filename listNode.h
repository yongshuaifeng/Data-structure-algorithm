#ifndef LISTNODE_H
#define LISTNOODE_H

typedef int Rank;
#define ListNodePosi(T) ListNode<T>* //列表节点位置

template<typename T> struct ListNode{//列表节点模板类
//成员
    T data;                 //数值
    ListNodePosi(T) pred;   //前驱
    ListNodePosi(T) succ;   //后继

//构造函数
    ListNode() {}//针对header和trailer地构造
    ListNode(T e,ListNodePosi(T) p = nullptr,ListNodePosi(T) s = nullptr):data(e),pred(p),succ(s){}//默认构造函数
    
//操作接口
    ListNodePosi(T) insertAsPred(T const& e);//紧靠当前节点之前插入新节点
    ListNodePosi(T) insertAsSucc(T const& e);//紧随当前节点之后插入新节点
};

template<typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e)
{
    ListNodePosi(T) x = new ListNode(e,pred,this);//创建新节点
    pred->succ  = x;    pred = x;//设置正向链接
    return x;//返回新节点地位置
}

template<typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e)
{
    ListNodePosi(T) x = new ListNode(e,this,succ);//创建新节点
    succ->pred = x;succ = x;//设置逆向链接
    return x;//返回新节点地位置
}





#endif//LISTNODE_H