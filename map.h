#ifndef MAP_H
#define MAP_H
#include<QWidget>
#include<QPointF>
#include<QVector>
#define Max_Vex 29

typedef struct  ENode{
    int adjVex;//任意顶点u相邻接的顶点
    int w;//边的权值
    struct ENode* nextArc;//指向下一个边结点
}ENode;
typedef struct{
    int n;//图的当前顶点数
    int e;//图的当前边数
    ENode **a;//指向一维指针的数组
}LGraph;

class Map:public QWidget
{
    Q_OBJECT
public:
    Map(int nSize);
    int Insert(int u,int v,int w);//插入边u  v
    bool Exist(int u,int v);//判断u   v是否存在
    int getN();//返回点的个数
    int getE();//返回边的条数
    int *getD();//返回最短路径矩阵
    int getPATH(int i);//返回路径结点
    int getSecondPATH(int i);//返回第二短路径结点
    int getThirdPATH(int i);//返回第三短路径结点
    QVector<int> getDFSVisited();//返回游园路径
    QVector<int> getDFSedgeVisited();//返回走路的路径
    void Dijkstra(int v);//dijkstra算法求最短路径
    void SecondDijkstra(int u, int v);//dijkstra算法求第二路径
    void ThirdDijkstra(int u,int v);//dijkstra算法求第三路径
    void setIsChosen(int i, int j, bool b);//i j之间的边被选中则b为true
    int Remove(int u,int v);//删除u v之间的边
    void DFSGraph(int k);//深度优先遍历的调用函数
    int JudgeEulur();//判断欧拉图
    void DFSEdge(int v);//深度优先遍历边
    int OutEdge(int u);//求节点的度
    void ClearDFSvisited();//清空DFSvisited
    void ClearDFSedgevisited();//清空DFSedgevisited
    ~Map();
private:
    int Choose(int *d,int *s,int n);//用于dijsktra算法，选择权值最小的边
    void DFS(int v, bool visited[]);//图的深度优先遍历的递归函数
    bool isChosen[Max_Vex][Max_Vex];//用于备选路线中判断边是否已经被选
    int d[Max_Vex];
    int path[Max_Vex];
    int Secondd[Max_Vex];
    int SecondPath[Max_Vex];
    int Thirdd[Max_Vex];
    int ThirdPath[Max_Vex];
    QVector<int> DFSvisited;
    QVector<int> DFSedgevisited;
    LGraph *lg;
};

#endif // MAP_H
