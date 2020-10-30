#include "gui/vgemainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    try {

        QApplication::setDesktopSettingsAware(true);
        QApplication a(argc, argv);
        VGEMainWindow w;
        w.show();
        int retCode = a.exec();
        qDebug() << "Exit";
        return retCode;

    } catch (std::exception &e){
        qDebug() << e.what();
        return 1;
    }
}
