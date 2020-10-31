#include "gui/vgemainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    int retCode = 1;
    try {
        QApplication::setDesktopSettingsAware(true);
        QApplication a(argc, argv);
        VGEMainWindow w;
        w.show();
        retCode = a.exec();
        qDebug() << "Exit";

    } catch (std::exception &e){
        qDebug() << e.what();
    }
    return retCode;
}
