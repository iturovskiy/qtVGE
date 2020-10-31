#include "treewidget.h"
#include "ui_widget.h"


TreeWidget::TreeWidget(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Widget)
{
     ui->setupUi(this);
     count = 0;
     ui->treeWidget->setColumnCount(1);
     QStringList headers;
     headers << tr("Shapes");
     ui->treeWidget->setHeaderLabels(headers);
     currentItem = nullptr;
     currentColumn = 0;
}


TreeWidget::~TreeWidget()
{
    delete ui;
}


int TreeWidget::treeCount(QTreeWidget *tree, QTreeWidgetItem *parent = nullptr)
{
    tree->expandAll();
    int count = 0;
    int childCount = parent ?  parent->childCount() : tree->topLevelItemCount();
    for (int i = 0; i < childCount; i++) {
        auto item = parent ? parent->child(i) : tree->topLevelItem(i);
        if (item->isExpanded()) {
            count += treeCount(tree, item);
        }
    }
    count += childCount;
    return count;
}


void TreeWidget::DeleteItem (QTreeWidgetItem *currentItem)
{
    QTreeWidgetItem *parent = currentItem->parent();
    int index;
    if (parent) {
        index = parent->indexOfChild(ui->treeWidget->currentItem());
        delete parent->takeChild(index);
    }
    else {
        index = ui->treeWidget->indexOfTopLevelItem(ui->treeWidget->currentItem());
        delete ui->treeWidget->takeTopLevelItem(index);
    }
}


void TreeWidget::InsertItem (QTreeWidgetItem *parent, QString text)
{
    if (parent->isExpanded()==false) {
        parent->setExpanded(true);
    }
    QTreeWidgetItem *newItem = new QTreeWidgetItem(parent, ui->treeWidget->currentItem());
    newItem->setText(currentColumn, std::move(text));
    newItem->setExpanded(true);
}


VGEShape * TreeWidget::search(const QString &name)
{
    for (auto &item : _shapeList) {
        if (name.compare(item->getName()) == 0) {
            return item;
        }
        if (qobject_cast<VGEGroup *>(item)) {
           auto grp = qobject_cast<VGEGroup *>(item);
           for (auto &i : grp->getList()) {
               if (name.compare(i->getName()) == 0) {
                   return i;
               }
           }
        }
    }
    return nullptr;
}


void TreeWidget::showAll(void)
{
     int cnt = treeCount (ui->treeWidget);
     QString str(tr("Всего: ")+QString("%1").arg(cnt));
     setWindowTitle(str);
}


void TreeWidget::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
     currentItem = item;
     currentColumn = column;
     QString name = currentItem->text(0);
     emit selectShape(search(name));
}


void TreeWidget::update(QList<VGEShape *> shapeList)
{
    ui->treeWidget->clear();
    _shapeList = shapeList;
    QTreeWidgetItem *newItem;
    for (auto item : _shapeList) {
        newItem = new QTreeWidgetItem(ui->treeWidget, ui->treeWidget->currentItem());
        newItem->setText(currentColumn, item->getName());
        newItem->setExpanded(true);
        if (qobject_cast<VGEGroup * >(item)) {
            auto grp = qobject_cast<VGEGroup * >(item);
            for (auto &it : grp->getList()) {
                InsertItem(newItem, it->getName());
            }
        }

    }
    currentItem = nullptr;
    showAll();
}


void TreeWidget::on_pushButton_2_clicked()
{
    if (currentItem) {
        QString name = currentItem->text(0);
        emit deleteShape(search(name));
        DeleteItem (currentItem);
        currentItem = nullptr;
    }
    showAll();
}
