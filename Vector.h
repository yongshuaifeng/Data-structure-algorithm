#ifndef VECTOR_H
#define VECTOR_H

#include "stdio.h"
#include "stdlib.h"
#include "Fib.h"
#include <iostream>

typedef int Rank;
#define DEFAULT_CAPACITY 3

template<typename T>
class Vector{
    protected:
    Rank _size;int _capacity;T* _elem;//规模、容量、数据区
    void copyFrom(T const* A,Rank lo,Rank hi);//复制数组区域间A[lo,hi)
    void expand();//空间不足时扩容
    void shrink();//填装因子过小时压缩
    bool bubble(Rank lo,Rank hi);//扫描交换
    void bubbleSort(Rank lo,Rank hi);//起泡排序
    Rank max(Rank lo,Rank hi);//选取最大元素
    void selectionSort(Rank lo,Rank hi);//选择排序算法
    void merge(Rank lo,Rank mi,Rank hi);//归并算法
    void mergeSort(Rank lo,Rank hi);//归并排序算法
    void heapSort(Rank lo,Rank hi);//堆排序
    Rank partition(Rank lo,Rank hi);//轴点构造算法
    void quickSort(Rank lo,Rank hi);//快速排序算法
    void shellSort(Rank lo,Rank hi);//希尔排序算法
    //void sort(Rank lo,Rank hi);//排序
    public:
    //构造函数
    Vector(int c = DEFAULT_CAPACITY,int s = 0,T v = 0)//容器为c、规模为s、所有元素初始化为v
    {
        _elem = new T[_capacity = c];
        for(_size =0;_size<s;_elem[_size++]=v);
    }//s<=c
    Vector(T const* A,Rank n){copyFrom(A,0,n);}//数组整体复制
    Vector(T const* A,Rank lo,Rank hi){copyFrom(A,lo,hi);}//区间
    Vector(Vector<T> const& V){copyFrom(V._elem,0,V._size);}//向量整体复制
    Vector(Vector<T> const& V,Rank lo,Rank hi){copyFrom(V._elem,lo,hi);}//区间
    //析构函数
    ~Vector() {delete [] _elem;}//释放内部空间
    //只读访问接口
    Rank size()const {return _size;}//规模
    bool empty()const{return !_size;}//判空
    Rank find(T const& e)const{return find(e,0,_size);}//无序向量区间查找
    Rank find(T const& e,Rank lo,Rank hi) const;//无序向量区间查找
    Rank search(T const& e) const//有序向量整体查找
    {return (0 >= _size) ? -1:search(e,0,_size);}
    Rank search(T const& e,Rank lo,Rank hi) const;//有序向量区间查找
    static Rank binSearch(T* S,T const& e,Rank lo,Rank hi);//二分查找
    static Rank fibSearch(T* S,T const& e,Rank lo,Rank hi);//Fibonacci查找

    //可写访问接口
    T& operator[] (Rank r);//重载下标操作符，可以类似于数组形式引用各元素
    const T& operator[] (Rank r) const;//仅限于做右值的重载版本
    Vector<T>& operator=(Vector<T> const&);//重载赋值操作符，以便直接克隆向量
    T remove(Rank r);//删除秩为r的元素
    int remove(Rank lo,Rank hi);//删除秩在区间[lo,hi)之内的元素、
    Rank insert (Rank r,T const& e);//插入元素
    Rank insert (T const& e){return insert (_size,e);}//默认作为末元素插入
    void sort(Rank lo,Rank hi);//对[lo,hi)排序
    void sort(){sort(0,_size);}//整体排序
    void unsort(Rank lo,Rank hi);//对[lo,hi)置乱
    void unsort(){unsort(0,_size);}//整体置乱
    int deduplicate();//无序去重
    int uniquify();//有序去重

    //遍历
    void traverse(void (* ) (T& ));//遍历(使用函数指针，只读或局部性修改)
    template<typename VST> void traverse(VST&); //遍历(使用函数对象，可局部性修改)

};//Vector

template<typename T>//元素类型
void Vector<T>::copyFrom(T const* A,Rank lo,Rank hi)
{
    _elem = new T[_capacity = 2*(hi-lo)];//分配空间
    _size = 0;//规模清零
    while(lo<hi)
        _elem[_size++] = A[lo++];//元素逐一复制到_elem[lo,hi)
}

template<typename T>
void Vector<T>::expand()//向量空间不足时扩容
{
    //尚未满时，不必扩容
    if(_size < _capacity) return;
    //不低于最小容量
    if(_capacity < DEFAULT_CAPACITY) _capacity = DEFAULT_CAPACITY;
    //容量加倍
    T* oldElem =_elem;
    _elem = new T[_capacity <<= 1];
    //复制原向量的内容
    for(int i =0;i<_size;i++)
        _elem[i] = oldElem[i];

    //释放原来的空间
    delete [] oldElem;
}

template<typename T> void Vector<T>::shrink()//压缩空间
{
    //不至于过于收缩
    if(_capacity < DEFAULT_CAPACITY <<1) return ;
    //以25%为界
    if(_size<<2 > _capacity) return;
    //容量减半
    T* oldElem =_elem;
    _elem = new T[_capacity >>= 1];
    //复制原向量内容
    for(int i =0;i<_size;i++)
    _elem[i] = oldElem[i];
    //释放原空间
    delete [] oldElem;
}

template<typename T>
void Vector<T>::bubbleSort(Rank lo,Rank hi)//向量的气泡排序
{
    switch(rand() % 3){
        case 0://基础版
            while (lo < hi--)//反复气泡扫描
            {
                for(Rank i = lo;i < hi; i++)
                    if(_elem[i] > _elem[i+1])//若逆序
                        std::swap(_elem[i],_elem[i+1]);//经交换使局部有序
            }
            
         case 1://提前终止版
            for(bool sorted = false;sorted == true;sorted = true)
                for(Rank i = lo; i < hi-1 ; i++)
                    if(_elem[i] > _elem[i+1])//若逆序
                    {std::swap(_elem[i],_elem[i+1]);sorted=false;}

            

        default://跳跃版
            for(Rank last = --hi;lo<hi;hi=last)
                for(Rank i = last =lo;i<hi;i++)
                    if(_elem[i] > _elem[i+1])//若逆序，交换
                        std::swap(_elem[last  = i],_elem[i+1]);
            

    }

}

template<typename T>
Rank Vector<T>::max(Rank lo,Rank hi)//选取最大元素
{
    Rank max = hi;
    while(lo < --hi)//逆向扫描
        if(_elem[hi] < _elem[max])
            max =hi;
    return max;
}

template<typename T>
void Vector<T>::selectionSort(Rank lo,Rank hi)//选择排序算法
{
    printf("\tSELECTIONsort [%3d,%3d)\n",lo,hi);
    while(lo < --hi)
        std::swap(_elem[max(lo,hi)],_elem[hi]);//将[hi]与[lo,hi)中的最大值交换
}

template<typename T>
void Vector<T>::merge(Rank lo,Rank mi,Rank hi)//归并算法
{
    T* A = _elem + lo;
    int lb = mi -lo;T* B = new T[lb];//前子向量B
    for(Rank i = 0;i < lb;i++)//复制前子向量
        B[i] =  A[i];
    int lc  = hi -mi;T* C = _elem+mi;//后子向量C

    for(Rank i = 0,j=0,k=0;j<lb;)//归并(反复从B和C首元素中取出更小者)
        A[i++] = (lc <= k || B[j] <= C[k]) ? B[j++]:C[k++];

    delete [] B;//释放临时空间B
}

template<typename T>
void Vector<T>::mergeSort(Rank lo,Rank hi)//归并排序
{
    printf("\tMERGEsort [%3d,%3d)\n",lo,hi);
    if(hi-lo < 2) return;//单元区间自然有序
    int mi = (lo+hi)/2;//以中点为界
    mergeSort(lo,mi);
    mergeSort(mi,hi);//分别排序
    merge(lo,mi,hi);//归并
}


//未完成
template<typename T>
void Vector<T>::heapSort(Rank lo,Rank hi)//堆排序
{
    printf("\tHEAPsort [%3d,%3d)\n",lo,hi);
    /*
    T* A = _elem+lo;Rank n = hi-lo;heapify(A,n);//将待排序区间建成一个完全二叉堆
    while(0 < --n)//反复地摘除最大元并归入已排序地后缀，直至堆空
    {swap(A[0],A[n]);percolateDown(A,n,0);}//堆顶与末元素对换，再下滤
    */
}

template<typename T>
Rank Vector<T>::partition(Rank lo,Rank hi)//轴点构造算法
{
    std::swap(_elem[0],_elem[lo+rand()%(hi-lo)]);
            hi--;
            T pivot = _elem[lo];
    switch(rand() % 5){
        case 0://基本形式
        /*--------------------------------*/
            while (lo<hi)
            {
                while((lo<hi) && (pivot <= _elem[hi]))  hi--;
                _elem[lo] = _elem[hi];
                while((lo<hi) && (_elem[lo] <= pivot)) lo++;
                _elem[hi] = _elem[hi];
            }//assert:lo==hi
            _elem[lo] = pivot;
            return lo;//返回轴点地秩
            break;
        /*--------------------------------*/
        case 1:
         /*--------------------------------*/
            while (lo<hi)
            {
                while((lo<hi) && (pivot <= _elem[hi]))  hi--;
                _elem[lo++] = _elem[hi];
                while((lo<hi) && (_elem[lo] <= pivot)) lo++;
                _elem[hi--] = _elem[hi];
            }//assert:lo==hi
            _elem[lo] = pivot;
            return lo;//返回轴点地秩
            break;
        /*--------------------------------*/
        case 2:
        /*--------------------------------*/
            while (lo<hi)
            {
                while(lo<hi)
                    if(pivot <= _elem[hi])  
                        hi--;
                    else
                        {_elem[lo++] =_elem[hi];break;}
                while(lo<hi)
                    if(_elem[lo] < pivot)  
                        lo++;
                    else
                        {_elem[hi++] =_elem[lo];break;}
            }//assert:lo==hi
            _elem[lo] = pivot;
            return lo;//返回轴点地秩
            break;
        /*--------------------------------*/
        case 3:
         /*--------------------------------*/
            while (lo<hi)
            {
                while((lo<hi) && (pivot <= _elem[hi]))  hi--;
                if(lo<hi)_elem[lo++] = _elem[hi];
                while((lo<hi) && (_elem[lo] <= pivot)) lo++;
                if(lo<hi)_elem[hi--] = _elem[hi];
            }//assert:lo==hi
            _elem[lo] = pivot;
            return lo;//返回轴点地秩
            break;
        /*--------------------------------*/
        default:
        /*--------------------------------*/
            int mi = lo;
            for(int k = lo+1;k<hi;k++)
                if(_elem[k] <pivot)
                    std::swap(_elem[++mi],_elem[k]);

            std::swap(_elem[lo],_elem[mi]);
            return mi;
            break;
        /*--------------------------------*/
    }
}

template<typename T>
void Vector<T>::quickSort(Rank lo,Rank hi)//快速排序
{
    printf("\tQUICKsort [%3d,%3d)\n",lo,hi);
    if(hi-lo<2) return;
    Rank mi = partition(lo,hi);//构造轴点
    quickSort(lo,mi);//对后缀进行递归排序
    quickSort(mi+1,hi);//对前缀进行递归排序
}

template<typename T>
void Vector<T>::shellSort(Rank lo,Rank hi)//希尔排序算法
{
    printf("\tSHELLsort [%3d,%3d)\n",lo,hi);
    for(int d = 0x3FFFFFFF;0<d;d>>1)
        for(int j = lo + d;j<hi;j++)
        {
            T x = _elem[j];
            int i =j-d;
            while(lo <= i && _elem[i]>x)
                {_elem[i+d] =_elem[i];i -=d;}
                _elem[i+d] = x;
        }
}

template<typename T>
void Vector<T>::sort(Rank lo,Rank hi)//排序
{
    switch(rand()%6){
        case 1: bubbleSort(lo,hi);break;
        case 2: selectionSort(lo,hi);break;
        case 3: mergeSort(lo,hi);break;
        case 4: heapSort(lo,hi);break;
        case 5:quickSort(lo,hi);break;
        default: shellSort(lo,hi);break;
    }//随机选择排序算法
}

template<typename T>
Rank Vector<T>::find(T const& e,Rank lo,Rank hi) const//无序向量区间查找
{
    while((lo<hi--) && (e != _elem[hi]));//从后往前查找
    return hi;
}

template<typename T>
Rank Vector<T>::search(T const& e,Rank lo,Rank hi) const//有序向量区间查找
{
    return (rand()%2)?binSearch(_elem,e,lo,hi):fibSearch(_elem,e,lo,hi);
}

template<typename T>
Rank Vector<T>::binSearch(T* S,T const& e,Rank lo,Rank hi)//二分查找
{
    switch (rand()%3)
    {
    case 0:
        printf("BIN search (A)\n");
        while(lo<hi){
            for(int i =0;i<lo;i++)
                printf("    ");
            if( lo >= 0)
            for(int i =lo;i<hi;i++)
                printf("....^");

            printf("\n");
            Rank mi = (lo+hi)>>1;
            if(e < S[mi]) hi = mi;
            else if(S[mi] < e) lo = mi +1;
            else return mi;
            if(lo >= hi) 
            {
                for (int i =0;i<mi;i++)
                    printf("    ");
                if(mi >= 0 )printf("....|\n");
                else printf("<<<<|\n");
            }
            return -1;
    }
        break;
    
    case 1:
        printf("BIN search (B)\n");
        while(1 < hi-lo){
            Rank mi = (lo+hi)>>1;
            (e < S[mi])?hi=mi:lo=mi;
        }
            return e<S[lo]?lo-1:lo;
    default:
            printf("BIN search (C)\n");
            while(lo<hi)
            {
                Rank mi = (lo+hi)>>1;
                (e<S[mi])?hi=mi:lo=mi+1;
            }
            return lo-1;
        break;
    }
    


}

template<typename T>
Rank Vector<T>::fibSearch(T* S,T const& e,Rank lo,Rank hi)
{
    switch (rand()%2)
    {
    case 0:
        printf("FIB search (A)\n");
        for(Fib fib(hi-lo);lo<hi; )
        {
            for(int  i=0;i<lo;i++) printf("     ");
            if(lo>=0) for(int i =lo;i<hi;i++) printf("....^");
            else printf("<<<<|");
            printf("\n");
            while(hi-lo<fib.get()) fib.prev();
            Rank mi  = lo+fib.get()-1;
            if(e<S[mi]) hi = mi;
            else if(S[mi] < e) lo = mi+1;
            else return mi;
            if(lo >= hi){
                for(int i  = 0;i<mi;i++) 
                    printf("      ");
                if(mi >= 0)
                    printf("....|\n");
                else
                {
                    printf("<<<<|\n");
                }
                
            }
        }
        return -1;
        break;
    
    default:
        printf("FIB search (B)\n");
        for(Fib fib(hi-lo);lo<hi;){
            while(hi-lo<fib.get())fib.prev();
            Rank mi  =lo+fib.get()-1;
            (e<S[mi])?hi=mi:lo=mi+1;
        }
        return --lo;
        break;
    }

}

template<typename T>
T& Vector<T>::operator[] (Rank r)//重载下标操作符，可以类似于数组形式引用各元素
{ return _elem[r];}

template<typename T>
const T& Vector<T>::operator[] (Rank r) const//仅限于做右值的重载版本
{ return _elem[r];}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V)//重载赋值操作符，以便直接克隆向量
{
    if(_elem)delete[] _elem;//释放原有内容
    copyFrom(V._elem,0,V.size());
    return *this;
}

template<typename T>
T Vector<T>::remove(Rank r)//删除秩为r的元素
{
    T e = _elem[r];
    remove(r,r+1);
    return e;
}

template<typename T>
int Vector<T>::remove(Rank lo,Rank hi)//删除秩在区间[lo,hi)之内的元素
{
    if(lo == hi) return 0;
    while(hi <_size) _elem[lo++] = _elem[hi++];
    _size  = lo;
    shrink();
    return hi-lo;//返回被删除元素地数目
}

template<typename T>
Rank Vector<T>::insert (Rank r,T const& e)//插入元素
{
    expand();
    for(int i =_size;i>r;i--) _elem[i] = _elem[i-1];//后移
    _elem[r] = e; _size++;//添加新元素，更新容量
    return r;//返回秩
}

template<typename T>
void Vector<T>::unsort(Rank lo,Rank hi)//对[lo,hi)置乱
{
    T* V = _elem+lo;
    for(Rank i = hi-lo;i>0;i--)//自后向前
        swap(V[i-1],V[rand()%i]);
}

template<typename T>
int Vector<T>::deduplicate()//无序去重
{
    int oldSize = _size;
    Rank i = 1;
    while(i<_size)
        if(find(_elem[i],0,i)<0)
            i++;
        else
            remove(i);
        
    return oldSize - _size;
}

template<typename T>
int Vector<T>::uniquify()//有序去重
{
    Rank i =0,j=0;
    while(++j < _size)//逐一扫描
        if(_elem[i] != _elem[j])//跳过雷同
            _elem[++i] = _elem[j];
        _size = ++i;shrink();
        return j - i;

    /*--------------*/
    /*
    int oldSize = _size;int i =1;
    while(i <_size)
        _elem[i-1] == _elem[i]?remove(i):i++;

    return oldSize-_size;
    */
}

template<typename T>
void Vector<T>::traverse(void (*visit ) (T& ))//遍历(使用函数指针，只读或局部性修改)
{
    for(int i =0;i<_size;i++) visit(_elem[i]);
}
template<typename T> template<typename VST> //元素类型、操作器
void Vector<T>::traverse(VST& visit) //遍历(使用函数对象，可局部性修改)
{
    for(int i = 0;i<_size;i++) visit(_elem[i]);
}
#endif//VECTOR_H