#ifndef VGRLINE_H
#define VGRLINE_H

#include "vgrshape.h"


namespace vgr {
class vgrLine;
}


namespace vgr {

class vgrLine : public vgrShape {
    Q_OBJECT

public:

    vgrLine(QObject *parent = nullptr);

    virtual ~vgrLine();

    virtual void move(QPointF vec) override;

    virtual void handleMousePressEvent(QMouseEvent * event) override;

    virtual void handleMouseMoveEvent(QMouseEvent * event) override;

    virtual void handleMouseReleaseEvent(QMouseEvent * event) override;

    virtual bool isReady() override;

    QPoint getFP() const;

    QPoint getSP() const;

    void setFP(const Point & point);

    void setSP(const Point & point);


protected:

    virtual void draw() override;


private:

    QPoint _firstPoint;
    QPoint _secondPoint;

};

}

#endif // VGRLINE_H
