#include "vgeline.h"

#include <QtMath>


VGELine::VGELine(QObject *parent, QColor color, QPointF firstPoint, QPointF lastPoint) :
VGEShape(parent, color), _firstPoint(firstPoint), _lastPoint(lastPoint) {
    draw();
}


void VGELine::move(QPointF displacement) {
    _firstPoint.rx() += displacement.x();
    _firstPoint.ry() += displacement.y();
    _lastPoint.rx() += displacement.x();
    _lastPoint.ry() += displacement.y();
    draw();
}


void VGELine::scale(qreal coefficeint) {
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
        else if (static_cast<int>(distX) == 0) {
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
        else if (static_cast<int>(distX) == 0) {
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


void VGELine::handleMousePressEvent(QMouseEvent * event) {
    _firstPoint = event->pos();
    _lastPoint = event->pos();
    _isMousePressed = true;
    draw();
}


void VGELine::handleMouseMoveEvent(QMouseEvent * event) {
    _lastPoint = event->pos();
    draw();
}


void VGELine::handleMouseReleaseEvent(QMouseEvent * event) {
    _lastPoint = event->pos();
    _isMousePressed = false;
    draw();
}


void VGELine::draw() {
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
    bresenhamLine(_firstPoint, _lastPoint, *_shapePoints);

    if (_raster) {
        delete _raster;
    }
    _raster = new VGERShape(_shapePoints, drawColor);
}


VGERShape& VGELine::getRaster() {
    if (!_raster) {
        draw();
    }
    return *_raster;
}


QString VGELine::str() const {
    std::string str = "LINE" + std::to_string(_number);
    return QString(str.c_str());
}
