#ifndef VGERECTANGLE_H
#define VGERECTANGLE_H

#include "vgeshape.hpp"


class VGERectangle;

class VGERectangle : public VGEShape {
    Q_OBJECT

public:
    explicit VGERectangle(QObject *parent = nullptr,
                     QColor color = vge::SHAPE_DEFAULT_COLOR) :
                     VGEShape(parent, color) {}

    explicit VGERectangle(QObject *parent, QColor color,
                     QPointF firstPoint, QPointF lastPoint);

    void move(QPointF displacement) override;
    void scale(qreal coefficeint) override;
    void handleMousePressEvent(QMouseEvent *event) override;
    void handleMouseMoveEvent(QMouseEvent *event) override;
    void handleMouseReleaseEvent(QMouseEvent *event) override;
    VGERShape& getRaster() override;
    QString str() const override;

    inline QPointF getFP() const
    { return _firstPoint; }

    inline QPointF getSP() const
    { return _lastPoint; }

    void setFP(const QPointF &point)
    { _firstPoint = point; draw(); }

    void setLP(const QPointF &point)
    { _lastPoint = point; draw(); }


protected:
    void draw() override;


private:
    QPointF _firstPoint;
    QPointF _lastPoint;

};


#endif // VGERECTANGLE_H
