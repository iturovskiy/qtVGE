#include "vgeline.h"

#include <QtMath>


VGELine::VGELine(QObject *parent, QColor color, QPointF firstPoint, QPointF lastPoint)
    : VGEShape(parent, std::move(color)),
      _firstPoint(std::move(firstPoint)),
      _lastPoint(std::move(lastPoint))
{
    _name = QString::fromStdString(std::string("line") + std::to_string(_number));
    draw();
}


void VGELine::move(const QPointF &displacement)
{
    _firstPoint.rx() += displacement.x();
    _firstPoint.ry() += displacement.y();
    _lastPoint.rx() += displacement.x();
    _lastPoint.ry() += displacement.y();
    clipMove(displacement);
    draw();
}


void VGELine::scale(qreal coefficeint)
{
    auto xmin = std::min<qreal>(_firstPoint.x(), _lastPoint.x());
    auto ymin = std::min<qreal>(_firstPoint.y(), _lastPoint.y());
    _firstPoint.rx() += (_firstPoint.x() - xmin) * (coefficeint - 1);
    _firstPoint.ry() += (_firstPoint.y() - ymin) * (coefficeint - 1);
    _lastPoint.rx() += (_lastPoint.x() - xmin) * (coefficeint - 1);
    _lastPoint.ry() += (_lastPoint.y() - ymin) * (coefficeint - 1);
    clipScale(coefficeint);
    draw();
}


void VGELine::handleMousePressEvent(QMouseEvent * event)
{
    _firstPoint = event->pos();
    _lastPoint = event->pos();
    _isMousePressed = true;
    draw();
}


void VGELine::handleMouseMoveEvent(QMouseEvent * event)
{
    _lastPoint = event->pos();
    draw();
}


void VGELine::handleMouseReleaseEvent(QMouseEvent * event)
{
    _lastPoint = event->pos();
    _isMousePressed = false;
    draw();
}


void VGELine::draw()
{
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


void VGELine::bresenhamLinePoints(const QPointF &fp, const QPointF &lp, QVector<QPoint> &line)
{
    int x2 = static_cast<int>(lp.x());
    int x1 = static_cast<int>(fp.x());
    int y2 = static_cast<int>(lp.y());
    int y1 = static_cast<int>(fp.y());
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;
    int error = deltaX - deltaY;
    QPoint point(x2,y2);
    if (clipContains(point)) {
        line.append(point);
    }
    while(x1 != x2 || y1 != y2) {
        point = QPoint(x1, y1);
        if (clipContains(point)) {
            line.append(point);
        }
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


VGERShape& VGELine::getRaster()
{
    if (!_raster) {
        draw();
    }
    return *_raster;
}
