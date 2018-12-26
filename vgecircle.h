#ifndef VGEELLIPSE_H
#define VGEELLIPSE_H


#include "vgeshape.h"


class VGECircle;

QPair<QPoint, QPoint> tangent(VGECircle *circle1, VGECircle *circle2, QPointF point);


class VGECircle : public VGEShape {
    Q_OBJECT

public:
    explicit VGECircle(QObject *parent = nullptr,
                     QColor color = vge::SHAPE_DEFAULT_COLOR) :
                     VGEShape(parent, color) {}

    explicit VGECircle(QObject *parent, QColor color,
                     QPointF center, qreal radius);

    void move(QPointF displacement) override;
    void scale(qreal coefficeint) override;
    void handleMousePressEvent(QMouseEvent *event) override;
    void handleMouseMoveEvent(QMouseEvent *event) override;
    void handleMouseReleaseEvent(QMouseEvent *event) override;
    VGERShape& getRaster() override;
    QString str() const override; //todo

    inline QPointF getCenter() const
    { return _center; }

    inline qreal getRadius() const
    { return _radius; }

    void setCenter(const QPointF &point)
    { _center = point; draw(); }

    void setRadius(const qreal &radius)
    { _radius = radius; draw(); }

    void draw() override;


private:
    QPointF _center;
    qreal _radius;
};


#endif // VGEELLIPSE_H
