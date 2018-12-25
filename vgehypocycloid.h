#ifndef VGEHYPOCYCLOID_H
#define VGEHYPOCYCLOID_H

#include "vgeshape.h"


class VGEHypocycloid : public VGEShape {
    Q_OBJECT

public:
    explicit VGEHypocycloid(QObject *parent = nullptr,
                            QColor color = vge::SHAPE_DEFAULT_COLOR) :
                            VGEShape(parent, color) {}

    explicit VGEHypocycloid(QObject *parent, QColor color,
                            QPointF center, qreal radiusOut, qreal radiusInn);

    void move(QPointF displacement) override;
    void scale(qreal coefficeint) override; //todo
    void handleMousePressEvent(QMouseEvent *event) override;
    void handleMouseMoveEvent(QMouseEvent *event) override;
    void handleMouseReleaseEvent(QMouseEvent *event) override;
    VGERShape& getRaster() override;
    QString str() const override; // redo

    inline QPointF getCenter() const
    { return _center; }

    inline qreal getRadiusOut() const
    { return _radiusOut; }

    inline qreal getRadiusInn() const
    { return _radiusInn; }

    void setCenter(const QPointF &point)
    { _center = point; draw(); }

    void setRadiusOut(const qreal &radius)
    { _radiusOut = radius; draw(); }

    void setRadiusInn(const qreal &radius)
    { _radiusInn = radius; draw(); }

    void draw() override;


private:
    QPointF _center;
    qreal _radiusOut;
    qreal _radiusInn;
    quint8 _pressCount = 0;
    QPointF _extra;

};


#endif // VGEHYPOCYCLOID_H
