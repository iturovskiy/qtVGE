#ifndef VGELINE_H
#define VGELINE_H

#include "vgeshape.h"
#include "vgecircle.h"

class VGELine;


class VGELine : public VGEShape {
    Q_OBJECT

public:
    explicit VGELine(QObject *parent = nullptr, QColor color = vge::SHAPE_DEFAULT_COLOR)
        : VGEShape(parent, std::move(color))
    {_name = QString::fromStdString(std::string("line") + std::to_string(_number));}

    explicit VGELine(QObject *parent, QColor color, QPointF firstPoint, QPointF lastPoint);

    void move(const QPointF &displacement) override;
    void scale(qreal coefficeint) override;
    void handleMousePressEvent(QMouseEvent *event) override;
    void handleMouseMoveEvent(QMouseEvent *event) override;
    void handleMouseReleaseEvent(QMouseEvent *event) override;
    VGERShape& getRaster() override;

    inline QPointF getFP() const
    { return _firstPoint; }

    inline QPointF getSP() const
    { return _lastPoint; }

    void setFP(QPointF point)
    { _firstPoint = std::move(point); draw(); }

    void setLP(QPointF point)
    { _lastPoint = std::move(point); draw(); }

    void draw() override;

private:
    void bresenhamLinePoints(const QPointF &fp, const QPointF &lp, QVector<QPoint> &line);


private:
    QPointF _firstPoint;
    QPointF _lastPoint;

};


#endif // VGELINE_H
