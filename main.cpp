#include "vgemainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    try {

        QApplication::setDesktopSettingsAware(true);
        QApplication a(argc, argv);
        VGEMainWindow w;
        w.show();
        return a.exec();

    } catch (std::exception e){
        return 123;
    }
}
