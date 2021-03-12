#include "mainwindow.h"
#include <QApplication>
#include "paintmap.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(770,980);
    w.show();

    return a.exec();
}
