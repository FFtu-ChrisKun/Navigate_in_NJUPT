#include "map.h"
#include<malloc.h>
#include<iostream>
#include<QDebug>
#include<time.h>
#include<stdlib.h>
#define INFTY 2147483647

Map::Map(int nSize)
{
    lg=new LGraph;
    this->lg->n=nSize;
    this->lg->e=0;
    lg->a=new ENode*[nSize*sizeof(ENode*)];
    if(!lg->a)
       ;
    else{
        for(int i=0;i<lg->n;i++) lg->a[i]=NULL;
    }
    for(int i=0;i<Max_Vex;i++)
        for(int j=0;j<Max_Vex;j++)
            isChosen[i][j]=false;
}

bool Map::Exist(int u, int v)
{
    ENode *p;
    if(u<0||v<0||u>lg->n-1||v>lg->n-1||u==v)
        return false;
    p=lg->a[u];
    while(p&&p->adjVex!=v)
        p=p->nextArc;
    if(!p)
        return false;
    else
        return true;
}

int Map::getN()
{
    return lg->n;
}

int Map::getE()
{
    return lg->e;
}

int *Map::getD()
{
    return d;
}

int Map::getPATH(int i)
{
    return path[i];
}

void Map::Dijkstra(int v)
{
        ENode *q;
        if (v<0 || v>lg->n)
            return ;
        int *s = (int *)malloc(sizeof(int)*lg->n);
        int i;
        ENode *p;
        for (i = 0; i < lg->n; i++)								//初始化
        {
            d[i] = INFTY;
            s[i] = 0;
            path[i] = -1;
        }
        p=lg->a[v];
        while (p)												//顶点v为源点
        {
            d[p->adjVex] = p->w;
            if (i != v && d[p->adjVex] < INFTY)
                path[p->adjVex] = v;
            p = p->nextArc;
        }
        s[v] = 1;
        d[v] = 0;
        int k;
        for (i = 1; i <= lg->n-1; i++)
        {
            k = Choose(d, s, lg->n);
            s[k] = 1;											//k加入s中
            q = lg->a[k];
            while (q)											//更新d和path
            {
                if (s[q->adjVex] == 0 && d[k] + q->w < d[q->adjVex])
                {
                    d[q->adjVex] = d[k] + q->w;
                    path[q->adjVex] = k;
                }
                q = q->nextArc;
            }
        }
        free(s);
}

void Map::SecondDijkstra(int u,int v)//删除权值最大的边，重新求最短路径
{
    int k=0;
    ENode *p,*q;
    int t=0;
    int maxEdge=0;
    for(int i=0;i<lg->n;i++) {
        p=lg->a[i];
    while(p){
        if(maxEdge<=p->w&&isChosen[i][p->adjVex]==true&&OutEdge(i)>1&&OutEdge(p->adjVex)>1){
            maxEdge=p->w;
            k=i;
            t=p->adjVex;
        }
        p=p->nextArc;
    }
    }
    Remove(k,t);
    Remove(t,k);
    if (v<0 || v>lg->n)
        return;
    int *s = (int *)malloc(sizeof(int)*lg->n);
    int i;
    for (i = 0; i < lg->n; i++)								//初始化
    {
        Secondd[i] = INFTY;
        s[i] = 0;
        SecondPath[i] = -1;
    }
   p = lg->a[u];
    while (p)												//顶点u为源点
    {
        Secondd[p->adjVex] = p->w;
        if (i != u && Secondd[p->adjVex] < INFTY)
            SecondPath[p->adjVex] = u;
        p = p->nextArc;
    }
    s[u] = 1;
    Secondd[u] = 0;
    for (i = 1; i <= lg->n-1; i++)
    {
        k = Choose(Secondd, s, lg->n);
        s[k] = 1;											//k加入s中
        q = lg->a[k];
        while (q)											//更新d和path
        {
            if (s[q->adjVex] == 0 && Secondd[k] + q->w < Secondd[q->adjVex])
            {
                Secondd[q->adjVex] = Secondd[k] + q->w;
                SecondPath[q->adjVex] = k;
            }
            q = q->nextArc;
        }
    }
    free(s);
    //选择矩阵归零，以备一下次选择
    for(int i=0;i<Max_Vex;i++)
        for(int j=0;j<Max_Vex;j++)
            isChosen[i][j]=false;
}

void Map::ThirdDijkstra(int u, int v)//删除权值最小的边，重新求最短路径
{
    int k=0,t=0;
    ENode *p,*q;
    int minEdge=INFTY;
    for(int i=0;i<lg->n;i++) {
        p=lg->a[i];
    while(p){
        if(minEdge>=p->w&&isChosen[i][p->adjVex]==true&&OutEdge(i)>1&&OutEdge(p->adjVex)>1){
            minEdge=p->w;
            k=i;
            t=p->adjVex;
        }
        p=p->nextArc;
    }
    }
    Remove(k,t);
    Remove(t,k);
    if (v<0 || v>lg->n)
        return;
    int *s = (int *)malloc(sizeof(int)*lg->n);
    int i;
    for (i = 0; i < lg->n; i++)								//初始化
    {
        Thirdd[i] = INFTY;
        s[i] = 0;
        ThirdPath[i] = -1;
    }
   p = lg->a[u];
    while (p)												//顶点u为源点
    {
        Thirdd[p->adjVex] = p->w;
        if (i != u && Thirdd[p->adjVex] < INFTY)
            ThirdPath[p->adjVex] = u;
        p = p->nextArc;
    }
    s[u] = 1;
    Thirdd[u] = 0;
    for (i = 1; i <= lg->n-1; i++)
    {
        k = Choose(Thirdd, s, lg->n);
        s[k] = 1;											//k加入s中
        q = lg->a[k];
        while (q)											//更新d和path
        {
            if (s[q->adjVex] == 0 && Thirdd[k] + q->w < Thirdd[q->adjVex])
            {
                Thirdd[q->adjVex] = Thirdd[k] + q->w;
                ThirdPath[q->adjVex] = k;
            }
            q = q->nextArc;
        }
    }
    free(s);
    //选择矩阵归零，以备一下次选择
    for(int i=0;i<Max_Vex;i++)
        for(int j=0;j<Max_Vex;j++)
            isChosen[i][j]=false;
}

int Map::getSecondPATH(int i)
{
    return SecondPath[i];
}

int Map::getThirdPATH(int i)
{
    return ThirdPath[i];
}

QVector<int> Map::getDFSVisited()
{
    return DFSvisited;
}

QVector<int> Map::getDFSedgeVisited()
{
    return DFSedgevisited;
}

void Map::setIsChosen(int i, int j,bool b)
{
    this->isChosen[i][j]=b;
    this->isChosen[j][i]=b;
}

int Map::Remove(int u, int v)
{
    ENode *p,*q;
    if(u<0||v<0||u>lg->n||v>lg->n||u==v)
        return 0;
    p=lg->a[u];
    q=NULL;
    while(p&&p->adjVex!=v){
        q=p;
        p=p->nextArc;
    }
    if(!p) return 0;
    if(q)
        q->nextArc=p->nextArc;
    else
        lg->a[u]=p->nextArc;
    free(p);
    lg->e--;
    return 1;
}

void Map::DFSGraph(int k)
{
    bool *visited=(bool*)malloc(lg->n*sizeof(bool));
    for(int i=0;i<lg->n;i++)
        visited[i]=false;
        if(!visited[k])
            DFS(k,visited);
        free(visited);
}

Map::~Map()
{

    ENode *p,*q;
    for(int i=0;i<lg->n;i++)
    {
        p=lg->a[i];
        q=p;
        while (p) {
            p=p->nextArc;
            delete q ;
            q=p;
        }
    }
    delete[] lg->a;
}

int Map::Choose(int *d, int *s, int n)
{//选出最小的d[i],i∈V-S
        int i, minpos;
        int min;
        min = INFTY;
        minpos = -1;
        for(i=0;i<n;i++)
            if (d[i] <= min&&s[i] == 0)
            {
                min = d[i];
                minpos = i;
            }
        return minpos;								//选出下标位置
}

int Map::OutEdge(int u)
{
    int count=0;
    ENode *p;
    p=lg->a[u];
    while(p){
        count++;
        p=p->nextArc;
    }
    return count;
}

void Map::ClearDFSvisited()
{
    DFSvisited.clear();
    QVector<int>().swap(DFSvisited);
}

void Map::ClearDFSedgevisited()
{
    DFSedgevisited.clear();
    QVector<int>().swap(DFSedgevisited);
}

void Map::DFS(int v, bool visited[])
{
    ENode *w;
    DFSvisited.push_back(v);
    visited[v]=true;
    for(w=lg->a[v];w;w=w->nextArc)
        if(!visited[w->adjVex])
            DFS(w->adjVex,visited);

}

void Map::DFSEdge(int v)
{
    DFSedgevisited.push_back(v);
    ENode*w;
    int q;
    for(w=lg->a[v];w;w=w->nextArc){
        q=w->adjVex;
        if(Exist(v,q)){
            Remove(v,q);
            Remove(q,v);
            DFSEdge(q);
        }
    }
}

int Map::JudgeEulur()
{
    int count=0;
    for(int i=0;i<Max_Vex;i++){
        if(OutEdge(i)%2==1)
            count++;
    }
    return count;
}

int Map::Insert(int u, int v, int w)
{
    ENode *p;
    if(u<0||v<0||u>lg->n-1||v>lg->n-1||u==v)
        return 0;
    if(Map::Exist(u,v))
        return 0;
    p=new ENode[sizeof(ENode)];
    p->adjVex=v;
    p->w=w;
    p->nextArc=lg->a[u];
    lg->a[u]=p;
    lg->e++;
    return 1;
}
