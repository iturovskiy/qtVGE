#include "vgeshape.h"
#include "vgeline.h"


quint32 VGEShape::_number = 0;


void VGEShape::select(bool var)
{
    if (_isSelected != var) {
        _isSelected = var;
        draw();
    }
}


int VGEShape::test(const QPoint &point)
{
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


bool VGEShape::clipContains(const QPointF &point)
{
    if (!_clipped) {
        return true;
    }
    qreal maxX,maxY,minX,minY;
    maxX = std::max<qreal>(_cutFP.x(), _cutLP.x());
    minX = std::min<qreal>(_cutFP.x(), _cutLP.x());
    maxY = std::max<qreal>(_cutFP.y(), _cutLP.y());
    minY = std::min<qreal>(_cutFP.y(), _cutLP.y());
    return point.x() >= minX && point.x() <= maxX && point.y() >= minY && point.y() <= maxY;
}


void VGEShape::clip(const QPointF &fp, const QPointF &lp)
{
    if(!_clipped) {
        _cutFP = fp;
        _cutLP = lp;
        _clipped = true;
    }
    else {
        _cutFP = QPointF(std::max<qreal>(std::min<qreal>(_cutFP.x(), _cutLP.x()), std::min<qreal>(fp.x(), lp.x())),
                         std::max<qreal>(std::min<qreal>(_cutFP.y(), _cutLP.y()), std::min<qreal>(fp.y(), lp.y())));

        _cutLP = QPointF(std::min<qreal>(std::max<qreal>(_cutFP.x(), _cutLP.x()), std::max<qreal>(fp.x(), lp.x())),
                         std::min<qreal>(std::max<qreal>(_cutFP.y(), _cutLP.y()), std::max<qreal>(fp.y(), lp.y())));
    }
    draw();
}


void VGEShape::clipMove(const QPointF &displacement)
{
    if(_clipped) {
        _cutFP.rx() += displacement.x();
        _cutFP.ry() += displacement.y();
        _cutLP.rx() += displacement.x();
        _cutLP.ry() += displacement.y();
    }
}


void VGEShape::clipScale(qreal coefficeint)
{
    if (_clipped) {
        auto xmin = std::min<qreal>(_cutFP.x(), _cutLP.x());
        auto ymin = std::min<qreal>(_cutFP.y(), _cutLP.y());
        _cutFP.rx() += (_cutFP.x() - xmin) * (coefficeint - 1);
        _cutFP.ry() += (_cutFP.y() - ymin) * (coefficeint - 1);
        _cutLP.rx() += (_cutLP.x() - xmin) * (coefficeint - 1);
        _cutLP.ry() += (_cutLP.y() - ymin) * (coefficeint - 1);
    }
}


VGERShape::VGERShape(QVector<QPoint> *points, QColor color)
    : _points(points),
      _pen(std::move(color))
{
    _pen.setWidth(vge::DEFAULT_W);
}


void VGERShape::operator()(QImage *image) const
{
    if (!_points) {
        return;
    }
    QPainter painter(image);
    painter.setPen(_pen);
    painter.drawPoints(_points->data(), _points->size());
    painter.end();
}


/// bresenham

bool clipContains(const QPointF &point, bool clipped, const QPointF &cutFP, const QPointF &cutLP)
{
    if (!clipped) {
        return true;
    }
    qreal maxX,maxY,minX,minY;
    maxX = std::max<qreal>(cutFP.x(), cutLP.x());
    minX = std::min<qreal>(cutFP.x(), cutLP.x());
    maxY = std::max<qreal>(cutFP.y(), cutLP.y());
    minY = std::min<qreal>(cutFP.y(), cutLP.y());
    return point.x() >= minX && point.x() <= maxX && point.y() >= minY && point.y() <= maxY;
}


void bresenhamLine(const QPointF &fp, const QPointF &lp,
                   QVector<QPoint> &line,
                   bool clipped, const QPointF &cutFP, const QPointF &cutLP)
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
    if (clipContains(point, clipped, cutFP, cutLP)) {
        line.append(point);
    }
    while(x1 != x2 || y1 != y2) {
        point = QPoint(x1, y1);
        if (clipContains(point, clipped, cutFP, cutLP)) {
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


void bresenhamCirclePoints(const QPointF &center, qreal radius, QVector<QPoint> &circle)
{
    const int x0 = static_cast<int>(center.x());
    const int y0 = static_cast<int>(center.y());
    int x = 0;
    int y = static_cast<int>(radius);
    int delta = static_cast<int>(1 - 2 * radius);
    int error = 0;
    while(y >= 0) {
        circle.append(QPoint(x0 - x, y0 + y));
        circle.append(QPoint(x0 + x, y0 + y));
        circle.append(QPoint(x0 - x, y0 - y));
        circle.append(QPoint(x0 + x, y0 - y));

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
