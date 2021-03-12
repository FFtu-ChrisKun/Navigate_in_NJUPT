#include "mainwindow.h"
#include<windows.h>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include <QGridLayout>
#include<QToolBar>
#include<QWidget>
#include  <QGraphicsPathItem>
#include<QGraphicsScene>
#include<QGraphicsItem>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    TempNum=-1;
    m=new Map(29);
    paintMap=new PaintMap;
    scene=new QGraphicsScene;
    StartPos=paintMap->qpointF[0];
    EndPos=paintMap->qpointF[0];
    InitPoint();
    InitPath();
    CreateToolBar();
    connect(NaviButton, SIGNAL(clicked(bool)),this,SLOT(FindPath()));
    connect(ReverseButton,&QPushButton::released,
                [=]()mutable
                {
                       if(page%3==1)
                       {
                           emit FindSecondPath();
                           page++;
                       }
                       else if(page%3==2)
                       {
                           emit FindThirdPath();
                           page++;
                       }
                       else{
                           emit FindPath();
                       }
                }
                );
    connect(ClearButton,SIGNAL(clicked(bool)),this,SLOT(InitPath()));
    connect(DFSButton,SIGNAL(clicked(bool)),this,SLOT(DFS()));
    connect(EulurButton,SIGNAL(clicked(bool)),this,SLOT(Eulur()));
    IntroductionLabel = new QLabel(tr("南京邮电大学仙林校区平面图"));
    SetStartButton=new QPushButton(tr("设为起点"));
    SetEndButton=new QPushButton(tr("设为终点"));
    TopLeftLayout=new QGridLayout;
    TopRightLayout=new QGridLayout;
    TopLeftLayout->addWidget(IntroductionLabel,0,0);
    TopRightLayout->addWidget(SetStartButton,0,1);
    TopRightLayout->addWidget(SetEndButton,1,1);
    TopLeftLayout->setSizeConstraint(QLayout::SetFixedSize);
    QFont font("楷体",12,75);
    IntroductionLabel->setFont(font);
    widget=new QWidget();
    this->setCentralWidget(widget);
     QGridLayout *mainLayout =new QGridLayout(this);
    widget->setLayout(mainLayout);
    mainLayout->setMargin(15);
    mainLayout->addLayout(TopLeftLayout,0,0);
    mainLayout->addLayout(TopRightLayout,0,1);
    mainLayout->setColumnMinimumWidth(0,450);
    mainLayout->setColumnStretch(0,1);
    mainLayout->setColumnStretch(3,1);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    connect(SetStartButton,SIGNAL(clicked(bool)),this,SLOT(SetStart()));
    connect(SetEndButton,SIGNAL(clicked(bool)),this,SLOT(SetEnd()));
    setWindowIcon(QIcon("navi.ico"));
    setWindowTitle(tr("阿土导航，为您护航！"));
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paintMap->drawBackground(&paint);//画背景地图
    //画图例
    paint.setBrush(Qt::red);
    paint.drawEllipse(QPointF(625,55),5,5);
    paint.drawText(QPointF(635,60),tr("起点"));
    paint.setBrush(Qt::blue);
    paint.drawEllipse(QPointF(625,75),5,5);
    paint.drawText(QPointF(635,80),tr("终点"));
    QPen pen;
    pen.setWidth(3.5);
    pen.setColor(QColor(50,205,50));
    paint.setPen(pen);
    paint.drawLine(QPointF(600,95),QPointF(625,95));
    pen.setColor(Qt::black);
    paint.setPen(pen);
    paint.drawText(QPointF(635,100),tr("推荐路线"));
    pen.setWidth(3.5);
    pen.setColor(QColor(173,255,47));
    paint.setPen(pen);
    paint.drawLine(QPointF(600,115),QPointF(625,115));
    pen.setColor(Qt::black);
    paint.setPen(pen);
    paint.drawText(QPointF(635,120),tr("备选路线1"));
    pen.setWidth(3.5);
    pen.setColor(QColor(144,238,144));
    paint.setPen(pen);
    paint.drawLine(QPointF(600,135),QPointF(625,135));
    pen.setColor(Qt::black);
    paint.setPen(pen);
    paint.drawText(QPointF(635,140),tr("备选路线2"));
}

void MainWindow::FindPath()
{
    page=0;
    Clear();
    AddEdge();
    QPainter paint(&paintMap->map);
    m->Dijkstra(StartComboBox->currentIndex());
    QVector<int> nextPath;
    nextPath=get_Path(EndComboBox->currentIndex());
    paint.setBrush(Qt::red);
    paint.drawEllipse(paintMap->qpointF[nextPath[0]]-QPointF(50,150),5,5);
    paint.setBrush(Qt::blue);
    paint.drawEllipse(paintMap->qpointF[nextPath[nextPath.size()-1]]-QPointF(50,150),5,5);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(50,205,50));
    paint.setPen(pen);
    for(int i=1;i<nextPath.size();i++){
       qDebug()<<nextPath[i]<<",";
       m->setIsChosen(nextPath[i-1],nextPath[i],true);
    }
    //开始画路径
    QPainterPath path;
    path.moveTo(StartPos.x()-50,StartPos.y()-150);
    for(int i=1;i<nextPath.size();i++){
        path.lineTo(paintMap->qpointF[nextPath[i]].x()-50,paintMap->qpointF[nextPath[i]].y()-150);
        paint.drawPath(path);
        path.moveTo(paintMap->qpointF[nextPath[i]].x()-50,paintMap->qpointF[nextPath[i]].y()-150);
    }
    this->update();//调用PaintEvent重画事件
    page++;
}

void MainWindow::FindSecondPath()
{
     Clear();
    QPainter paint(&paintMap->map);
    m->SecondDijkstra(StartComboBox->currentIndex(),EndComboBox->currentIndex());
    QVector<int> nextPath;
    nextPath=get_SecondPath(EndComboBox->currentIndex());
    paint.setBrush(Qt::red);
    paint.drawEllipse(paintMap->qpointF[nextPath[0]]-QPointF(50,150),5,5);
    paint.setBrush(Qt::blue);
    paint.drawEllipse(paintMap->qpointF[nextPath[nextPath.size()-1]]-QPointF(50,150),5,5);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(173,255,47));
    paint.setPen(pen);
    for(int i=1;i<nextPath.size();i++){
       qDebug()<<nextPath[i]<<",";
       m->setIsChosen(nextPath[i-1],nextPath[i],true);
    }
    QPainterPath path;
    path.moveTo(StartPos.x()-50,StartPos.y()-150);
    for(int i=1;i<nextPath.size();i++){
        path.lineTo(paintMap->qpointF[nextPath[i]].x()-50,paintMap->qpointF[nextPath[i]].y()-150);
        paint.drawPath(path);
        path.moveTo(paintMap->qpointF[nextPath[i]].x()-50,paintMap->qpointF[nextPath[i]].y()-150);
    }
    this->update();
}

void MainWindow::FindThirdPath()
{
    Clear();
    QPainter paint(&paintMap->map);
    m->SecondDijkstra(StartComboBox->currentIndex(),EndComboBox->currentIndex());
    QVector<int> nextPath;
    nextPath=get_SecondPath(EndComboBox->currentIndex());
    paint.setBrush(Qt::red);
    paint.drawEllipse(paintMap->qpointF[nextPath[0]]-QPointF(50,150),5,5);
    paint.setBrush(Qt::blue);
    paint.drawEllipse(paintMap->qpointF[nextPath[nextPath.size()-1]]-QPointF(50,150),5,5);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(144,238,144));
    paint.setPen(pen);
    for(int i=1;i<nextPath.size();i++){
       qDebug()<<nextPath[i]<<",";
    }
    QPainterPath path;
    path.moveTo(StartPos.x()-50,StartPos.y()-150);
    for(int i=1;i<nextPath.size();i++){
        path.lineTo(paintMap->qpointF[nextPath[i]].x()-50,paintMap->qpointF[nextPath[i]].y()-150);
        paint.drawPath(path);
        path.moveTo(paintMap->qpointF[nextPath[i]].x()-50,paintMap->qpointF[nextPath[i]].y()-150);
    }
    this->update();
    AddEdge();
}

void MainWindow::AddEdge()
{//从文件中读取数据，并插入表中

    QFile file("MapData.txt");
    QString str;
    QStringList temp;
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        str=in.readLine(0);
        while(str!=""){
           temp= str.split(",");
           if(!m->Exist(atoi(temp[0].toLatin1().data()),atoi(temp[1].toLatin1().data())))
                   m->Insert(atoi(temp[0].toLatin1().data()),atoi(temp[1].toLatin1().data()),atoi(temp[2].toLatin1().data()));
           str=in.readLine(0);
        }
    }
    file.close();
}

void MainWindow::CreateToolBar()
{
    StartComboBox=new QComboBox;
    EndComboBox=new QComboBox;
    NaviButton=new QPushButton(tr("开始导航"));
    ReverseButton=new QPushButton(tr("切换路线"));
    ClearButton=new QPushButton(tr("全部清除"));
    DFSButton=new QPushButton(tr("我要游园"));
    EulurButton=new QPushButton(tr("我想走路"));
    QToolBar *ToolBar=addToolBar("Tool");
    StartLabel = new QLabel(tr("起点: "));EndLabel=new QLabel(tr("终点:"));
    StartComboBox->addItem(tr("南门"));EndComboBox->addItem(tr("南门"));//0-南门
    StartComboBox->addItem(tr("行政楼"));EndComboBox->addItem(tr("行政楼"));//1-行政楼
    StartComboBox->addItem(tr("教1"));EndComboBox->addItem(tr("教1"));//2-教1
    StartComboBox->addItem(tr("圆楼"));EndComboBox->addItem(tr("圆楼"));//3-圆楼
    StartComboBox->addItem(tr("南一"));EndComboBox->addItem(tr("南一"));//4-南一
    StartComboBox->addItem(tr("教2"));EndComboBox->addItem(tr("教2"));//5-教2
    StartComboBox->addItem(tr("教3"));EndComboBox->addItem(tr("教3"));//6-教3
    StartComboBox->addItem(tr("梅苑"));EndComboBox->addItem(tr("梅苑"));//7-梅苑
    StartComboBox->addItem(tr("兰苑"));EndComboBox->addItem(tr("兰苑"));//8-兰苑
    StartComboBox->addItem(tr("教4"));EndComboBox->addItem(tr("教4"));//9-教4
    StartComboBox->addItem(tr("图书馆"));EndComboBox->addItem(tr("图书馆"));//10-图书馆
    StartComboBox->addItem(tr("南二"));EndComboBox->addItem(tr("南二"));//11-南二
    StartComboBox->addItem(tr("竹苑"));EndComboBox->addItem(tr("竹苑"));//12-竹苑
    StartComboBox->addItem(tr("菊苑"));EndComboBox->addItem(tr("菊苑"));//13-菊苑
    StartComboBox->addItem(tr("鼎山"));EndComboBox->addItem(tr("鼎山"));//14-鼎山
    StartComboBox->addItem(tr("南操"));EndComboBox->addItem(tr("南操"));//15-南操
    StartComboBox->addItem(tr("学科楼"));EndComboBox->addItem(tr("学科楼"));//16-学科楼
    StartComboBox->addItem(tr("东门"));EndComboBox->addItem(tr("东门"));//17-东门
    StartComboBox->addItem(tr("桃苑"));EndComboBox->addItem(tr("桃苑"));//18-桃苑
    StartComboBox->addItem(tr("门诊楼"));EndComboBox->addItem(tr("门诊楼"));//19-门诊楼
    StartComboBox->addItem(tr("李苑"));EndComboBox->addItem(tr("李苑"));//20-李苑
    StartComboBox->addItem(tr("柳苑"));EndComboBox->addItem(tr("柳苑"));//21-柳苑
    StartComboBox->addItem(tr("南三"));EndComboBox->addItem(tr("南三"));//22-南三
    StartComboBox->addItem(tr("大活"));EndComboBox->addItem(tr("大活"));//23-大活
    StartComboBox->addItem(tr("桂苑"));EndComboBox->addItem(tr("桂苑"));//24-桂苑
    StartComboBox->addItem(tr("荷苑"));EndComboBox->addItem(tr("荷苑"));//25-荷苑
    StartComboBox->addItem(tr("北操"));EndComboBox->addItem(tr("北操"));//26-北操
    StartComboBox->addItem(tr("青教"));EndComboBox->addItem(tr("青教"));//27-青教
    StartComboBox->addItem(tr("北门"));EndComboBox->addItem(tr("北门"));//28-北门

    connect(StartComboBox,SIGNAL(activated(int)),this,SLOT(setStartPoint()));
    connect(EndComboBox,SIGNAL(activated(int)),this,SLOT(setEndPoint()));

    ToolBar->addWidget (StartLabel);
    ToolBar->addWidget (StartComboBox);
    ToolBar->addSeparator ();
    ToolBar->addWidget (EndLabel);
    ToolBar->addWidget (EndComboBox);
    ToolBar->addSeparator();
    ToolBar->addWidget(NaviButton);
    ToolBar->addSeparator();
    ToolBar->addWidget(ReverseButton);
    ToolBar->addSeparator();
    ToolBar->addWidget(ClearButton);
    ToolBar->addSeparator();
    ToolBar->addWidget(DFSButton);
    ToolBar->addSeparator();
    ToolBar->addWidget(EulurButton);
}

void MainWindow::InitPoint()
{
    QPainter paint(&paintMap->map);
    paint.setBrush(QColor(0, 255, 255));
    for(int i=0;i<m->getN();i++)
        paint.drawEllipse(paintMap->qpointF[i]-QPointF(50,150),5,5);
}

void MainWindow::setStartPoint()
{
    StartNum=StartComboBox->currentIndex();
    StartPos=paintMap->qpointF[StartComboBox->currentIndex()];
    page=0;
}

void MainWindow::setNextPoint(int index)
{
    NextNum=index;
    NextPos=paintMap->qpointF[index];
}

QVector<int> MainWindow::get_Path(int endPos)
{
    QVector<int> path;
    for(;endPos!=-1;endPos=m->getPATH(endPos)){
        path.push_back(endPos);
    }
    std::reverse(path.begin(),path.end());
    return path;
}

QVector<int> MainWindow::get_SecondPath(int endPos)
{
    QVector<int> path;
    for(;endPos!=-1;endPos=m->getSecondPATH(endPos)){
        path.push_back(endPos);
    }
    std::reverse(path.begin(),path.end());
    return path;
}

QVector<int> MainWindow::get_ThirdPath(int endPos)
{
    QVector<int> path;
    for(;endPos!=-1;endPos=m->getThirdPATH(endPos)){
        path.push_back(endPos);
    }
    std::reverse(path.begin(),path.end());
    return path;
}

void MainWindow::setEndPoint()
{
    EndNum=EndComboBox->currentIndex();
    EndPos=paintMap->qpointF[EndComboBox->currentIndex()];
    page=0;
}

void MainWindow::InitPath()
{
    AddEdge();
    Clear();
}

void MainWindow::Clear()
{
    InitPoint();
    QPainter paint(&paintMap->map);
    QPen pen;
    QPainterPath path;
    pen.setWidth(3.5);
    pen.setColor(QColor(207,207,207));
    paint.setPen(pen);
    for(int i=0;i<m->getN();i++){
        path.moveTo(paintMap->qpointF[i]-QPointF(50,150));
        for(int j=0;j<m->getN();j++){
            if(m->Exist(i,j)){
                path.lineTo(paintMap->qpointF[j]-QPointF(50,150));
                paint.drawPath(path);
                path.moveTo(paintMap->qpointF[i]-QPointF(50,150));
            }
        }
    }
    this->repaint();
}

void MainWindow::DFS()
{
    InitPath();
    int j=0,i=0;
    QVector<int> nextPath;
     for(i=0;i<m->getN();i++){
        m->DFSGraph(i);
        nextPath=m->getDFSVisited();
        for(j=1;j<nextPath.size();j++){
            if(!m->Exist(nextPath[j-1],nextPath[j])){
                break;
        }
     }
        if(j!=nextPath.size()){
            nextPath.clear();
            QVector<int>().swap(nextPath);
            m->ClearDFSvisited();
        }
        else
            break;
     }
    if(i==m->getN()){
        for(int k=0;k<m->getN();k++)
            for(int l=0;l<m->getN();l++)
                if(m->Exist(k,l)){
                    m->Remove(k,l);
                }
        AddEdge();
        m->DFSGraph(1);
        nextPath=m->getDFSVisited();
    }
     paintPath(nextPath);
     m->ClearDFSvisited();
     page=0;
}

void MainWindow::Eulur()
{
    InitPath();
    int count=m->JudgeEulur();
    int i;
        m->ClearDFSvisited();
        if(count==0){
            for (i = 0; i < m->getN(); i++)
                {
                    if (m->OutEdge(i) % 2 == 0)
                        break;
                }
            m->DFSEdge(i);
            QVector<int> nextPath;
            nextPath=m->getDFSedgeVisited();
            paintPath(nextPath);
            AddEdge();
            m->ClearDFSedgevisited();
        }
        else if(count==2){
            for (i = 0; i < m->getN(); i++)
            {
                  if (m->OutEdge(i)% 2 == 1)
                       break;
             }
             m->DFSEdge(i);
            QVector<int> nextPath;
            nextPath=m->getDFSedgeVisited();
            paintPath(nextPath);
            AddEdge();
            m->ClearDFSedgevisited();
        }
        else{
            QMessageBox:: StandardButton message= QMessageBox::information(NULL,"提示", "Sorry,您不能走完所有的路！\n"
                                                                                  "点击Yes将为您推荐游园路线。",QMessageBox::Yes|QMessageBox::No);
            if(message==QMessageBox::Yes){
                emit DFS();
            }
        }
        page=0;
}

void MainWindow::SetStart()
{
    if(TempNum==-1||TempNum>=Max_Vex)
        QMessageBox::information(NULL,"提示", "请选择一个起点\n",QMessageBox::Yes);
    else{
        StartComboBox->setCurrentIndex(TempNum);
        setStartPoint();
        page=0;
    }
}

void MainWindow::SetEnd()
{
    if(TempNum==-1||TempNum>=Max_Vex)
        QMessageBox::information(NULL,"提示", "请选择一个终点\n",QMessageBox::Yes);
    else{
        EndComboBox->setCurrentIndex(TempNum);
        setEndPoint();
        page=0;
    }
}

void MainWindow::paintPath(QVector<int> nextPath)
{
   QPainter paint(&paintMap->map);
    paint.setBrush(Qt::red);
    paint.drawEllipse(paintMap->qpointF[nextPath[0]]-QPointF(50,150),5,5);
    paint.setBrush(Qt::blue);
    paint.drawEllipse(paintMap->qpointF[nextPath[nextPath.size()-1]]-QPointF(50,150),5,5);
    QPen pen;
    pen.setWidth(3.5);
    pen.setColor(QColor(50,205,50));
    paint.setPen(pen);
    for(int i=0;i<nextPath.size();i++){
       qDebug()<<nextPath[i]<<",";
    }
    QPainterPath path;
    path.moveTo(paintMap->qpointF[nextPath[0]].x()-50,paintMap->qpointF[nextPath[0]].y()-150);
    for(int i=1;i<nextPath.size();i++){
        path.lineTo(paintMap->qpointF[nextPath[i]].x()-50,paintMap->qpointF[nextPath[i]].y()-150);
        paint.drawPath(path);
        path.moveTo(paintMap->qpointF[nextPath[i]].x()-50,paintMap->qpointF[nextPath[i]].y()-150);
    }
    this->update();
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    this->setMouseTracking(true);//用于监测鼠标位置
    double x,y;
    x=e->x();
    y=e->y();
    if(e->button()==Qt::LeftButton){
        int k=paintMap->ChoosePoint(x,y);
        TempNum=k;
        switch (k) {
        case 0:IntroductionLabel->setText(tr("南京邮电大学南大门"));break;
        case 1:IntroductionLabel->setText(tr("南京邮电大学行政楼"));break;
        case 2:IntroductionLabel->setText(tr("南京邮电大学教1"));break;
        case 3:IntroductionLabel->setText(tr("南京邮电大学圆楼"));break;
        case 4:IntroductionLabel->setText(tr("南京邮电大学一食堂"));break;
        case 5:IntroductionLabel->setText(tr("南京邮电大学教2"));break;
        case 6:IntroductionLabel->setText(tr("南京邮电大学教3"));break;
        case 7:IntroductionLabel->setText(tr("南京邮电大学梅苑学生宿舍"));break;
        case 8:IntroductionLabel->setText(tr("南京邮电大学兰苑学生宿舍"));break;
        case 9:IntroductionLabel->setText(tr("南京邮电大学教4"));break;
        case 10:IntroductionLabel->setText(tr("南京邮电大学图书馆"));break;
        case 11:IntroductionLabel->setText(tr("南京邮电大学二食堂"));break;
        case 12:IntroductionLabel->setText(tr("南京邮电大学竹苑学生宿舍"));break;
        case 13:IntroductionLabel->setText(tr("南京邮电大学菊苑学生宿舍"));break;
        case 14:IntroductionLabel->setText(tr("鼎山"));break;
        case 15:IntroductionLabel->setText(tr("南京邮电大学南运动场"));break;
        case 16:IntroductionLabel->setText(tr("南京邮电大学学科楼组"));break;
        case 17:IntroductionLabel->setText(tr("南京邮电大学东门"));break;
        case 18:IntroductionLabel->setText(tr("南京邮电大学桃苑学生宿舍"));break;
        case 19:IntroductionLabel->setText(tr("南京邮电大学门诊楼"));break;
        case 20:IntroductionLabel->setText(tr("南京邮电大学李苑学生宿舍"));break;
        case 21:IntroductionLabel->setText(tr("南京邮电大学柳苑学生宿舍"));break;
        case 22:IntroductionLabel->setText(tr("南京邮电大学三食堂"));break;
        case 23:IntroductionLabel->setText(tr("南京邮电大学大学生活动中心"));break;
        case 24:IntroductionLabel->setText(tr("南京邮电大学桂苑学生宿舍"));break;
        case 25:IntroductionLabel->setText(tr("南京邮电大学荷苑学生宿舍"));break;
        case 26:IntroductionLabel->setText(tr("南京邮电大学北运动场"));break;
        case 27:IntroductionLabel->setText(tr("南京邮电大学青年教师公寓"));break;
        case 28:IntroductionLabel->setText(tr("南京邮电大学北大门"));break;
        default:
            break;
        }
    }
}

MainWindow::~MainWindow()
{

}

