#ifndef VGEELLIPSE_H
#define VGEELLIPSE_H


#include "vgeshape.h"


class VGECircle;

QPair<QPoint, QPoint> tangent(VGECircle *circle1, VGECircle *circle2, const QPointF &point);


class VGECircle : public VGEShape {
    Q_OBJECT

public:
    explicit VGECircle(QObject *parent = nullptr, QColor color = vge::SHAPE_DEFAULT_COLOR)
        : VGEShape(parent, std::move(color))
    { _name = QString::fromStdString(std::string("circle") + std::to_string(_number)); }

    explicit VGECircle(QObject *parent, QColor color, QPointF center, qreal radius);

    void move(const QPointF &displacement) override;
    void scale(qreal coefficeint) override;
    void handleMousePressEvent(QMouseEvent *event) override;
    void handleMouseMoveEvent(QMouseEvent *event) override;
    void handleMouseReleaseEvent(QMouseEvent *event) override;
    VGERShape& getRaster() override;

    inline QPointF getCenter() const
    { return _center; }

    inline qreal getRadius() const
    { return _radius; }

    void setCenter(QPointF point)
    { _center = std::move(point); draw(); }

    void setRadius(qreal radius)
    { _radius = radius; draw(); }

    void draw() override;


private:
    void bresenhamCircle(QVector<QPoint> &circle);


private:
    QPointF _center;
    qreal _radius;
};


#endif // VGEELLIPSE_H
