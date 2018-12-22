#include "vgeellipse.h"

#include <QtMath>


VGEEllipse::VGEEllipse(QObject *parent, QColor color, QPointF center, qreal radius) :
VGEShape(parent, color), _center(center),_radius(radius) {
    draw();
}


void VGEEllipse::move(QPointF displacement) {
    _center.rx() += displacement.x();
    _center.ry() += displacement.y();
    draw();
}

/// todo
void VGEEllipse::scale(qreal coefficeint) {

    draw();
}


void VGEEllipse::handleMousePressEvent(QMouseEvent * event) {
    _firstPoint = event->pos();
    _lastPoint = event->pos();
    _isMousePressed = true;
    draw();
}


void VGEEllipse::handleMouseMoveEvent(QMouseEvent * event) {
    _lastPoint = event->pos();
    draw();
}


void VGEEllipse::handleMouseReleaseEvent(QMouseEvent * event) {
    _lastPoint = event->pos();
    _isMousePressed = false;
    draw();
}


void VGEEllipse::draw() {
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


VGERShape& VGEEllipse::getRaster() {
    if (!_raster) {
        draw();
    }
    return *_raster;
}

// redo
QString VGEEllipse::str() const {
    std::string str;
    return QString(str.c_str());
}
