#ifndef VGEACTIONS_H
#define VGEACTIONS_H

#include <QMainWindow>
#include <QtWidgets>

#include "vgeshape.hpp"


class ColorGridWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ColorGridWidget(QWidget* parent = nullptr): QWidget(parent) {}


protected:
    void closeEvent(QCloseEvent *event)
    { emit closed(); event->ignore(); }


signals:
    void closed();

};


class ColorButton: public QPushButton
{
    Q_OBJECT

public:
    ColorButton(quint8 color);
    quint8 getColor() { return m_color; }
    virtual ~ColorButton() = default;


signals:
    void changeColor(quint8 color);


private slots:
    void checkButton(bool state)
    { if (state) emit changeColor(m_color);  }


private:
    quint8 m_color;

};


class VGEActions : public QMainWindow
{
    Q_OBJECT

public:
    VGEActions(QWidget *parent,
               vge::shapeType type,
               QColor color,
               QPointF first,
               QPointF last);

    VGEActions(QWidget *parent,
               vge::shapeType type,
               QPointF center,
               qreal radius);

    VGEActions(QWidget *parent,
               vge::shapeType type,
               QPointF center,
               qreal oradius,
               qreal iradius);

    ~VGEActions()
    { delete _colorButton; }


protected:
    void closeEvent(QCloseEvent *event)
    { event->ignore(); cancel(); }


private:
    void init(QPointF firstCoord);


signals:
    void finished();
    void updateShape(QColor color, QPointF first, QPointF last, float rOuter, float rInner);


public slots:
    void showColorGrid();
    void setColor(quint8 color);
    void hideColorGrid();
    void saveColorGrid();
    void cancel();


private:
    QWidget* _mainWgt = new QWidget(this);
    QGridLayout* _gridLayout = new QGridLayout(this);



    ColorButton *_colorButton;
    bool _colorConsistency = true; // hz
    QList<ColorButton *> _colorGridButtons;
    ColorGridWidget* _colorSelectionWidget;

    QPushButton* _confirmButton = new QPushButton(this);
    QPushButton* _cancelButton = new QPushButton(this);
    QPushButton* _colorConfirmButton = new QPushButton(this);
    QPushButton* _colorCancelButton = new QPushButton(this);

    vge::shapeType _shapeType;
};

#endif // VGEACTIONS_H
