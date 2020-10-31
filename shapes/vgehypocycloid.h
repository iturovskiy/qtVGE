#ifndef VGEHYPOCYCLOID_H
#define VGEHYPOCYCLOID_H

#include "vgeshape.h"


class VGEHypocycloid : public VGEShape {
    Q_OBJECT

public:
    explicit VGEHypocycloid(QObject *parent = nullptr, QColor color = vge::SHAPE_DEFAULT_COLOR)
        : VGEShape(parent, std::move(color))
    {_name = QString::fromStdString(std::string("hypo") + std::to_string(_number));}

    explicit VGEHypocycloid(QObject *parent, QColor color, QPointF center, qreal radiusOut, qreal radiusInn);

    void move(const QPointF &displacement) override;
    void scale(qreal coefficeint) override;
    void handleMousePressEvent(QMouseEvent *event) override;
    void handleMouseMoveEvent(QMouseEvent *event) override;
    void handleMouseReleaseEvent(QMouseEvent *event) override;
    VGERShape& getRaster() override;

    inline QPointF getCenter() const
    { return _center; }

    inline qreal getRadiusOut() const
    { return _radiusOut; }

    inline qreal getRadiusInn() const
    { return _radiusInn; }

    void setCenter(QPointF point)
    { _center = std::move(point); draw(); }

    void setRadiusOut(qreal radius)
    { _radiusOut = radius; draw(); }

    void setRadiusInn(qreal radius)
    { _radiusInn = radius; draw(); }

    void draw() override;


private:
    void hypocycloidPoints(QVector<QPoint> &hypo);
    int NOD(int a, int b);


private:
    QPointF _center;
    qreal _radiusOut;
    qreal _radiusInn;
    quint8 _pressCount = 0;
    QPointF _extra;

};


#endif // VGEHYPOCYCLOID_H
