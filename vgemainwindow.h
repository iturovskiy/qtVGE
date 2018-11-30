#ifndef QTVERMAINWINDOW_H
#define QTVERMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class qtVGEMainWindow;
}

class qtVGEMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit qtVGEMainWindow(QWidget *parent = 0);
    ~qtVGEMainWindow();

private:
    Ui::qtVGEMainWindow *ui;
};

#endif // QTVGEMAINWINDOW_H
