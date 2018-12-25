#include "vgegroup.h"


VGEGroup::VGEGroup(QObject *parent, QColor color, QList<VGEShape *> shList, QString name = "newgroup") :
VGEShape(parent, color), _shList(std::move(shList)), _name(name) {
    draw();
}


void VGEGroup::move(QPointF displacement) {
    for (auto &item : _shList) {
        item->move(displacement);
    }
    draw();
}


void VGEGroup::scale(qreal coefficeint) {
    for (auto &item : _shList) {
        item->scale(coefficeint);
    }
    draw();
}


void VGEGroup::handleMousePressEvent(QMouseEvent * event) {
    for (auto &item : _shList) {
        item->handleMousePressEvent(event);
    }
    _isMousePressed = true;
    draw();
}


void VGEGroup::handleMouseMoveEvent(QMouseEvent * event) {
    for (auto &item : _shList) {
        item->handleMouseMoveEvent(event);
    }
    draw();
}


void VGEGroup::handleMouseReleaseEvent(QMouseEvent * event) {
    for (auto &item : _shList) {
        item->handleMouseReleaseEvent(event);
    }
    _isMousePressed = false;
    draw();
}


VGERShape& VGEGroup::getRaster() {
    if (!_raster) {
        draw();
    }
    return *_raster;
}


void VGEGroup::draw() {
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
    *_shapePoints = QVector<QPoint>::fromStdVector(shPoints);

    if (_raster) {
        delete _raster;
    }
    _raster = new VGERShape(_shapePoints, drawColor);
}


void VGEGroup::add(VGEShape * shape) {
    if (std::find(_shList.begin(), _shList.end(), shape) == _shList.end()) {
        _shList.append(shape);
    }
}


bool VGEGroup::del(size_t index) {
    size_t i = 0;
    for (auto it = _shList.begin(); it != _shList.end(); it++, i++) {
        if (i == index) {
            _shList.erase(it);
            return true;
        }
    }
    return false;
}


bool VGEGroup::del(QString name) {
    int index = 0;
    for (auto it : _shList) {
        if (name.compare(it->str()) == 0) {
            _shList.removeAt(index);
            return true;
        }
        index++;
    }
    return false;
}
