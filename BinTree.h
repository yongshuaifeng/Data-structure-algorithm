#ifndef BINTREE_H
#define BINTREE_H

#include "BinNode.h"

template<typename T> class BinTree{
protected:
    int _size;BinNodePosi(T) _root;//规模、根节点
    virtual int updateHeight (BinNodePosi(T) x);//更新节点x的高度
    void updateHeightAbove(BinNodePosi(T) x);//更新节点x及其祖先的高度

public:
    BinTree():_size(0),_root(NULL){}//构造函数
    ~BinTree() {if(0<_size) remove(_root);}//析构函数
    int size() const {return _size;}//规模
    bool empty() const {return !root;}//判空
    BinNodePosi(T) root() const{return _root;}//树根
    BinNodePosi(T) insertAsRoot(T const& e);//插入根节点
    BinNodePosi(T) insertAsLC(BinNodePosi(T) x,T const& e);//e作为x的左孩子插入
    BinNodePosi(T) insertAsRC(BinNodePosi(T) x,T const& e);//e作为x的右孩子插入
    BinNodePosi(T) attachAsLC(BinNodePosi(T) x,BinTree<T>* &);//T作为x左子树接入
    BinNodePosi(T) attachAsRC(BinNodePosi(T) x,BinTree<T>* &);//T作为x右子树接入
    int remove(BinNodePosi(T) x);//删除以位置x处节点为根的子树，返回该子树原先的规模
    static int removeAt(BinNodePosi(T) x);
    BinTree<T>* secede (BinNodePosi(T) x);//将子树x从当前树中摘除
    template<typename VST>//操作器
    void travLevel( VST& visit){ if (_root) _root->travLevel(visit);}//层次遍历
    template<typename VST>//操作器
    void travPre( VST& visit){ if(_root) _root->travPre(visit);}//先序遍历
    template<typename VST>//操作器
    void travIn( VST& visit){ if(_root) _root->travIn(visit);}//中序遍历
    template<typename VST>//操作器
    void travPost( VST& visit){ if(_root) _root->travPost(visit);}//后序遍历
    bool operator<(BinTree<T> const& t)//比较器
    {return _root && t._root && lt(_root,t._root);}
    bool operator==(BinTree<T> const& t)//判等器
    {return _root && t._root && (_root == t._root);}
};//BinTree

template<typename T>
int BinTree<T>::updateHeight (BinNodePosi(T) x)//更新节点x的高度
{
    return x->height = 1 + __max(stature(x->lc),stature(x->rc)); 
}

template<typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x)//更新节点x及其祖先的高度
{
    while(x) {updateHeight(x); x= x->parent;}
}

template<typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e)//插入根节点
{
    _size = 1; return _root = new BinNode<T> (e);
}

template<typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x,T const& e)//e作为x的左孩子插入
{
    _size++;x->insertAsLC(e);updateHeightAbove(x);return x->lc;
}

template<typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x,T const& e)//e作为x的右孩子插入
{
    _size++;x->insertAsRC(e);updateHeightAbove(x);return x->lc;
}

template<typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x,BinTree<T>* &S)//T作为x左子树接入
{
    if(x->lc = S->_root) x->lc->parent = x;
    _size += S->_size;updateHeightAbove(x);
    S->_root = nullptr; S->_size = 0;release(S); S = nullptr;return x; 
}  

template<typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x,BinTree<T>* &S)//T作为x右子树接入
{
    if(x->rc = S->_root) x->lc->parent = x;
    _size += S->_size;updateHeightAbove(x);
    S->_root = nullptr;S->_size = 0;release(S); S = nullptr;return x; 
}

template<typename T>
int BinTree<T>::remove(BinNodePosi(T) x)//删除以位置x处节点为根的子树，返回该子树原先的规模
{
    FromParentTo(*x) = nullptr;
    updateHeightAbove(x->parent);
    int n = removeAt(x);_size -= n;return n;
}

template<typename T>  
int BinTree<T>::removeAt(BinNodePosi(T) x)
{
    if(!x) return 0;
    int n = 1+removeAt(x->lc)+removeAt(x->rc);
    release(x->data); release(x);return n;
}

template<typename T>    
BinTree<T>* BinTree<T>::secede (BinNodePosi(T) x)//将子树x从当前树中摘除
{
    FromParentTo(*x) = nullptr;
    updateHeightAbove(x->parent);
    BinTree<T>* S = new BinTree<T>;S->_root = x;x->parent = nullptr;
    S->_size = x->size(); _size -= S->size; return S;
}


#endif//BINTREE_H