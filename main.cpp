#include "qtvgemainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtVGEMainWindow w;
    w.show();
    return a.exec();
}
