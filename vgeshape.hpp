#ifndef VGESHAPE_H
#define VGESHAPE_H

#include <QObject>
#include <QWidget>
#include "configs.h"


namespace vge {
class VGEShape;
}


namespace vge {

class VGEShape : public QObject {
    Q_OBJECT

public:

    explicit VGEShape(QObject *parent = nullptr,
                      QColor color = vge::SHAPE_DEFAULT_COLOR) :
                      QObject(parent), _color(color) {}

    virtual ~VGEShape()
    { delete _shapePoints; }

    virtual void move(QPointF vec) = 0;

    virtual void handleMousePressEvent(QMouseEvent * event) = 0;

    virtual void handleMouseMoveEvent(QMouseEvent * event) = 0;

    virtual void handleMouseReleaseEvent(QMouseEvent * event) = 0;

    virtual bool isReady()
    { return !_isMousePressed; }

    QVector<QPoint> * getPoints()
    { return _shapePoints; }

    QColor getColor() const
    { return _color;}

    int getWidth() const
    { return _width; }

    bool isSelected() const
    { return _isSelected; }

    void setColor(const QColor &newColor)
    { _color = newColor; draw(); }


protected:

    virtual void draw() = 0;


protected:

    QColor _color;
    const int _width = DEFAULT_WIDTH;
    QVector<QPoint> * _shapePoints = new QVector<QPoint>;
    bool _isMousePressed = false;
    bool _isSelected = false;


signals:


public slots:


};


} // namespace vge

#endif // VGESHAPE_H
