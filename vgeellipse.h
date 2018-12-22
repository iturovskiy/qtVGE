#ifndef VGEELLIPSE_H
#define VGEELLIPSE_H


#include "vgeshape.hpp"


class VGEEllipse;


class VGEEllipse : public VGEShape {
    Q_OBJECT

public:
    explicit VGEEllipse(QObject *parent = nullptr,
                     QColor color = vge::SHAPE_DEFAULT_COLOR) :
                     VGEShape(parent, color) {}

    explicit VGEEllipse(QObject *parent, QColor color,
                     QPointF center, qreal radius);

    void move(QPointF displacement) override;
    void scale(qreal coefficeint) override;
    void handleMousePressEvent(QMouseEvent *event) override;
    void handleMouseMoveEvent(QMouseEvent *event) override;
    void handleMouseReleaseEvent(QMouseEvent *event) override;
    VGERShape& getRaster() override;
    QString str() const override;

    inline QPointF getCenter() const
    { return _center; }

    inline qreal getRadius() const
    { return _radius; }

    void setCenter(const QPointF &point)
    { _center = point; draw(); }

    void setRadius(const qreal &radius)
    { _radius = radius; draw(); }


protected:
    void draw() override;


private:
    QPointF _center;
    qreal _radius;
    QPointF _firstPoint, _lastPoint;
};


#endif // VGELINE_H


#endif // VGEELLIPSE_H
