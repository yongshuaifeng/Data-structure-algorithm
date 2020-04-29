#ifndef BINNODE_H
#define BINNODE_H

#include"Queue.h"
#include"Stack_vector.h"

#define BinNodePosi(T) BinNode<T>* //节点位置
#define stature(p) ((p) ? (p)->hight:-1)//节点高度
typedef enum{RB_RED,RB_BLACK} RBColor;//节点颜色


//AVL
#define HeightUpdated(x)    ((x).height == 1 + __max(stature((x).lc),stature((x).rc)))
#define Balanced(x) (stature((x).lc) == stature((x).rc))
#define BalFac(x) (stature((x).lc) - stature((x).rc))
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))

//BASIC
#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (& (x) == (x).parent->lc))
#define IsRChild(x) (! IsRoot(x) && (& (x) == (x).parent->rc))
#define HasParent(x) (! IsROot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (! HasChild(x))

#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc))
#define uncle(x) (IsChild(*((x)->parent)) ? (x)->parent->parent->rc:(x)->parent->parent->lc)
#define FromParentTo(x) (IsRoot(x) ? this->_root:(IsChild(x) ? (x).parent->lc : (x).parent->rc))
//RedBlack
#define IsBlack(p) (!(p) || (RC_BLACK == (p)->color))
#define IsRed(p) (!IsBlack(p))
#define BlackHeightUpdated(x) ((stature((x).lc) == stature((x).rc)) && ((x).height == (IsRed(& x) ? stature((x).lc):stature((x).lc)+1)))




template<typename T> struct BinNode{//二叉树节点模板类
    //成员
    T data;//数值
    BinNodePosi(T) parent;BinNodePosi(T) lc;BinNodePosi(T) rc;//父节点及左右孩子
    int height;//高度
    int npl;//Null Path Length
    RBColor color;//颜色(红黑树)
    //构造函数
    BinNode():parent(nullptr),lc(nullptr),rc(nullptr),height(0),npl(1),color(RB_RED){}
    BinNode(T e,BinNodePosi(T) p = nullptr,BinNodePosi(T) lc = nullptr,BinNodePosi(T) rc = nullptr,int h = 0,int l = 1,RBColor c = RB_RED):
        data(e),parent(p),lc(lc),rc(rc),height(h),npl(l),color(c){}
    //操作接口
    int size();//统计当前节点后代总数
    BinNodePosi(T) insertAsLC(T const&);//作为当前节点的左孩子插入新节点
    BinNodePosi(T) insertAsRC(T const&);//作为当前节点的右孩子插入新节点
    BinNodePosi(T) succ();//取当前节点的直接后继
    template<typename VST> void travLevel(VST&);//子树层次遍历
    template<typename VST> void travPre(VST&);//子树先序遍历
    template<typename VST> void travPre_I1(BinNodePosi(T) ,VST&);//子树先序遍历
    template<typename VST> void travPre_I2(BinNodePosi(T) ,VST&);//子树先序遍历
    template<typename VST> static void visitAlongVine ( BinNodePosi(T), VST& , Stack<BinNodePosi(T)>& );
    template<typename VST> void travPre_R(BinNodePosi(T) ,VST&);//子树先序遍历
    template<typename VST> void travIn(VST&);//子树中序遍历
    static void goAlongVine(BinNodePosi(T),Stack<BinNodePosi(T)>&);
    template<typename VST> void travIn_I1(BinNodePosi(T) ,VST&);
    template<typename VST> void travIn_I2(BinNodePosi(T) ,VST&);
    template<typename VST> void travIn_I3(BinNodePosi(T) ,VST&);
    template<typename VST> void travIn_I4(BinNodePosi(T) ,VST&);
    template<typename VST> void travIn_R(BinNodePosi(T) ,VST&);
    template<typename VST> void travPost(VST&);//子树后序遍历
    template<typename VST> void travPost_I(BinNodePosi(T),VST&);
    template<typename VST> void travPost_R(BinNodePosi(T),VST&);
    static void gotoLeftmostLeaf(Stack<BinNodePosi(T)>&);

    //比较器、判等器
    bool operator<(BinNode const& bn){return data<bn.data;}//小于
    bool operator==(BinNode const& bn){return data==bn.data;}//等于

    /*DSA*/
    /*DSA*/BinNodePosi(T) zig();//顺时针旋转
    /*DSA*/BinNodePosi(T) zag();//逆时针旋转
    /*DSA*/BinNodePosi(T) balance();//完全平衡化
    /*DSA*/BinNodePosi(T) imitate(const BinNodePosi(T));

    void stretchByZig(BinNodePosi(T)&);
    void stretchByZag(BinNodePosi(T)&);

};

template<typename T> int BinNode<T>::size(){
    int s  = 1;//计入本身
    if(lc) s += lc->size();//递归计入左子树规模
    if(rc) s += rc->size();//递归计入右子树规模
    return s;
}

template<typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)//作为当前节点的左孩子插入新节点
{
    return lc =  new BinNode(e,this);
}

template<typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e)//作为当前节点的右孩子插入新节点
{
    return rc = new BinNode(e,this);
}

template<typename T>
BinNodePosi(T) BinNode<T>::succ()//取当前节点的直接后继
{
    BinNodePosi(T) s = this;//记录后继的临时变量
    if(rc){//若有右孩子，则直接后继必在右子树中
            s = rc;//右子树
            while(HasLChild(*s)) s = s->lc;//最靠左（最小）的节点
    }else
    {//否则，直接后继应是
        while (IsRChild(*s))//逆向地沿右向分支，不断朝左上方移动
        {
            s = s->parent;
        }
        s = s->parent; //最后再朝右上方移动一步
    }
    return s;
}
template<typename T> template<typename VST> 
void BinNode<T>::travLevel(VST& visit)//子树层次遍历
{
    Queue<BinNodePosi(T)> Q;
    Q.enqueue(this);
    while (!Q.empty())
    {
        BinNodePosi(T) x = Q.enqueue();visit(x->data);
        if(HasLChild(*x)) Q.enqueue(x->lc);//左孩子入队
        if(HasRChild(*x)) Q.enqueue(x->rc);//右孩子入队
    }
    
}


template<typename T> 
template<typename VST> 
void  BinNode<T>::travPre(VST& visit)//子树先序遍历
{
    switch (rand() % 3)
    {
    case 1: travPre_I1(this,visit);
        break;
    case 2: travPre_I2(this,visit);
        break;
    default: travPre_R(this,visit);
        break;
    }
}

template<typename T> 
template<typename VST> 
void  BinNode<T>::travPre_I1(BinNodePosi(T) x,VST& visit)//子树先序遍历
{
    Stack<BinNodePosi(T)> S;//辅助栈
    if(x) S.push(x);//根节点入栈
    while(!S.empty()){
        x = S.pop();visit(x->data);
        if(HasRChild(*x)) S.push(x->rc);
        if(HasLChild(*x)) S.push(x->lc);
    }
}

template<typename T> 
template<typename VST> 
void  BinNode<T>::travPre_I2(BinNodePosi(T) x,VST& visit)//子树先序遍历
{
    Stack<BinNodePosi(T)> S;
    while(true){
        visitAlongVine(x,visit,S);//从当前节点出发，逐批访问
        if(S.empty()) break;//直到栈空
        x = S.pop();//弹出下一批的起点
    }
}

template<typename T> 
template<typename VST> 
void  BinNode<T>::visitAlongVine (BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S )
{
    while(x){
        visit(x->data);
        S.push(x->rc);
        x = x->lc;
    }
}

template<typename T> 
template<typename VST> 
void  BinNode<T>::travPre_R(BinNodePosi(T) x,VST& visit)//子树先序遍历
{
    if(!x) return;
    visit(x->data);
    travPre_R(x->lc,visit);
    travPre_R(x->rc,visit);
}


template<typename T> 
template<typename VST> 
void  BinNode<T>::travIn(VST& visit)//子树中序遍历
{
    switch (rand()%5)
    {
    case 1:
        travIn_I1(this,visit);
        break;
    case 2:
        travIn_I2(this,visit);
        break;
    case 3:
        travIn_I3(this,visit);
        break;
    case 4:
        travIn_I4(this,visit);
        break;
    
    default: travIn_R(this,visit);
        break;
    }
}

template<typename T>
void  BinNode<T>::goAlongVine(BinNodePosi(T) x,Stack<BinNodePosi(T)>& S)
{
    while(x) {S.push(x);x=x->lc;}
}

template<typename T> 
template<typename VST> 
void  BinNode<T>::travIn_I1(BinNodePosi(T) x,VST& visit)
{
    Stack<BinNodePosi(T)> S;
    while (true)
    {
        goAlongVine(x,S);
        if(S.empty()) break;
        x = S.pop();visit(x->data);
        x = x->rc;
    }
    
}


template<typename T> 
template<typename VST> 
void  BinNode<T>::travIn_I2(BinNodePosi(T) x,VST& visit)
{
    Stack<BinNodePosi(T)> S;
    while(true){
       if(x){
           S.push(x);
           x = x->lc;
       }else if(!S.empty()){
           x = S.pop();
           visit(x->data);
           x = x->rc;
       }else
            break;
    }
}

template<typename T> 
template<typename VST> 
void  BinNode<T>::travIn_I3(BinNodePosi(T) x,VST& visit)
{
    bool backtrack =  false;
    while(true){
        if(!backtrack && HasLChild(*x))
            x = x->lc;
        else{
            visit(x->data);
            if(HasRChild(*x)){
                x = x->rc;
                backtrack = false;
            }else{
                if(!(x = x->succ() )) break;
                backtrack = true;
            }
        }
    }
}

template<typename T> 
template<typename VST> 
void  BinNode<T>::travIn_I4(BinNodePosi(T) x,VST& visit)
{
    while(true)
        if(HasLChild(*x))
            x = x->lc;
        else{
            visit(x->data);
            while(!HasRChild(*x))
                if(!(x = x->succ)) return;
                else visit(x->data);
            x = x->rc;
        }
}

template<typename T> 
template<typename VST> 
void  BinNode<T>::travIn_R(BinNodePosi(T) x,VST& visit)
{
    if(!x) return;
    travIn_R(x->lc,visit);
    visit(x->data);
    travIn_R(x->rc);
}



template<typename T> 
template<typename VST> 
void BinNode<T>::travPost(VST& visit)//子树后序遍历
{
    switch (rand()%2)
    {
    case 1: travPost_I(this,visit);
        break;
    
    default: travPost_R(this,visit);
        break;
    }
}

template<typename T>
void BinNode<T>::gotoLeftmostLeaf(Stack<BinNodePosi(T)>& S)
{
    while(BinNodePosi(T) x = S.top())
        if(HasLChild(*x)){
            if(HasRChild(*x)) S.push(x->rc);
            S.push(x->lc);
        }else
        {
            S.push(x->rc);
        }
        S.pop();
}

template<typename T> 
template<typename VST>
void  BinNode<T>::travPost_I(BinNodePosi(T) x,VST& visit)
{
    Stack<BinNodePosi(T)> S;
    if(x) S.push(x);
    while(!S.empty()){
        if(S.top() != x->parent)
        gotoLeftmostLeft(S);
        x = S.pop();visit(x->data);
    }
}

template<typename T> 
template<typename VST>
void BinNode<T>::travPost_R(BinNodePosi(T) x,VST& visit)
{
    if(!x) return;
    travPost_R(x->lc,visit);
    travPost_R(x->rc,visit);
    visit(x->data);
}


    /*DSA*/
template<typename T>BinNodePosi(T) BinNode<T>::zig()//顺时针旋转
{
    BinNodePosi(T) lChild = lc;
    lChild->parent = this->parent;
    if(lChild->parent)
        ((this == lChild->parent->rc)?lChild->parent->rc:lChild->parent->lc)=lChild;
    lc = lChild->rc;if(lc) lc->parent = lChild;
    //update heights
    height = 1 + __max(stature(lc),stature(rc));
    lChild->height = 1 +__max(stature(lChild->lc),stature(lChild->rc));
    for(BinNodePosi(T) x = lChild->parent;x;x=x->parent)
        if(HeightUpdated(*x))
            break;
        else
        {
            x->height = 1 + __max(stature(x->lc),stature(x->rc));
        }
        return lChild;
}

/*DSA*/
template<typename T>
BinNodePosi(T) BinNode<T>::zag()//逆时针旋转
{
    BinNodePosi(T) rChild = rc;
    rChild->parent = this->parent;
    if(rChild->parent)
        ((this == rChild->parent->lc)?rChild->parent->lc:rChild->parent->rc) = rChild;
    rc = rChild->lc;if(rc) rc->parent = this;
    rChild->lc = this;this->parent = rChild;
    //update heights
    height = 1 + __max(stature(lc),stature(rc));
    rChild->height = 1 + __max(stature(rChild->lc),stature(rChild->rc));
    for(BinNodePosi(T) x = rChild->parent;x;x = x->parent)
        if(HeightUpdated(*x))
            break;
        else
        {
            x->height = 1 + __max(stature(x->lc),stature(x->rc));
        }
        return rChild;     
}

template<typename T>
void BinNode<T>::stretchByZig(BinNodePosi(T)& x)
{
    int  c = 0;
    BinNodePosi(T) v = x;
    while(x->lc) x = x->lc;
    for(;v;v =  v->lc){
        while (v->lc)
        {
            v = v->zig();
            c++;
        }
    }
    printf("\nsize = %d,height = %d, #zig = %d\n",x->size(),x->height,c);
}

template<typename T>
void BinNode<T>::stretchByZag(BinNodePosi(T)& x)
{
    int c = 0;
    BinNodePosi(T) p = x;while(p->rc) p = p->rc;
    while(x->lc) x = x->lc;
    for(;x != p;x = x->parent){
        while (x->rc)
        {
            x->zag();
            c++;
        }
    }
    printf("\nsize = %d, height = %d, #zag = %d\n\n",x->size(),x->height,c);
}

#endif//BINNODE_H