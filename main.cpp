#include "qtvgrmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qtVGRMainWindow w;
    w.show();
    return a.exec();
}
