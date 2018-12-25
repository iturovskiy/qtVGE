#include "vgehypocycloid.h"


#include <QtMath>


VGEHypocycloid::VGEHypocycloid(QObject *parent, QColor color, QPointF center,
                               qreal radiusOut, qreal radiusInn) : VGEShape(parent, color),
                                                                   _center(center),
                                                                   _radiusOut(radiusOut),
                                                                   _radiusInn(radiusInn)
{
    draw();
}


void VGEHypocycloid::move(QPointF displacement) {
    _center.rx() += displacement.x();
    _center.ry() += displacement.y();
    draw();
}


void VGEHypocycloid::scale(qreal coefficeint) {
    qreal disp;
    if (coefficeint > 0) {
        disp = _radiusOut - _radiusOut / coefficeint;
        _center.rx() += disp / sqrt(2);
        _center.ry() -= disp / sqrt(2);
    }
    else {
        disp = _radiusOut / coefficeint - _radiusOut;
        _center.rx() -= disp / sqrt(2);
        _center.ry() += disp / sqrt(2);
    }
    _radiusOut *= coefficeint;
    _radiusInn *= coefficeint;
    draw();
}


void VGEHypocycloid::handleMousePressEvent(QMouseEvent * event) {
    qDebug() << __FUNCTION__ << "_pressCount : " << _pressCount;
    if (_pressCount == 0) {
        _center = event->pos();
    }
    _isMousePressed = true;
    draw();
}


void VGEHypocycloid::handleMouseMoveEvent(QMouseEvent * event) {
    // what here is?
    QPointF pos;
    pos = event->pos();
    if (_pressCount == 0) {
        _radiusOut = sqrt((pos.x() - _center.x()) * (pos.x() - _center.x())
                     + (pos.y() - _center.y()) * (pos.y() - _center.y()));
    }
    else {
        _radiusInn = sqrt((pos.x() - _extra.x()) * (pos.x() - _extra.x())
                         + (pos.y() - _extra.y()) * (pos.y() - _extra.y()));
    }
    draw();
}


void VGEHypocycloid::handleMouseReleaseEvent(QMouseEvent * event) {
    qDebug() << __FUNCTION__ << "_pressCount : " << _pressCount;
    QPointF pos;
    pos = event->pos();
    if (_pressCount == 0) {
        _radiusOut = sqrt((pos.x() - _center.x()) * (pos.x() - _center.x())
                          + (pos.y() - _center.y()) * (pos.y() - _center.y()));
        _extra = pos;
        _pressCount++;
    }
    else {
        _radiusInn = sqrt((pos.x() - _extra.x()) * (pos.x() - _extra.x())
                          + (pos.y() - _extra.y()) * (pos.y() - _extra.y()));

        _isMousePressed = false;
        _pressCount++;
    }
    draw();
}


void VGEHypocycloid::draw() {
    QColor drawColor;
    if (_isSelected){
        drawColor = QColor(0xFF - _color.red(), 0xFF - _color.green(), 0xFF - _color.blue(), 0xFF);
        if (drawColor == vge::BG_DEFAULT_COLOR){
            drawColor = vge::RED_COLOR;
        }
    }
    else {
        drawColor = _color;
    }

    _shapePoints->clear();

    if (_pressCount >= 2) {
        drawHypocycloid(_center, _radiusOut, _radiusInn, *_shapePoints);
    }
    else {
        bresenhamEllipse(_center, _radiusOut, *_shapePoints, false);
        if (_pressCount == 1) {
            // recount
            QPoint point(static_cast<int>(_center.x()), static_cast<int>((_center.y() - _radiusOut) + _radiusInn));
            bresenhamEllipse(point, _radiusInn, *_shapePoints, false);
        }
    }

    if (_raster) {
        delete _raster;
    }
    _raster = new VGERShape(_shapePoints, drawColor);
}


VGERShape& VGEHypocycloid::getRaster() {
    if (!_raster) {
        draw();
    }
    return *_raster;
}


QString VGEHypocycloid::str() const {
    std::string str = "HYPO" + std::to_string(_number);
    return QString(str.c_str());
}
