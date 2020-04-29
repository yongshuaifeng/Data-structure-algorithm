class Fib{//Fibonacci数列类
private:
    int f,g;
public:
    Fib(int n)//初始化为不小于n地最小Fibonacci项
    {f =1;g=0;while(g<n) next();}
    int get(){return g;}
    int next(){g+=f;f = g-f;return g;}
    int prev(){f=g-f;g-=f;return g;}
};