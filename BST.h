#ifndef BST_H
#define BST_H

#include "BinTree.h"

template<typename T>
class BST : public BinTree<T>{
protected:
    BinNodePosi(T) _hot;//"命中"节点的父亲
    BinNodePosi(T) connect34(//按值"3+4"结构，联接3个节点及四个子树
        BinNodePosi(T),BinNodePosi(T),BinNodePosi(T),
        BinNodePosi(T),BinNodePosi(T),BinNodePosi(T),BinNodePosi(T)
    );
    BinNodePosi(T) rotateAt(BinNodePosi(T) x);//对x及其父亲、祖父做统一旋转调整
public:
    virtual BinNodePosi(T) & search(const T& e);//查找
    virtual BinNodePosi(T) insert(const T& e);//插入
    virtual bool remove(const T& e);//删除

    /*DSA*/
    /*DSA*/void stretchToLpath() {stretchByZag(this->_root);}
    /*DSA*/void stretchToRpath() {stretchByZig(this->_root);}
    /*DSA*/void stretch();
};

template<typename T>
BinNodePosi(T) BST<T>::connect34(//按值"3+4"结构，联接3个节点及四个子树
        BinNodePosi(T) a,BinNodePosi(T) b,BinNodePosi(T) c,
        BinNodePosi(T) T0,BinNodePosi(T) T1,BinNodePosi(T) T2,BinNodePosi(T)T3
    )
{
    a->lc = T0;if(T0) T0->parent = a;
    a->rc = T1;if(T1) T1->parent = a; updateHeight(a);
    c->lc = T2;if(T2) T2->parent = c;
    c->rc = T3;if(T3) T3->parent = c; updateHeight(c);
    b->lc = a; a->parent = b;
    b->rc = c; c->parent = b;updateHeight(b);
    return b;
}

template<typename T>
bool BST<T>::remove(const T& e){
    BinNodePosi(T) & x = search(e); if(x) return false;
    removeAt(x,_hot);this->_size--;
    updateHeightAbove(_hot);
    return true;
}

template<typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T) &x,BinNodePosi(T) & hot)
{
    BinNodePosi(T) w = x;
    BinNodePosi(T) succ = nullptr;
    if(!HasLChild(*x))
        succ = x = x->rc;
    else if(!HasRChild(*x))
        succ = x = x->lc;
    else{
        w = w->succ();
        swap(x->data,w->data);
        BinNodePosi(T) u = w->parent;
        ((u == x)? u->rc:u->lc)= succ = w->rc;
    }
    hot = w->parent;
    if(succ) succ->parent = hot;
    release(w->data);release(w);return succ;
}

template<typename T> BinNodePosi(T) BST<T>::rotateAt(BinNodePosi(T) v)
{
    if(!v) {printf("\a\nFail to rotate a null node\n");exit(-1);}
    BinNodePosi(T) p = v->parent;BinNodePosi(T) g = p->parent;
    if(IsLChild(*p))
        if(IsLChild(*v)){
            p->parent = g->parent;
            return connect34(v,p,g,v->lc,p->rc,g->rc);
        }else
        {
            v->parent = g->parent;
            return connect34(p,v,g,p->lc,v->lc,v->lc,v->rc);
        }
    else
        if(IsRChild(*v)){
            p->parent = g->parent;
            return connect34(g,p,v,g->lc,v->lc,v->rc);
        }else{
            v->parent = g->parent;
            return connect34(g,v,p,g->lc,v->lc,v->rc,p->rc);
        }
    
}


template<typename T>BinNodePosi(T) & BST<T>::search(const T & e)
{
    if(!this->_root || e == this->_root->data){this->_hot = nullptr;return this->_root;}
    for(this->_hot = this->_root;;){
        BinNodePosi(T) & c = (e < this->_hot->data )?this->_hot->lc:this->_hot->rc;
        if(!c || e == c->data) return c;this->_hot = c;
    }
}

#endif//BST_H