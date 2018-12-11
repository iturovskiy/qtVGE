#ifndef VGESHAPE_H
#define VGESHAPE_H


#include <QObject>
#include <QtWidgets>
#include <QMouseEvent>

#include "configs.h"


class VGEShape;
class VGERShape;


class VGERShape : public QObject {
    Q_OBJECT

public:
    VGERShape() : _points(nullptr), _keyPoints(nullptr) {}

    VGERShape(QVector<QPoint> *points, QColor color, QVector<QPoint> *keyPoints = nullptr);

    void operator() (QImage *image) const;

private:
    QVector<QPoint> *_points;
    QPen _pen;
    QVector<QPoint> *_keyPoints;
    QPen _keyPen;

friend class VGEShape;

};


class VGEShape : public QObject {
    Q_OBJECT

public:
    explicit VGEShape(QObject *parent = nullptr,
                      QColor color = vge::SHAPE_DEFAULT_COLOR) :
                      QObject(parent), _color(color) {}

    virtual ~VGEShape()
    { delete _shapePoints; if (_raster) delete _raster; }

    virtual void move(QPointF displacement) = 0;

    virtual void handleMousePressEvent(QMouseEvent *event) = 0;

    virtual void handleMouseMoveEvent(QMouseEvent *event) = 0;

    virtual void handleMouseReleaseEvent(QMouseEvent *event) = 0;

    virtual VGERShape& getRaster() = 0;

    virtual QString str() const = 0;

    virtual bool isReady()
    { return !_isMousePressed; }

    inline QVector<QPoint> * getPoints()
    { return _shapePoints; }

    inline QColor getColor() const
    { return _color;}

    void select(bool var);

    inline bool isSelected() const
    { return _isSelected; }

    void setColor(const QColor &newColor)
    { _color = newColor; draw(); }


protected:
    virtual void draw() = 0;


protected:
    QColor _color;
    bool _isMousePressed = false;
    bool _isSelected = false;
    VGERShape *_raster = new VGERShape;
    QVector<QPoint> *_shapePoints = new QVector<QPoint>;


signals:
public slots:

};


void VGEShape::select(bool var) {
    if (_isSelected != var) {
        _isSelected = var;
        draw();
    }
}


VGERShape::VGERShape(QVector<QPoint> *points, QColor color, QVector<QPoint> *keyPoints) :
_points(points), _pen(color), _keyPoints(keyPoints), _keyPen(QColor(0xFF, 0xFF, 0, 0xFF)) {
    _pen.setWidth(vge::DEFAULT_WIDTH);
    _keyPen.setWidth(vge::DEFAULT_WIDTH * 5);
}


void VGERShape::operator()(QImage * image) const {
    if (!_points) {
        return;
    }
    QPainter painter(image);
    painter.setPen(_pen);
    painter.drawPoints(_points->data(), _points->size());
    if(_keyPoints) {
        painter.setPen(_keyPen);
        painter.drawPoints(_keyPoints->data(), _keyPoints->size());
    }
    painter.end();
}


#endif // VGESHAPE_H
