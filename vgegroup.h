#ifndef VGEGROUP_H
#define VGEGROUP_H

#include "vgeshape.h"

class VGEGroup;


class VGEGroup : protected VGEShape {
    Q_OBJECT

public:
    explicit VGEGroup(QObject *parent = nullptr,
                     QColor color = vge::SHAPE_DEFAULT_COLOR) :
                     VGEShape(parent, color) {}

    explicit VGEGroup(QObject *parent, QColor color,
                      QList<VGEShape *> shList, QString name);

    void move(QPointF displacement) override;
    void scale(qreal coefficeint) override;
    void handleMousePressEvent(QMouseEvent *event) override;
    void handleMouseMoveEvent(QMouseEvent *event) override;
    void handleMouseReleaseEvent(QMouseEvent *event) override;
    VGERShape& getRaster() override;

    QString str() const override
    { return _name; }

    inline void setName(QString newname)
    { _name = newname; }

    VGEShape * get(size_t index);
    void add(VGEShape * shape);
    bool del(size_t index);
    bool del(QString name);

    void draw() override;


private:
    QList<VGEShape *> _shList;
    QString _name;

};

#endif // VGEGROUP_H
