#include "vgegroup.h"
#include "vgeshape.h"


VGEGroup::VGEGroup(QObject *parent, QColor color, QList<VGEShape *> shList)
    : VGEShape(parent, std::move(color)),
      _shList(std::move(shList))
{
    _name = QString::fromStdString(std::string("newgroup") + std::to_string(_number));
    draw();
}


void VGEGroup::clip(const QPointF &fp, const QPointF &lp)
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
    for (auto item : _shList) {
        item->clip(fp, lp);
    }
    draw();
}


void VGEGroup::move(const QPointF &displacement)
{
    for (auto item : _shList) {
        item->move(displacement);
    }
    draw();
}


void VGEGroup::scale(qreal coefficeint)
{
    for (auto item : _shList) {
        item->scale(coefficeint);
    }
    draw();
}


void VGEGroup::handleMousePressEvent(QMouseEvent * event)
{
    for (auto item : _shList) {
        item->handleMousePressEvent(event);
    }
    _isMousePressed = true;
    draw();
}


void VGEGroup::handleMouseMoveEvent(QMouseEvent * event)
{
    for (auto item : _shList) {
        item->handleMouseMoveEvent(event);
    }
    draw();
}


void VGEGroup::handleMouseReleaseEvent(QMouseEvent * event)
{
    for (auto item : _shList) {
        item->handleMouseReleaseEvent(event);
    }
    _isMousePressed = false;
    draw();
}


VGERShape& VGEGroup::getRaster()
{
    if (!_raster) {
        draw();
    }
    return *_raster;
}


void VGEGroup::draw()
{
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
    std::vector<QPoint> shPoints;
    for (auto &item : _shList) {
        shPoints.insert(shPoints.end(), item->getPoints()->begin(), item->getPoints()->end());
    }
    *_shapePoints = QVector<QPoint>(shPoints.begin(), shPoints.end());

    if (_raster) {
        delete _raster;
    }
    _raster = new VGERShape(_shapePoints, drawColor);
}


void VGEGroup::add(VGEShape * shape)
{
    if (std::find(_shList.begin(), _shList.end(), shape) == _shList.end()) {
        _shList.append(shape);
    }
}



void VGEGroup::del(const QString &name)
{
    int index = 0;
    for (auto it : _shList) {
        if (name.compare(it->getName()) == 0) {
            _shList.removeAt(index);
            break;
        }
        index++;
    }
    draw();
}
