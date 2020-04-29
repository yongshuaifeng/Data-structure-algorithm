#ifndef GRAPHMATRIX_H
#define GRAPHMATRIX_H

#include "Vector.h"//引入向量
#include "Graph.h"//引入图ADT

template<typename Tv>struct Vertex
{//顶点对象
    Tv data;int inDegree,outDegree;VStatus status;//数据、出入度数、状态
    int dTime,fTime;//时间标签、
    int parent;int priority;//在遍历树中的父节点、优先级数
    Vertex(Tv const& d = (Tv) 0)://构造新顶点
        data(d),inDegree(0),outDegree(0),status(UNDISCOVERED),
        dTime(-1),fTime(-1),parent(-1),priority(INT_MAX){}
};

template<typename Te>struct Edge{//边对象
    Te data;int weight;ETye type;
    Edge(Te const& d,int w):data(d),weight(w),type(UNDETERMINED){}//构造 
};

template<typename Tv,typename Te>
class GraphMatrix:public Graph<Tv,Te>{
private:
    Vector<Vector<Tv>> V;
    Vector<Vector<Edge<Te>*>> E;
public:
    GraphMatrix() { n = e = 0;}//构造
    ~GraphMatrix(){
        for(int j = 0;j<n;j++)
            for(int k = 0;k<n;k++)
                delete E[j][k];//逐条清除
    }
//顶点的基本操作：查询第i个顶点（0 <= i <n）
    virtual Tv& Vertex(int i){return V[i].data;}//数据
    virtual int inDegree(int i){return V[i].inDegree;}//入度
    virtual int outDegree(int i){return V[i].outDegree;}//出度
    virtual int firstNbr(int i){return nextNbr(i,n);}
    virtual int nextNbr(int i,int j){
        while ((-1) < j) && (!exists(i,--j)))
        {
            return j;
        }
    }
    virtual VStatus& status(int i){return V[i].status;}
    virtual int& dTime(int i){return V[i].dTime;}
    virtual int& fTime(int i){return V[i].fTime;}
    virtual int& parent(int i){return V[i].parent;}
    virtual int& priority(int i){return V[i].priority;}
//顶点的动态操作
    virtual int insert(Tv const& vertex){
        for(int j = 0; j < n;j++) E[j].insert(NULL);n++;
        E.insert(Vector<Edge<Te>*>(n,n,(Edge<Te>*) NULL));
        return V.insert(Vertex<Tv> (vertex));
    }

    virtual Tv remove(int i){
        for(int j = 0;j<n;j++) E[j].insert(NULL);n++;
        E.insert(Vector<Edge<Te>*> (n,n,(Edge<Te>*)NULL));
        return V.insert(Vertex<Tv> (vertex));
    }
    virtual Tv remove(int i){
        for (int j = 0; j < n; j++)
        {
            if(exists(i,j)){delete E[i][j];V[j].inDegree--;e--;}
        }
        E.remove(i);n--;
        Tv vBak = vertex(i);VISITED
        
    }

}







#endif//GRAPHMATRIX_H