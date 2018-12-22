#ifndef VGESHAPE_H
#define VGESHAPE_H


#include <QObject>
#include <QtWidgets>
#include <QMouseEvent>

#include "configs.h"

void bresenhamLine(const QPointF &fp, const QPointF &lp, QVector<QPoint> &line);

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
    virtual void scale(qreal coefficient) = 0;
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

    int test(QPoint point);


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


int VGEShape::test(QPoint point) {
    int min = 2 * vge::SEARCH_W * vge::SEARCH_W;
    for (QPoint shapePoint : *_shapePoints){
        int distX = abs(shapePoint.x() - point.x());
        int distY = abs(shapePoint.y() - point.y());
        if (distX < vge::SEARCH_W && distY < vge::SEARCH_W) {
            int square =  distX * distX + distY * distY;
            min = min < square ? min : square;
        }
    }
    return min;
}


VGERShape::VGERShape(QVector<QPoint> *points, QColor color, QVector<QPoint> *keyPoints) :
_points(points), _pen(color), _keyPoints(keyPoints), _keyPen(QColor(0xFF, 0xFF, 0, 0xFF)) {
    _pen.setWidth(vge::DEFAULT_W);
    _keyPen.setWidth(vge::DEFAULT_W * 5);
}


void VGERShape::operator()(QImage *image) const {
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


void bresenhamLine(const QPointF &fp, const QPointF &lp, QVector<QPoint> &line) {

    int x2 = static_cast<int>(lp.x());
    int x1 = static_cast<int>(fp.x());
    int y2 = static_cast<int>(lp.y());
    int y1 = static_cast<int>(fp.y());
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;

    line.append(QPoint(x2, y2));

    while(x1 != x2 || y1 != y2) {
        line.append(QPoint(x1, y1));
        const int error2 = error * 2;
        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}

#endif // VGESHAPE_H
