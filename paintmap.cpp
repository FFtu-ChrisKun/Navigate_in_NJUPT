#include "paintmap.h"
#include<QPainter>
#include<QFile>
#include<QTextStream>
#include<QGraphicsScene>
#include<math.h>
#include<QDebug>


PaintMap::PaintMap()
{
    readMap();
    int width=map.width();
    int height =map.height();
    QGraphicsScene *scene=new QGraphicsScene(this);
    //限定显示大小
    scene->setSceneRect(50,150,width,height);
    setScene(scene);
    setCacheMode(CacheBackground);
}
void PaintMap::readMap()
{
    QString mapName;
    QFile mapFile("maps.txt");
    int ok=mapFile.open(QIODevice::ReadOnly);
    if(ok)
    {
        QTextStream ts(&mapFile);
        if(!ts.atEnd()){
            ts>>mapName;
            ts>>x1>>y1>>x2>>y2;
        }
    }
    map.load(mapName);
}

void PaintMap::drawBackground(QPainter *painter)
{
    painter->drawPixmap(int(sceneRect().left()),int(sceneRect().top()),map);
}

int PaintMap::ChoosePoint(double x,double y)
{
    int i;
    for(i=0;i<Max_Vex;i++){
        if(qpointF[i].x()>x-15&&qpointF[i].x()<x+15&&qpointF[i].y()>y-15&&qpointF[i].y()<y+15)
            break;
    }
    return i;
}
