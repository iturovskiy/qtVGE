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
    auto xmin = std::min<qreal>(_firstPoint.x(), _lastPoint.x());
    auto ymin = std::min<qreal>(_firstPoint.y(), _lastPoint.y());
    _firstPoint.rx() += (_firstPoint.x() - xmin) * (coefficeint - 1);
    _firstPoint.ry() += (_firstPoint.y() - ymin) * (coefficeint - 1);
    _lastPoint.rx() += (_lastPoint.x() - xmin) * (coefficeint - 1);
    _lastPoint.ry() += (_lastPoint.y() - ymin) * (coefficeint - 1);
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
    bresenhamLinePoints(_firstPoint, _lastPoint, *_shapePoints);

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
