#include "vgemainwindow.h"

// Action
VGEAction::VGEAction(const QIcon& icon, const QString& label, const vge::editorMode mode, QObject* parent):
    QAction(icon, label, parent),
    _mode(mode)
{
    connect(this, &QAction::toggled, this, &VGEAction::slotToggled);
}


void VGEAction::slotToggled(bool state) {
    if (state) {
        emit toggled(_mode);
    }
}


void VGEAction::enable(bool state) {
    QAction::setEnabled(state);
}


QString msgLabelText(const QString& str) {
    QString labelText = "<P><b><i><FONT COLOR='#ff0000' FONT SIZE = 4>";
    labelText .append(str);
    labelText .append("</i></b></P></br>");
    return labelText;
}

// main window
VGEMainWindow::VGEMainWindow(QWidget *parent)
    : QMainWindow(parent),
      _document(new VGEDocument(this)),
      _scrollArea(new QScrollArea(this)),
      _leftToolBar(new QToolBar(this)),
      _widget(new TreeWidget(nullptr)),
      _coordXLabel(new QLabel(this)),
      _coordYLabel(new QLabel(this)),
      _toolLabel(new QLabel(this)),
      _messageLabel(new QLabel(this)),
      _toolActionGroup(new QActionGroup(this))
{

    initToolsList();
    initToolbar(_leftToolBar, _toolList, (Qt::LeftToolBarArea));

    _scrollArea->setBackgroundRole(QPalette::Dark);
    _scrollArea->setWidget(_document);

    _scrollArea->setVisible(true);
    setCentralWidget(_scrollArea);

    _coordXLabel->setFont(QFont("Monospace"));
    _coordYLabel->setFont(QFont("Monospace"));
    _toolLabel->setFont(QFont("Monospace"));
    _coordXLabel->setText(QString("X:").append(QString::number(0, 'd', 5)));
    _coordYLabel->setText(QString("Y:").append(QString::number(0, 'd', 5)));

    _messageLabel->setStyleSheet("font-weight: bold; color: red");
    _messageLabel->setText("");
    statusBar()->addWidget(_coordXLabel);
    statusBar()->addWidget(_coordYLabel);
    statusBar()->addWidget(_toolLabel);
    statusBar()->addWidget(_messageLabel);

    connect(_document, &VGEDocument::updateCoord, this, &VGEMainWindow::coordUpdate);
    connect(_document, &VGEDocument::switchToSelection, this, &VGEMainWindow::resetToSelection);
    connect(_document, &VGEDocument::sendMsgToUI, this, &VGEMainWindow::putMessage);
    connect(_document, &VGEDocument::showSetupWindow, this, &VGEMainWindow::openNewWindow);

    connect(_document, &VGEDocument::updateTree, _widget, &TreeWidget::update);
    connect(_widget, &TreeWidget::deleteShape, _document, &VGEDocument::deleteShape);
    connect(_widget, &TreeWidget::selectShape, _document, &VGEDocument::selectShape);

    resetToSelection();
}


void VGEMainWindow::coordUpdate(QPoint coord) {
    _coordXLabel->setText(QString().sprintf("%s: %4d", "X", coord.x()));
    _coordYLabel->setText(QString().sprintf("%s: %4d", "Y", coord.y()));
}


void VGEMainWindow::initToolsList() {
    _toolList.append(ToolPair(vge::DrawLine, "Line"));
    _toolList.append(ToolPair(vge::DrawRectangle, "Rectangle"));
    _toolList.append(ToolPair(vge::DrawCircle, "Circle"));
    _toolList.append(ToolPair(vge::DrawHypocycloid, "Hypocycloid"));

    _toolList.append(ToolPair(vge::SelectShape, "Select"));
    _toolList.append(ToolPair(vge::DeleteShape, "Delete shape"));
    _toolList.append(ToolPair(vge::SetUp, "Setup shape"));
    _toolList.append(ToolPair(vge::Move, "Move"));

    _toolList.append(ToolPair(vge::MakeTangent, "Make tangent"));
    _toolList.append(ToolPair(vge::Clipping, "Clipping"));
    _toolList.append(ToolPair(vge::MakeGroup, "Make group"));

}


void VGEMainWindow::initToolbar(QToolBar * toolBar, QList<ToolPair> elements, Qt::ToolBarArea area) {
    int counter = 0;
    for (ToolPair mode : elements){
        VGEAction* act = new VGEAction(QPixmap(":/" + mode.second + ".svg"), mode.second, mode.first);
        act->setCheckable(true);
        _toolActionGroup->addAction(act);
        toolBar->addAction(act);
        if (counter == 3 || counter == 7) {
            toolBar->addSeparator();
        }

        QObject::connect(act, &VGEAction::toggled, this, &VGEMainWindow::modeUpdate);
        QObject::connect(act, &VGEAction::toggled, _document, &VGEDocument::setEditorMode);
        if (mode.first == vge::SelectShape){
            act->setChecked(true);
            _selectAction = act;
        }
        if (counter > 5 && counter < 9) {
            act->setEnabled(false);
            QObject::connect(_document, &VGEDocument::shapeSelected, act, &VGEAction::enable);
        }
        counter++;
    }
    _toolActionGroup->setExclusive(true);
    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->addWidget(_widget);
    addToolBar(area, _leftToolBar);
}


void VGEMainWindow::modeUpdate(vge::editorMode mode) {
    QString name("None");
    for (ToolPair pair : _toolList){
        if (pair.first == mode){
            name = pair.second;
        }
    }
    _toolLabel->setText(name.rightJustified(20, ' '));
}


void VGEMainWindow::resetToSelection(){
    if (_selectAction){
        _selectAction->setChecked(true);
    }
}


void VGEMainWindow::putMessage(QString text, bool isError)
{
    if (isError){
        _messageLabel->setStyleSheet("font-weight: bold; color: red");
    } else {
        _messageLabel->setStyleSheet("font-weight: regular; color: light gray");
    }
    _messageLabel->setText(text);
    qDebug() << text << " " << isError;
}


void VGEMainWindow::openNewWindow(QWidget *wgt)
{
    qDebug() << wgt;
    wgt->show();
}


void VGEMainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug() << event;
    if (event->key() == Qt::Key_Escape){
        event->accept();
        resetToSelection();
    } else {
        event->ignore();
    }
}
