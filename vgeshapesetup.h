#ifndef VGEACTION_H
#define VGEACTION_H

#include <QMainWindow>
#include <QtWidgets>

#include "vgeshape.h"


class ColorGridWidget;
class ColorButton;
class VGEShapeSetUp;


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


class VGEShapeSetUp: public QMainWindow
{
    Q_OBJECT

public:
    VGEShapeSetUp(QWidget *parent,
                  vge::shapeType type,
                  QColor color,
                  QPointF first,
                  QPointF last); //

    VGEShapeSetUp(QWidget *parent,
                  vge::shapeType type,
                  QColor color,
                  QPointF center,
                  qreal radius);

    VGEShapeSetUp(QWidget *parent,
                   vge::shapeType type,
                   QColor color,
                   QPointF center,
                   qreal oradius,
                   qreal iradius);



    ~VGEShapeSetUp()
    { delete _colorButton; }


protected:
    void closeEvent(QCloseEvent *event)
    { event->ignore(); cancel(); }


private:
    void init(QPointF point);


signals:
    void finished();
    void updateShape(QColor color, qreal coef, QPointF first, QPointF last, float rOuter, float rInner);


public slots:
    void showColorGrid();
    void setColor(quint8 color);
    void hideColorGrid();
    void saveColorGrid();
    void cancel();
    void save();


private:
    // main tips
    vge::shapeType _shapeType;
    QWidget* _mainWgt = new QWidget(this);

    QLabel* _firstCoordXLabel = new QLabel(this);
    QDoubleSpinBox* _firstCoordXSpinBox = new QDoubleSpinBox(this);
    QLabel* _firstCoordYLabel  = new QLabel(this);
    QDoubleSpinBox* _firstCoordYSpinBox = new QDoubleSpinBox(this);

    QLabel* _coefficientLabel = new QLabel(this);
    QDoubleSpinBox* _coefficientSpinBox = new QDoubleSpinBox(this);

    QLabel* _lastCoordXLabel  = nullptr;
    QDoubleSpinBox* _lastCoordXSpinBox = nullptr;
    QLabel* _lastCoordYLabel  = nullptr;
    QDoubleSpinBox* _lastCoordYSpinBox = nullptr;

    QLabel* _radiusInnLabel  = nullptr;
    QDoubleSpinBox* _radiusInnSpinBox = nullptr;
    QLabel* _radiusOutLabel  = nullptr;
    QDoubleSpinBox* _radiusOutSpinBox = nullptr;

    // for pallete
    QGridLayout* _gridLayout = new QGridLayout(this);
    QLabel* _colorLabel = new QLabel(this);
    QColor _color;
    QColor _newColor;
    ColorButton *_colorButton;
    bool _colorConsistency = true; // hz
    QList<ColorButton *> _colorGridButtons;
    ColorGridWidget* _colorSelectionWidget = new ColorGridWidget(this);
    QPushButton* _colorConfirmButton = new QPushButton(this);
    QPushButton* _colorCancelButton = new QPushButton(this);

    // confirm & cancel
    QPushButton* _confirmButton = new QPushButton(this);
    QPushButton* _cancelButton = new QPushButton(this);

};

#endif // VGEACTION_H
