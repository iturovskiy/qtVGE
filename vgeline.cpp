#include "vgeline.h"


void bresenhamLine(const QPointF &fp, const QPointF &lp, QVector<QPoint> &line) {

    int x2 = static_cast<int>(lp.x());
    int x1 = static_cast<int>(fp.x());
    int y2 = static_cast<int>(lp.y());
    int y1 = static_cast<int>(fp.y());
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;

    line.append(QPoint(x2, y2));
    while(x1 != x2 || y1 != y2) {
        line.append(QPoint(x1, y1));

        const int error2 = error * 2;

        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}


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

// redo
QString VGELine::str() const {
    std::string str;
    return QString(str.c_str());
}
