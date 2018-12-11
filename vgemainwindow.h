#ifndef QTVERMAINWINDOW_H
#define QTVERMAINWINDOW_H

#include <QMainWindow>
#include "vgedocument.h"

#include <QList>
#include <QtWidgets>

class VGEMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VGEMainWindow(QWidget *parent = nullptr);
    ~VGEMainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void open();
    void save();


private:
    QToolBar *_leftToolBar;
    QLabel *_coordX;
    QLabel *_coordY;
    VGEDocument *_document;
};

#endif // QTVGEMAINWINDOW_H
