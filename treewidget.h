#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>

#include "vgeshape.h"
#include "vgeline.h"
#include "vgerectangle.h"
#include "vgecircle.h"
#include "vgehypocycloid.h"
#include "vgegroup.h"


namespace Ui {
    class Widget;
}


class TreeWidget : public QWidget {
    Q_OBJECT

public:
    explicit TreeWidget(QWidget *parent = nullptr);
    ~TreeWidget();


signals:
    void deleteShape(VGEShape *);
    void selectShape(VGEShape *);



public slots:
    void update(QList<VGEShape *> shapeList);


private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);
    void on_pushButton_2_clicked();

private:
    int treeCount(QTreeWidget *, QTreeWidgetItem *);
    void DeleteItem (QTreeWidgetItem *currentItem);
    void InsertItem (QTreeWidgetItem *, QString);
    void showAll(void);
    VGEShape * search(QString name);


private:
    QList<VGEShape *> _shapeList;
    Ui::Widget *ui;
    int count;
    QTreeWidgetItem *currentItem;
    int currentColumn;

};

#endif // WIDGET_H
