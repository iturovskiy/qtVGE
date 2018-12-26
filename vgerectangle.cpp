#include "vgerectangle.h"

#include <QtMath>


VGERectangle::VGERectangle(QObject *parent, QColor color, QPointF firstPoint, QPointF lastPoint) :
VGEShape(parent, color), _firstPoint(firstPoint), _lastPoint(lastPoint) {
    draw();
}


void VGERectangle::move(QPointF displacement) {
    _firstPoint.rx() += displacement.x();
    _firstPoint.ry() += displacement.y();
    _lastPoint.rx() += displacement.x();
    _lastPoint.ry() += displacement.y();
    draw();
}


void VGERectangle::scale(qreal coefficeint) {
    auto xmin = std::min<qreal>(_firstPoint.x(), _lastPoint.x());
    auto ymin = std::min<qreal>(_firstPoint.y(), _lastPoint.y());
    _firstPoint.rx() += (_firstPoint.x() - xmin) * (coefficeint - 1);
    _firstPoint.ry() += (_firstPoint.y() - ymin) * (coefficeint - 1);
    _lastPoint.rx() += (_lastPoint.x() - xmin) * (coefficeint - 1);
    _lastPoint.ry() += (_lastPoint.y() - ymin) * (coefficeint - 1);
    draw();
}


void VGERectangle::handleMousePressEvent(QMouseEvent * event) {
    _firstPoint = event->pos();
    _lastPoint = event->pos();
    _isMousePressed = true;
    draw();
}


void VGERectangle::handleMouseMoveEvent(QMouseEvent * event) {
    _lastPoint = event->pos();
    draw();
}


void VGERectangle::handleMouseReleaseEvent(QMouseEvent * event) {
    _lastPoint = event->pos();
    _isMousePressed = false;
    draw();
}


void VGERectangle::draw() {
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
    if (_filled) {
        const qreal distY = _lastPoint.y() - _firstPoint.y();
        int i = 0;
        if (distY >= 0) {
            while (abs(_firstPoint.y() + i) <= abs(_lastPoint.y())) {
                QPointF p1(_firstPoint.x(), _firstPoint.y() + i);
                QPointF p2(_lastPoint.x(), _firstPoint.y() + i);
                bresenhamLinePoints(p1, p2, *_shapePoints);
                i++;
            }
        }
        else {
            while (abs(_firstPoint.y() - i) >= abs(_lastPoint.y())) {
                QPointF p1(_firstPoint.x(), _firstPoint.y() - i);
                QPointF p2(_lastPoint.x(), _firstPoint.y() - i);
                bresenhamLinePoints(p1, p2, *_shapePoints);
                i++;
            }
        }
    }
    else {
        bresenhamLinePoints(_firstPoint, QPoint(static_cast<int>(_firstPoint.x()), static_cast<int>(_lastPoint.y())), *_shapePoints);
        bresenhamLinePoints(_firstPoint, QPoint(static_cast<int>(_lastPoint.x()), static_cast<int>(_firstPoint.y())), *_shapePoints);
        bresenhamLinePoints(_lastPoint,  QPoint(static_cast<int>(_firstPoint.x()), static_cast<int>(_lastPoint.y())), *_shapePoints);
        bresenhamLinePoints(_lastPoint,  QPoint(static_cast<int>(_lastPoint.x()), static_cast<int>(_firstPoint.y())), *_shapePoints);
    }


    if (_raster) {
        delete _raster;
    }
    _raster = new VGERShape(_shapePoints, drawColor);
}


VGERShape& VGERectangle::getRaster() {
    if (!_raster) {
        draw();
    }
    return *_raster;
}


QString VGERectangle::str() const {
    std::string str = "RECT" + std::to_string(_number);
    return QString(str.c_str());
}

/// todo
QList<VGEShape *> VGERectangle::clip() {
    QList<VGEShape *> s;
    return s;
}
