#ifndef QTVERMAINWINDOW_H
#define QTVERMAINWINDOW_H

#include <QMainWindow>
#include "vgedocument.h"
#include "configs.h"

#include <QList>
#include <QtWidgets>
#include <QAction>


class VGEAction;
class VGEMainWindow;


class VGEAction : public QAction {
    Q_OBJECT

public:
    VGEAction(const QIcon &icon, const QString &label, const vge::editorMode mode, QObject *parent=nullptr);


public slots:
    void enable(bool state);


signals:
    void toggled(vge::editorMode mode);


private:
    vge::editorMode _mode;


private slots:
    void slotToggled(bool state);

};


typedef QPair<vge::editorMode, QString> ToolPair;


class VGEMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VGEMainWindow(QWidget *parent = nullptr);
    ~VGEMainWindow() override = default;


public slots:
    void coordUpdated(QPoint coord);
    void updateMode(vge::editorMode);
    void resetToSelection();
    void putMessage(QString text, bool isError);
    void openNewWindow(QWidget * window);


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void initToolbar(QToolBar*, QList<ToolPair>, Qt::ToolBarArea);
    void initToolsList();


private:
    VGEDocument *_document;
    QScrollArea *_scrollArea;
    QToolBar *_leftToolBar;
    //QTreeView *_rightTree;
    QLabel *_coordXLabel;
    QLabel *_coordYLabel;
    QLabel *_toolLabel;
    QLabel *_messageLabel;
    QList<ToolPair> _toolList;
    QActionGroup* _toolActionGroup;
    QAction* _selectAction = nullptr;

};

#endif // QTVGEMAINWINDOW_H
