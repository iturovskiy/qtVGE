#ifndef QTVGRMAINWINDOW_H
#define QTVGRMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class qtVGRMainWindow;
}

class qtVGRMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit qtVGRMainWindow(QWidget *parent = 0);
    ~qtVGRMainWindow();

private:
    Ui::qtVGRMainWindow *ui;
};

#endif // QTVGRMAINWINDOW_H
