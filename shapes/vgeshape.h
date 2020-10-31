#ifndef VGESHAPE_H
#define VGESHAPE_H


#include <QObject>
#include <QtWidgets>
#include <QMouseEvent>

#include "configs.h"

void bresenhamLine(const QPointF &fp, const QPointF &lp,
                   QVector<QPoint> &line,
                   bool clipped = false,
                   const QPointF &cutFP = QPointF(0,0),
                   const QPointF &cutLP = QPointF(vge::IMAGE_W, vge::IMAGE_W));

void bresenhamCirclePoints(const QPointF &center, qreal radius, QVector<QPoint> &ellipse);


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
    explicit VGEShape(QObject *parent = nullptr, QColor color = vge::SHAPE_DEFAULT_COLOR)
        : QObject(parent),
          _color(std::move(color))
    { _number++; }

    virtual ~VGEShape()
    { delete _shapePoints; if (_raster) delete _raster; }

    virtual void move(const QPointF & displacement) = 0;
    virtual void scale(qreal coefficeint) = 0;
    virtual void handleMousePressEvent(QMouseEvent *event) = 0;
    virtual void handleMouseMoveEvent(QMouseEvent *event) = 0;
    virtual void handleMouseReleaseEvent(QMouseEvent *event) = 0;
    virtual VGERShape& getRaster() = 0;
    virtual void draw() = 0;

    virtual void clip(const QPointF &fp, const QPointF &lp);
    bool clipContains(const QPointF &point);
    void clipMove(const QPointF &displacement);
    void clipScale(qreal coefficeint);
    int test(const QPoint &point);

    virtual bool isReady()
    { return !_isMousePressed; }

    inline QVector<QPoint> * getPoints()
    { return _shapePoints; }

    inline QColor getColor() const
    { return _color;}

    inline QString getName() const
    { return _name; }

    void select(bool var);

    inline bool isSelected() const
    { return _isSelected; }

    void setColor(QColor newColor)
    { _color = std::move(newColor); draw(); }

    void setPoints(QVector<QPoint> *points)
    { delete _shapePoints; _shapePoints = points; }

    void setName(QString name)
    { _name = std::move(name); }


protected:
    QColor _color;
    QPointF _cutFP, _cutLP;
    QVector<QPoint> *_shapePoints = new QVector<QPoint>;
    VGERShape *_raster = new VGERShape;
    bool _clipped = false;
    bool _isMousePressed = false;
    bool _isSelected = false;
    static quint32 _number;
    QString _name;
};


#endif // VGESHAPE_H
