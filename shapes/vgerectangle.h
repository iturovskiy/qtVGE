#ifndef VGERECTANGLE_H
#define VGERECTANGLE_H

#include "vgeshape.h"


class VGERectangle;

class VGERectangle : public VGEShape {
    Q_OBJECT

public:
    explicit VGERectangle(QObject *parent = nullptr, QColor color = vge::SHAPE_DEFAULT_COLOR)
        : VGEShape(parent, std::move(color))
    {_name = QString::fromStdString(std::string("rectangle") + std::to_string(_number));}

    explicit VGERectangle(QObject *parent, QColor color, QPointF firstPoint, QPointF lastPoint);

    void move(const QPointF &displacement) override;
    void scale(qreal coefficeint) override;
    void handleMousePressEvent(QMouseEvent *event) override;
    void handleMouseMoveEvent(QMouseEvent *event) override;
    void handleMouseReleaseEvent(QMouseEvent *event) override;
    VGERShape& getRaster() override;


    inline QPointF getFP() const
    { return _firstPoint; }

    inline QPointF getLP() const
    { return _lastPoint; }

    inline bool filled() const
    { return _filled; }

    void setFP(QPointF point)
    { _firstPoint = std::move(point); draw(); }

    void setLP(QPointF point)
    { _lastPoint = std::move(point); draw(); }

    void change()
    { _filled = !_filled; }


protected:
    void draw() override;


private:
    QPointF _firstPoint;
    QPointF _lastPoint;
    bool _filled = true;

};


#endif // VGERECTANGLE_H
