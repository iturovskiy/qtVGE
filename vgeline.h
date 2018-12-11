#ifndef VGELINE_H
#define VGELINE_H

#include "vgeshape.hpp"


class VGELine;

void bresenhamLine(const QPointF &fp, const QPointF &lp, QVector<QPoint> &line);


class VGELine : public VGEShape {
    Q_OBJECT

public:
    explicit VGELine(QObject *parent = nullptr,
                     QColor color = vge::SHAPE_DEFAULT_COLOR) :
                     VGEShape(parent, color) {}

    explicit VGELine(QObject *parent, QColor color,
                     QPointF firstPoint, QPointF lastPoint);

    void move(QPointF displacement) override;

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


#endif // VGELINE_H
