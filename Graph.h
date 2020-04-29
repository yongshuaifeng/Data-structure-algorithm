#ifndef GRAPH_H
#define GRAPH_H

#include "Stack_vector.h"
#include "Queue.h"

#include "stdlib.h"

typedef enum {UNDISCOVERED,DISCOVERED,VISITED} VStatus;//顶点状态
typedef enum {UNDETERMINED,TREE,CROSS,FORWARD,BACKWARD} EType;//边在遍历树中所属的类型

template<typename Tv,typename Te>//顶点类型、边类型
class Graph{//图Graph模板类
private:
    void reset(){//所有顶点、边的辅助信息复位
        for(int i = 0;i<n;i++){
            status(i) = UNDISCOVERED;dTime(i) = fTime(i) = -1;//状态，时间标签
            parent(i) = -1;priority(i) = INT_MAX;//(在遍历树中的)父节点，优先级数
            for(int j = 0;j<n;j++)
                if(exists(i,j)) type(i,j) = UNDETERMINED;//类型

        }
    }

    void BFS(int,int&);//广度优先搜索算法
    void DFS(int,int&);//深度优先搜索算法
    void BCC(int,int&,Stack<int>&);//基于DFS的双连通分量分解算法
    bool TSort(int,int&,Stack<Tv>*);//基于DFS的拓扑排序算法
    template <typename PU> void PFS(int,PU);//优先级搜索框架
public:
//顶点
    int n;//顶点总数
    virtual int insert(Tv const&) = 0;//插入顶点，返回编号
    virtual Tv remove(int)  = 0;//删除顶点及其关联边，返回该顶点信息
    virtual Tv& vertex(int) = 0;//顶点v的数据（该顶点的确存在）
    virtual int inDegree(int) = 0;//顶点v的入度（该顶点的确存在）
    virtual int outDegree(int) = 0;//顶点v的出度（该顶点的确存在）
    virtual int firstNbr(int) = 0;//顶点v的首个邻接顶点
    virtual int nextNbr(int,int)  = 0;//顶点v的（相对于顶点j的）下一个邻接顶点
    virtual VStatus& status(int) = 0;//顶点v的状态
    virtual int& dTime(int) = 0;//顶点v的时间标签dTime
    virtual int& fTime(int) = 0;//顶点v的时间标签fTime
    virtual int& parent(int) = 0;//顶点v在遍历树中的父亲
    virtual int& priority(int) = 0;//顶点v在遍历中的优先级数
//边：这里约定，无向边均统一转换为方向互逆的一对有向边，从而将无向图视作有向图的特例
    int e;//边总数
    virtual bool exists(int,int)  = 0;//边（v,u）是否存在
    virtual void insert(Te const&,int,int,int)  = 0;//在顶点v和u之间插入权重为w的边e
    virtual Te remove(int,int) = 0;//删除顶点v和u之间的边e，返回该边信息
    virtual EType& type (int,int) = 0;//边（v,u）的类型
    virtual Te& edge(int,int) = 0;//边（v,u）的数据（该边的确存在）
    virtual int& weight(int,int) = 0;//边(v,u)的权重
//算法
    void bfs(int);//广度优先搜索算法
    void dfs(int);//深度优先搜索算法
    void bcc(int);//基于DFS的双连通分量分解算法
    Stack<Tv>* tSort(int);//基于DFS的拓扑排序算法
    void prim(int);//最小支撑树Prim算法
    void dijkstra(int);//最短路径Dijkstra算法
    template<typename PU>void pfs(int,PU);//优先级搜索框架
};

template<typename Tv,typename Te>//广度优先搜索BFS算法（全图）
void Graph<Tv,Te>::bfs(int s){//assert:0 <= s < n
    reset();int clock = 0;int v= s;//初始化
    do//逐一检查所有顶点
        if(UNDISCOVERED == status(v))//一旦遇到尚未发现的顶点
            BFS(v,clock);//即从该顶点出发启动一次BFS
    while(s != (v = (++v%n)));//按序号检查，故不漏不重
}

template<typename Tv,typename Te>//g广度优先搜索BFS算法（单个连通域）
void Graph<Tv,Te>::BFS(int v,int& clock){//assert: 0 <= v < n
    Queue<int> Q;//引入辅助队列
    status(v) = DISCOVERED;Q.enqueue(v);//初始化起点
    while(!Q.empty()){//在Q变空之前，不断
        int v = Q.dequeue(); dTime(v) = ++clock;//取出队首顶点v
        for(int u = firstNbr(v);-1<u;u=nextNbr(v,u))//枚举v的所有邻居u
            if(UNDISCOVERED = status(u)){//若u尚未被发现
                status(u) = DISCOVERED;Q.enqueue(u);//发现该顶点
                type(v,u) = TREE;parent(u) = v;//引入树边拓展支撑树
            }else{//若u已被发现，或者甚至已访问完毕，则
                type(v,u) = CROSS;//将（v,u）归类于跨边
            }
        status(v) = VISITED;//至此，当前顶点访问完毕
    }
}
template<typename Tv,typename Te>//深度优先搜索DFS算法（全图）
void Graph<Tv,Te>::dfs(int s){//assert: 0 <= s <n
    reset();int clock = 0;int v = s;//初始化
    do//逐一检查所有顶点
    {
        if(UNDISCOVERED == status(v))//一旦遇到尚未发现的顶点
            DFS(v,clock);//即从该顶点出发启动依次DFS
    } while (s!=(v=(++v%n)));//按序号检查，故不漏不重
}

template<typename Tv,typename Te>
void Graph<Tv,Te>::DFS(int v,int& clock){
    dTime(v) = ++clock;status(v) = DISCOVERED;
    for(int u = firstNbr(v);-1<u;u = nextNbr(v,u))
        switch (status(u))
        {
        case UNDISCOVERED:
            type(v,u) = TREE;parent(u) = v;DFS(u,clock);
            break;
        case DISCOVERED:
            type(v,u) = BACKWARD;
            break;
        default:
            type(v,u) = (dTime(v) < dTime(u))?FORWARD:CROSS; 
            break;
        } 
    status(v)=VISITED;fTime(v) = ++clock;
}

template<typename Tv,typename Te>
void Graph<Tv,Te>::bcc(int s){
    reset();int clock = 0;int v = s;Stack<int> S;
    do 
        if(UNDISCOVERED == status(v)){
            BCC(v,clock,S);
            S.pop();
        }
    while(s != (v = (++v%n)));
}

#define hca(x) (fTime(x))//利用此处闲置的fTime[]充当hca[]
template<typename Tv,typename Te>
void Graph<Tv,Te>::BCC(int v,int& clock,Stack<int>& S){
    hca(v) = dTime(v) = ++clock;status(v) = DISCOVERED;S.push(v);
    for(int u = firstNbr(v);-1<u;u = nextNbr(v,u))
        switch (status(u))
        {
        case UNDISCOVERED:
            parent(u) = v;type(v,u) = TREE;BCC(u,clock,S);//从顶点u处深入
            if(hca(u) < dTime(v))
                hca(v) = __min(hca(v),hca(u));
            else{
                printf("BCC rooted at %c:",vertex(v));
                Stack<int> temp;
                do{
                    temp.push(S.pop());
                    print(vertex(temp.top()));
                }while(u != temp.top());
                print(vertex(parent(u)));
                while(!temp.empty()) S.push(temp.pop());
                while(u != S.pop());
                printf("\n");
            }
            break;
        case DISCOVERED:
            type(v,u) = BACKWARD;
            if( u != parent(v)) hca(v) = __min(hca(v),dTime(u));
            break;
        
        default:
            type(v,u) = (dTime(v) < dTime(u))?FORWARD:CROSS;
            break;
        }
    status(v) = VISITED;
}
#undef hca

template<typename Tv,typename Te>
void Graph<Tv,Te>::prim(int s){
    reset();priority(s) = 0;
    for(int i = 0;i<n;i++){
        status(s) = VISITED;
        if(-1 != parent(s)) type(parent(s),s) = TREE;//引入当前的s
        for(int j = firstNbr(s);-1 <j;j = nextNbr(s,j))
            if((status(j) == UNDISCOVERED) && (priority(j) > weight(s,j)))
                {priority(j) = weight(s,j);parent(j) = s;}
        for(int shortest = INT_MAX,j=0;j<n;j++)
            if((status(j) == UNDISCOVERED) && (shortest > priority(j)))
            {shortest = priority(j);s = j;}
    }
}


template<typename Tv,typename Te>
void Graph<Tv,Te>::dijkstra(int s)
{
    reset();priority(s) = 0;
    for(int i =0;i<n;i++){
        status (s) = VISITED;
        if(-1 != parent(s)) type(parent(s),s) = TREE;
        for(int j = firstNbr(s);-1 < j;j=nextNbr(s,j))
            if((status(j) == UNDISCOVERED && (priority(j) > priority(s) + weight(s,j))))
            {priority(j) = priority(s) + weight(s,j);parent(j) = s;}
        for(int shortest = INT_MAX,j=0;j<n;j++)
            if((status(j) == UNDISCOVERED) && (shortest > priority(j)))
            { shortest = priority(j); s = j;}
    }
}

template<typename Tv,typename Te>
Stack<Tv>* Graph<Tv,Te>::tSort(int s){
    reset();int clock = 0;int v = s;
    Stack<Tv>* S = new Stack<Tv>;
    do{
        if(UNDISCOVERED == status(v))
            if(!TSort(v,clock,S)){
                print(S);
                while(!S->empty())
                    S->pop();break;
            }
    }while(s != (v = (++v % n)));
    return S;
}

template<typename Tv,typename Te>
bool Graph<Tv,Te>::TSort(int v,int& clock,Stack<Tv>* S){\
    dTime(v) = ++clock;status(v) = DISCOVERED;
    for(int u = firstNbr(v);-1<u;u=nextNbr(v,u))
        switch (status(u))
        {
        case UNDISCOVERED:
            parent(u) = v;type(v,u) = TREE;
            if(!TSort(u,clock,S))
                return false; 
            break;
        case DISCOVERED:
            type(v,u) = BACKWARD;
            return false;
        
        default:
            type(v,u) = (dTime(v) <dTime(u)) ? FORWARD:CROSS;
            break;
        }
    status(v) = VISITED;S->push(vertex(v));
    return true;
}


template<typename Tv,typename Te> 
template<typename PU>//优先级搜索（全图）
void Graph<Tv,Te>::pfs(int s,PU prioUpdater){
    reset();int v = s;
    do
    {
        if(UNDISCOVERED == status(v))
            PFS(v,prioUpdater);
    } while (s!=(v = (++v % n)));
    
}

template<typename Tv,typename Te>
template<typename PU>
void Graph<Tv,Te>::PFS(int s,PU prioUpdater){
    priority(s) = 0;status(s) = VISITED;parent(s) = -1;
    while (1)
    {
        for(int w = firstNbr(s);-1 < w;w = nextNbr(s,w))
            prioUpdater(this,s,w);
        for(int shortest = INT_MAX,w = 0;w < n;w++)
            if(UNDISCOVERED == status(w))
                if(shortest > priority(w))
                    {shortest = priority(w),s = w;}
        if(VISITED == status(s)) break;
        status(s) = VISITED;type(parent(s),s) = TREE; 
    }
    
}

template<typename Tv,typename Te> struct BfsPU{//针对BFS算法的顶点优先级更新器
    virtual void operator() (Graph<Tv,Te>* g,int uk,int v){
        if(g->status(v) == UNDISCOVERED)
            if(g->priority(v) > g->priority(uk) +1 ){
                g->priority(v) = g->priority(uk) +1;
                g->parent(v) = uk;
            }
    }

};

template<typename Tv,typename Te> struct DfsPU{//针对DFS算法的顶点优先级更新器
    virtual void operator() (Graph<Tv,Te>* g,int uk,int v){
        if(g->status(v) == UNDISCOVERED)
            if(g->priority(v) > g->priority(uk) -1){
                g->priority(v) = g->priority(uk)-1;
                g->parent(v) = uk;
                return;
            }
    }
};


template<typename Tv,typename Te>struct DijkstraPU{
    virtual void operator()(Graph<Tv,Te>* g,int uk,int v){
        if(UNDISCOVERED == g->status(v))
            if(g->priority(v) > g->priority(uk) + g->weight(uk,v)){
                g->priority(v) = g->priority(uk) + g->weight(uk,v);
                g->parent(v) = uk;
            }
    }
};

template<typename Tv,typename Te>struct PrimPU{//针对Prim算法的顶点优先级更新器
    virtual void operator() (Graph<Tv,Te>* g,int uk,int v){
        if(UNDISCOVERED == g->status(v))
            if(g->priority(v) > g->weight(uk,v)){
                g->priority(v) = g->weight(uk,v);
                g->parent(v) = uk;
            }
    }
};


#endif//GRAPH