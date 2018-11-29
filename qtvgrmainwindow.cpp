#include "qtvgrmainwindow.h"
#include "ui_qtvgrmainwindow.h"

qtVGRMainWindow::qtVGRMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::qtVGRMainWindow)
{
    ui->setupUi(this);
}

qtVGRMainWindow::~qtVGRMainWindow()
{
    delete ui;
}
