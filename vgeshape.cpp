#include "vgeshape.h"

void VGEShape::select(bool var) {
    if (_isSelected != var) {
        _isSelected = var;
        draw();
    }
}


int VGEShape::test(QPoint point) {
    int min = 2 * vge::SEARCH_W * vge::SEARCH_W;
    for (QPoint shapePoint : *_shapePoints){
        int distX = abs(shapePoint.x() - point.x());
        int distY = abs(shapePoint.y() - point.y());
        if (distX < vge::SEARCH_W && distY < vge::SEARCH_W) {
            int square =  distX * distX + distY * distY;
            min = min < square ? min : square;
        }
    }
    return min;
}


VGERShape::VGERShape(QVector<QPoint> *points, QColor color, QVector<QPoint> *keyPoints) :
_points(points), _pen(color), _keyPoints(keyPoints), _keyPen(QColor(0xFF, 0xFF, 0, 0xFF)) {
    _pen.setWidth(vge::DEFAULT_W);
    _keyPen.setWidth(vge::DEFAULT_W * 5);
}


void VGERShape::operator()(QImage *image) const {
    if (!_points) {
        return;
    }
    QPainter painter(image);
    painter.setPen(_pen);
    painter.drawPoints(_points->data(), _points->size());
    if(_keyPoints) {
        painter.setPen(_keyPen);
        painter.drawPoints(_keyPoints->data(), _keyPoints->size());
    }
    painter.end();
}


/// bresenham

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


void bresenhamEllipse(const QPointF &center, qreal radius, QVector<QPoint> &ellipse, bool filled) {
    const int x0 = static_cast<int>(center.x());
    const int y0 = static_cast<int>(center.y());
    int x = 0;
    int y = static_cast<int>(radius);
    int delta = static_cast<int>(1 - 2 * radius);
    int error = 0;
    while(y >= 0) {

        if (filled) {
            bresenhamLine(QPoint(x0 - x, y0 + y), QPoint(x0 + x, y0 + y), ellipse);
            bresenhamLine(QPoint(x0 - x, y0 - y), QPoint(x0 + x, y0 - y), ellipse);
        }
        else {
            ellipse.append(QPoint(x0 - x, y0 + y));
            ellipse.append(QPoint(x0 + x, y0 + y));
            ellipse.append(QPoint(x0 - x, y0 - y));
            ellipse.append(QPoint(x0 + x, y0 - y));
        }

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


void drawHypocycloid(const QPointF &center, qreal radiusOut, qreal radiusInn, QVector<QPoint> &hypo) {
    const int x0 = static_cast<int>(center.x());
    const int y0 = static_cast<int>(center.y());
    const int R  = static_cast<int>(radiusOut);
    const int r  = static_cast<int>(radiusInn);
    const double k = R / r;
    int x, y;
    for (double f = -M_PI; f <= M_PI; f += 0.001) {
        x = static_cast<int>(r * ((k - 1) * cos(f) + cos((k - 1) * f)));
        y = static_cast<int>(r * ((k - 1) * sin(f) - sin((k - 1) * f)));
        hypo.append(QPoint(x0 + x, y0 + y));
    }
}
