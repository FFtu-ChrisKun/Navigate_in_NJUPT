#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPainter>
#include<QBrush>
#include<QWidget>
#include<QLineEdit>
#include<QPushButton>
#include<QLabel>
#include<QComboBox>
#include<QGridLayout>
#include <QMouseEvent>
#include<QVector>
#include<QPalette>
#include"map.h"
#include"paintmap.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget * = 0);
    void paintEvent(QPaintEvent *);//重画事件
    void CreateToolBar();//创建工具栏
    void InitPoint();//初始化，将各个景点画上
    void setNextPoint(int index);//得到路径上当前点的下一个点
    void AddEdge();//添加学校所有的顶点以及边
    QVector<int> get_Path(int endPos);//以正序得到最短路径
    QVector<int> get_SecondPath(int endPos);//以正序得到第二短路径
    QVector<int> get_ThirdPath(int endPas);//以正序得到第三短路径
    void paintPath(QVector<int> nextPath);//画各种路径的函数
    void mousePressEvent(QMouseEvent *e);//处理鼠标点击事件，用于在地图上选点
    ~MainWindow();
public slots:
        void FindPath();//用于寻找最短路径并在图上显示，响应开始导航按钮和切换路线按钮
        void FindSecondPath();//用于寻找第二短路径，并在图上显示，响应切换路线按钮
        void FindThirdPath();//用于寻找第三短路径，并在图上显示，响应切换路线按钮
        void setStartPoint();//设置路线起点，响应起点下拉列表框和设为起点按钮
        void setEndPoint();//设置路线终点，响应终点下拉列表框和设为终点按钮
        void InitPath();//初始化路线图，用于清空操作
        void Clear();//清空图上已经画的图和点，重新添加删除的边或点
        void DFS();//深度优先遍历，设计一次游园路线，响应我要游园按钮
        void Eulur();//设计一次走完所有路的路线，响应我要走路按钮
        void SetStart();//设置起点，用于地图选点，响应设为起点按钮
        void SetEnd();//设置终点，用于地图选点，响应设为终点按钮
signals:
   void clicked(bool);//鼠标点击操作
private:
    Map *m;//定义map类，用于各用基础函数，包括但不限于DFS和求最短路径的Dijkstra算法
    PaintMap *paintMap;//定义paintMap类，用于画背景地图及地图上点的坐标
    QComboBox *StartComboBox;
    QComboBox *EndComboBox;
    QPushButton *NaviButton;
    QPushButton *ReverseButton;
    QPushButton *ClearButton;
    QPushButton *DFSButton;
    QPushButton *EulurButton;
    QGridLayout *Layout;
    QLabel *StartLabel;
    QGraphicsScene *scene;
    QGraphicsView *view;
    QLabel *EndLabel;
    QPointF StartPos;
    QPointF EndPos;
    QPointF NextPos;
    QLabel *IntroductionLabel;
    QPushButton *SetStartButton;
    QPushButton *SetEndButton;
    QGridLayout *TopLeftLayout;
    QGridLayout *TopRightLayout;
     QWidget *widget;
    int StartNum;
    int EndNum;
    int NextNum;
    int TempNum;//用于暂存使用鼠标得到的点
};
    static int page=0;//用于切换路线按钮选择槽函数
#endif // MAINWINDOW_H
