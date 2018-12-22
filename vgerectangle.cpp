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
    const qreal distX = _lastPoint.x() - _firstPoint.x() ;
    const qreal distY = _lastPoint.y() - _firstPoint.x() ;
    const qreal length = qSqrt(distX * distX + distY * distY);
    const qreal newlength = coefficeint * length;
    const qreal deltaLength = newlength - length;
    const qreal tan = distX / distY;
    const qreal angle = qAtan(tan);
    qreal deltaX, deltaY;
    if (deltaLength > 0) {
        deltaX = qSin(angle) * deltaLength;
        deltaY = qCos(angle) * deltaLength;
        if (distX < 0) {
            _firstPoint.rx() += deltaX;
            _firstPoint.ry() += deltaY;
        }
        else if ((int)distX == 0) {
            if (distY > 0) {
                _firstPoint.ry() += deltaY;
            }
            else {
                _lastPoint.ry() += deltaY;
            }
        }
        else if (distX > 0) {
            _lastPoint.rx() += deltaX;
            _lastPoint.ry() += deltaY;
        }
    }
    else {
        deltaX = qSin(angle) * newlength;
        deltaY = qCos(angle) * newlength;
        if (distX < 0) {
            _firstPoint.rx() = _lastPoint.x() + deltaX;
            _firstPoint.ry() = _lastPoint.y() + deltaY;
        }
        else if ((int)distX == 0) {
            if (distY > 0) {
                _firstPoint.ry() += deltaY;
            }
            else {
                _lastPoint.ry() += deltaY;
            }
        }
        else if (distX > 0) {
            _lastPoint.rx() = _firstPoint.x() + deltaX;
            _lastPoint.ry() = _firstPoint.x() + deltaY;
        }
    }
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
    const qreal distY = _lastPoint.y() - _firstPoint.y();
    int step;
    (distY < 0) ? step = -1 : step = 1;
    int i = 0;
    while (abs(_firstPoint.y() + i * step) < abs(_lastPoint.y())) {
        QPointF p1(_firstPoint.x(), _firstPoint.y() + i * step);
        QPointF p2(_lastPoint.x(), _firstPoint.y() + i * step);
        bresenhamLine(p1, p2, *_shapePoints);
        i++;
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

//! redo
QString VGERectangle::str() const {
    std::string str;
    return QString(str.c_str());
}
