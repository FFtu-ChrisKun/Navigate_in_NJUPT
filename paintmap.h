#ifndef PAINTMAP_H
#define PAINTMAP_H
#include<QWidget>
#include<QPainter>
#include<QPen>
#include<QBrush>
#include<QGraphicsView>
#include<QLineEdit>
#include<QComboBox>
#include<QPushButton>
#include"map.h"

class PaintMap : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PaintMap();
    void readMap();
    void drawBackground(QPainter *painter);
        QPixmap map;
    QPointF qpointF[29]={
        QPointF(410,945),//0-南门
        QPointF(327,905),//1-行政楼
        QPointF(358,845),//2-教1
        QPointF(445,835),//3-圆楼
        QPointF(515,777),//4-南一
        QPointF(358,770),//5-教2
        QPointF(358,695),//6-教3
        QPointF(505,690),//7-梅苑
        QPointF(510,610),//8-兰苑
        QPointF(358,620),//9-教4
        QPointF(400,530),//10-图书馆
        QPointF(480,530),//11-南二
        QPointF(545,530),//12-竹苑
        QPointF(520,475),//13-菊苑
        QPointF(370,440),//14-鼎山
        QPointF(525,430),//15-南操
        QPointF(240,400),//16-学科楼
        QPointF(622,355),//17-东门
        QPointF(500,340),//18-桃苑
        QPointF(354,320),//19-门诊楼
        QPointF(555,315),//20-李苑
        QPointF(550,255),//21-柳苑
        QPointF(505,280),//22-南三
        QPointF(355,280),//23-大活
        QPointF(519,200),//24-桂苑
        QPointF(475,220),//25-荷苑
        QPointF(360,200),//26-北操
        QPointF(195,290),//27-青教
        QPointF(240,180)//28-北门
                        };
        int ChoosePoint(double x, double y);
signals:

public slots:
private:
    double x1,x2,y1,y2;
};

#endif // PAINTMAP_H
