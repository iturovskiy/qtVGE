#include "vgecircle.h"

#include <QtMath>

/// tangent to 2 circle
QPair<QPoint, QPoint> tangent(VGECircle *circle1, VGECircle *circle2, QPointF pos) {
    auto c1 = circle1->getCenter();
    auto c2 = circle2->getCenter();
    float x1, x2, y1, y2;

    double r = circle1->getRadius() + circle2->getRadius();
    double z = std::pow(c1.x() - c2.x(), 2) + std::pow(c1.y() - c2.y(), 2);
    double d = sqrt (abs(z - r * r));
    double a = ((c1.x() - c2.x()) * r + (c1.y() - c2.y()) * d) / z;
    double b = ((c1.y() - c2.y()) * r - (c1.x() - c2.x()) * d) / z;
    double c = circle1->getRadius() - a * c1.x() - b * c1.y();
    x1 = 0;
    y1 = static_cast<float>(-c / b);
    x2 = vge::IMAGE_W;
    y2 = static_cast<float>((-c - a * x2) / b);
    double d0 = abs(a * pos.x() + b * pos.y() + c) / sqrt(a * a + b * b);
    double d1;

    r = circle1->getRadius() - circle2->getRadius();
    z = std::pow(c1.x() - c2.x(), 2) + std::pow(c2.y() - c2.y(), 2);
    d = sqrt(abs(z - r * r));
    a = ((c1.x() - c2.x()) * r + (c1.y() - c2.y()) * d) / z;
    b = ((c1.y() - c2.y()) * r - (c1.x() - c2.x()) * d) / z;
    c = circle1->getRadius() - a * c1.x() - b * c1.y();
    d1 = abs(a * pos.x() + b * pos.y() + c) / sqrt(a * a + b * b);
    if (d0 > d1) {
        d0 = d1;
        x1 = 0;
        y1 = static_cast<float>(-c / b);
        x2 = vge::IMAGE_W;
        y2 = static_cast<float>((-c - a * x2) / b);
    }

    r = -circle1->getRadius() + circle2->getRadius();
    z = std::pow(c1.x() - c2.x(), 2) + std::pow(c2.y() - c2.y(), 2);
    d = sqrt(abs(z - r * r));
    a = ((c1.x() - c2.x()) * r + (c1.y() - c2.y()) * d) / z;
    b = ((c1.y() - c2.y()) * r - (c1.x() - c2.x()) * d) / z;
    c = -circle1->getRadius() - a * c1.x() - b * c1.y();
    d1 = abs(a * pos.x() + b * pos.y() + c) / sqrt(a * a + b * b);
    if (d0 > d1) {
        d0 = d1;
        x1 = 0;
        y1 = static_cast<float>(-c / b);
        x2 = vge::IMAGE_W;
        y2 = static_cast<float>((-c - a * x2) / b);
    }

    r = -circle1->getRadius() - circle2->getRadius();
    z = std::pow(c1.x() - c2.x(), 2) + std::pow(c2.y() - c2.y(), 2);
    d = sqrt(abs(z - r * r));
    a = ((c1.x() - c2.x()) * r + (c1.y() - c2.y()) * d) / z;
    b = ((c1.y() - c2.y()) * r - (c1.x() - c2.x()) * d) / z;
    c = -circle1->getRadius() - a * c1.x() - b * c1.y();
    d1 = abs(a * pos.x() + b * pos.y() + c) / sqrt(a * a + b * b);
    if (d0 > d1) {
        d0 = d1;
        x1 = 0;
        y1 = static_cast<float>(-c / b);
        x2 = vge::IMAGE_W;
        y2 = static_cast<float>((-c - a * x2) / b);
    }
    int ix0 = static_cast<int>(round(x1));
    int ix1 = static_cast<int>(round(x2));
    int iy0 = static_cast<int>(round(y1));
    int iy1 = static_cast<int>(round(y2));
    return qMakePair(QPoint(ix0, iy0), QPoint(ix1, iy1)); // { ..., ... }
}


VGECircle::VGECircle(QObject *parent, QColor color, QPointF center, qreal radius) :
VGEShape(parent, color), _center(center),_radius(radius) {
    _name = QString::fromStdString(std::string("circle") + std::to_string(count++));
    draw();
}


void VGECircle::move(QPointF displacement) {
    _center.rx() += displacement.x();
    _center.ry() += displacement.y();
    clipMove(displacement);
    draw();
}


void VGECircle::scale(qreal coefficeint) {
    auto xmin = _center.x() - _radius;
    auto ymin = _center.y() - _radius;
    _center.rx() += (_center.x() - xmin) * (coefficeint - 1);
    _center.ry() += (_center.y() - ymin) * (coefficeint - 1);
    _radius *= coefficeint;
    clipScale(coefficeint);
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
    bresenhamCircle(*_shapePoints);

    if (_raster) {
        delete _raster;
    }
    _raster = new VGERShape(_shapePoints, drawColor);
}

void VGECircle::bresenhamCircle(QVector<QPoint> &circle) {
    const int x0 = static_cast<int>(_center.x());
    const int y0 = static_cast<int>(_center.y());
    int x = 0;
    int y = static_cast<int>(_radius);
    int delta = static_cast<int>(1 - 2 * _radius);
    int error = 0;
    while(y >= 0) {
        bresenhamLine(QPoint(x0 - x, y0 + y), QPoint(x0 + x, y0 + y), circle, _clipped, _cutFP, _cutLP);
        bresenhamLine(QPoint(x0 - x, y0 - y), QPoint(x0 + x, y0 - y), circle, _clipped, _cutFP, _cutLP);
        error = 2 * (delta + y) - 1;
        if (delta < 0 && error <= 0) {
            ++x;
            delta += 2 * x + 1;
            continue;
        }
        error = 2 * (delta - x) - 1;
        if (delta > 0 && error > 0) {
            --y;
            delta += 1 - 2 * y;
            continue;
        }
        ++x;
        delta += 2 * (x - y);
        --y;
    }
}


VGERShape& VGECircle::getRaster() {
    if (!_raster) {
        draw();
    }
    return *_raster;
}
