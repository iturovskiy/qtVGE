#ifndef VGESHAPE_H
#define VGESHAPE_H


#include <QObject>
#include <QtWidgets>
#include <QMouseEvent>

#include "configs.h"

static quint32 count = 0;

int NOD(int a, int b);
void bresenhamLinePoints(const QPointF &fp, const QPointF &lp, QVector<QPoint> &line);
void bresenhamCirclePoints(const QPointF &center, qreal radius, QVector<QPoint> &ellipse, bool filled = true);
void hypocycloidPoints(const QPointF &center, qreal radiusOut, qreal radiusInn, QVector<QPoint> &hypo);


class VGEShape;
class VGERShape;


class VGERShape : public QObject {
    Q_OBJECT

public:
    VGERShape() : _points(nullptr) {}
    VGERShape(QVector<QPoint> *points, QColor color);
    void operator() (QImage *image) const;

private:
    QVector<QPoint> *_points;
    QPen _pen;

friend class VGEShape;

};


class VGEShape : public QObject {
    Q_OBJECT

public:
    explicit VGEShape(QObject *parent = nullptr,
                      QColor color = vge::SHAPE_DEFAULT_COLOR) :
                      QObject(parent), _color(color)
                      { _number = count++; }

    virtual ~VGEShape()
    { delete _shapePoints; if (_raster) delete _raster; }

    virtual void move(QPointF displacement) = 0;
    virtual void scale(qreal coefficient) = 0;
    virtual void handleMousePressEvent(QMouseEvent *event) = 0;
    virtual void handleMouseMoveEvent(QMouseEvent *event) = 0;
    virtual void handleMouseReleaseEvent(QMouseEvent *event) = 0;
    virtual VGERShape& getRaster() = 0;
    virtual QString str() const = 0;
    virtual void draw() = 0;

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

    void setPoint(QVector<QPoint> *point)
    { delete _shapePoints; std::swap(_shapePoints, point); }

    int test(QPoint point);

protected:
    quint32 _number;
    QColor _color;
    bool _isMousePressed = false;
    bool _isSelected = false;
    VGERShape *_raster = new VGERShape;
    QVector<QPoint> *_shapePoints = new QVector<QPoint>;


signals:

public slots:

};


#endif // VGESHAPE_H
