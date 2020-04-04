#include "vgehypocycloid.h"


#include <QtMath>


VGEHypocycloid::VGEHypocycloid(QObject *parent, QColor color, QPointF center,
                               qreal radiusOut, qreal radiusInn) : VGEShape(parent, color),
                                                                   _center(center),
                                                                   _radiusOut(radiusOut),
                                                                   _radiusInn(radiusInn)
{
    _name = QString::fromStdString(std::string("hypo") + std::to_string(count++));
    draw();
}


void VGEHypocycloid::move(QPointF displacement) {
    _center.rx() += displacement.x();
    _center.ry() += displacement.y();
    clipMove(displacement);
    draw();
}


void VGEHypocycloid::scale(qreal coefficeint) {
    auto xmin = _center.x() - _radiusOut;
    auto ymin = _center.y() - _radiusOut;
    _center.rx() += (_center.x() - xmin) * (coefficeint - 1);
    _center.ry() += (_center.y() - ymin) * (coefficeint - 1);
    _radiusOut *= coefficeint;
    _radiusInn *= coefficeint;
    clipScale(coefficeint);
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
    } else {
        drawColor = _color;
    }

    _shapePoints->clear();

    if (_pressCount >= 2) {
        hypocycloidPoints(*_shapePoints);
    }
    else {
        bresenhamCirclePoints(_center, _radiusOut, *_shapePoints);
        if (_pressCount == 1) {
            QPoint point(static_cast<int>(_center.x()), static_cast<int>((_center.y() - _radiusOut) + _radiusInn));
            bresenhamCirclePoints(point, _radiusInn, *_shapePoints);
        }
    }

    if (_raster) {
        delete _raster;
    }
    _raster = new VGERShape(_shapePoints, drawColor);
}

void VGEHypocycloid::hypocycloidPoints(QVector<QPoint> &hypo) {
    const int x0 = static_cast<int>(_center.x());
    const int y0 = static_cast<int>(_center.y());
    const double k = _radiusOut / _radiusInn;
    const double a = _radiusInn * (k - 1);
    int nod = NOD(_radiusOut, _radiusInn);
    double loop = _radiusInn / nod;
    double x, y;
    QPoint point;
    for (double f = 0; f < 2 * M_PI * loop; f += 0.001) {
        x = (a * (cos(f) + cos((k - 1.0) * f) / (k - 1.0)));
        y = (a * (sin(f) - sin((k - 1.0) * f) / (k - 1.0)));
        point = QPoint(x0 + x, y0 + y);
        if (clipContains(point)) {
            hypo.append(point);
        }
    }
}


int VGEHypocycloid::NOD(int a, int b) {
    while (a != b) {
        if (a > b)
            a -= b;
        else
            b -= a;
    }
    return a;
}


VGERShape& VGEHypocycloid::getRaster() {
    if (!_raster) {
        draw();
    }
    return *_raster;
}
