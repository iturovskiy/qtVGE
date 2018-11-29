#ifndef VGRSHAPE_H
#define VGRSHAPE_H

#include <QObject>
#include "configs.h"


namespace vgr {
class vgrShape;
}

namespace vgr {

class vgrShape : public QObject {
    Q_OBJECT

public:

    explicit vgrShape(QObject *parent = nullptr, QColor color = vgr::SHAPE_DEFAULT_COLOR,
                      int width = 2) : QObject(parent), _color(color), _width(width) {}

    virtual ~vgrShape()
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

    void setWidth(int width)
    { _width = width; draw(); }


protected:

    virtual void draw() = 0;


protected:

    QColor _color;
    int _width;
    QVector<QPoint> * _shapePoints = new QVector<QPoint>;
    bool _isMousePressed = false;
    bool _isSelected = false;


signals:


public slots:


};


}

#endif // VGRSHAPE_H
