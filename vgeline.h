#ifndef VGELINE_H
#define VGELINE_H

#include "vgeshape.h"


namespace vge {
class VGELine;

void brasenhamLine(int x0, int y0, int x1, int y1, QVector<QPoint> &line);
}


namespace vge {

class VGELine : public VGEShape {
    Q_OBJECT

public:

    explicit VGELine(QObject *parent = nullptr, const QColor &color = SHAPE_DEFAULT_COLOR) :
        VGEShape(parent, color) {}

    virtual ~VGELine() override;

    virtual void move(QPointF vec) override;

    virtual void handleMousePressEvent(QMouseEvent * event) override;

    virtual void handleMouseMoveEvent(QMouseEvent * event) override;

    virtual void handleMouseReleaseEvent(QMouseEvent * event) override;

    virtual bool isReady() override;

    QPoint getFP() const;

    QPoint getSP() const;

    void setFP(const QPoint & point);

    void setSP(const QPoint & point);


protected:

    virtual void draw() override;


private:

    QPoint _firstPoint;
    QPoint _secondPoint;

};

} // namespace vge

#endif // VGELINE_H
