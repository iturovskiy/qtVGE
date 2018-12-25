#include "vgecircle.h"

#include <QtMath>


VGECircle::VGECircle(QObject *parent, QColor color, QPointF center, qreal radius) :
VGEShape(parent, color), _center(center),_radius(radius) {
    draw();
}


void VGECircle::move(QPointF displacement) {
    _center.rx() += displacement.x();
    _center.ry() += displacement.y();
    draw();
}


void VGECircle::scale(qreal coefficeint) {
    qreal disp;
    if (coefficeint > 0) {
        disp = _radius - _radius / coefficeint;
        _center.rx() += disp / sqrt(2);
        _center.ry() -= disp / sqrt(2);
    }
    else {
        disp = _radius / coefficeint - _radius;
        _center.rx() -= disp / sqrt(2);
        _center.ry() += disp / sqrt(2);
    }
    _radius *= coefficeint;
    draw();
}


void VGECircle::handleMousePressEvent(QMouseEvent * event) {
    _center = event->pos();
    _isMousePressed = true;
    draw();
}


void VGECircle::handleMouseMoveEvent(QMouseEvent * event) {
    QPointF pos;
    pos = event->pos();
    _radius = sqrt((pos.x() - _center.x()) * (pos.x() - _center.x())
                   + (pos.y() - _center.y()) * (pos.y() - _center.y()));
    draw();
}


void VGECircle::handleMouseReleaseEvent(QMouseEvent * event) {
    QPointF pos;
    pos = event->pos();
    _radius = sqrt((pos.x() - _center.x()) * (pos.x() - _center.x())
              + (pos.y() - _center.y()) * (pos.y() - _center.y()));
    _isMousePressed = false;
    draw();
}


void VGECircle::draw() {
    QColor drawColor;
    if (_isSelected){
        drawColor = QColor(0xFF - _color.red(), 0xFF - _color.green(), 0xFF - _color.blue(), 0xFF);
        if (drawColor == vge::BG_DEFAULT_COLOR){
            drawColor = vge::RED_COLOR;
        }
    } else {
        drawColor = _color;
    }

    _shapePoints->clear();

    bresenhamEllipse(_center, _radius, *_shapePoints);
    if (_raster) {
        delete _raster;
    }
    _raster = new VGERShape(_shapePoints, drawColor);
}


VGERShape& VGECircle::getRaster() {
    if (!_raster) {
        draw();
    }
    return *_raster;
}

// redo
QString VGECircle::str() const {
    std::string str = "CIRC" + std::to_string(_number);
    return QString(str.c_str());
}
