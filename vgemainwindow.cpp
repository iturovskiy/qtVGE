#include "qtvgemainwindow.h"
#include "ui_qtvgrmainwindow.h"

qtVGEMainWindow::qtVGEMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::qtVGEMainWindow)
{
    ui->setupUi(this);
}

qtVGEMainWindow::~qtVGEMainWindow()
{
    delete ui;
}
