#include "vgedocument.h"


VGEDocument::VGEDocument(QWidget *parent) :
QWidget(parent), _image(new QImage(vge::IMAGE_W, vge::IMAGE_H, QImage::Format_ARGB32_Premultiplied))
{
    qDebug() << __FUNCTION__;
    _image->fill(vge::BG_DEFAULT_COLOR);
    resize(vge::IMAGE_W, vge::IMAGE_H);
    setMouseTracking(true);
    emit shapeSelected(false);
}


VGEDocument::~VGEDocument() {
    for (auto shape : _shapeList) {
        delete shape;
    }
    if (_tmpShape) {
        delete _tmpShape;
    }
    if (_image) {
        delete _image;
    }
}


void VGEDocument::unSelect() {
    for (auto shape : _selectedShapeList) {
        shape->select(false);
    }
    _selectedShapeList.clear();
    emit shapeSelected(false);
}


void VGEDocument::updateImage() {
    _image->fill(vge::BG_DEFAULT_COLOR);
    for (auto shape : _shapeList) {
        shape->getRaster()(_image);
    }
    if (_tmpShape) {
        _tmpShape->getRaster()(_image);
    }
    update(_image->rect());
}


void VGEDocument::searchPixel(QPoint point) {
    int minDist = 2 * vge::SEARCH_W * vge::SEARCH_W;
    VGEShape * selectedShape = nullptr;
    for (auto shape : _shapeList){
        int dist = shape->test(point);
        if (dist < minDist){
            selectedShape = shape;
            minDist = dist;
        }
    }
    if (selectedShape && !selectedShape->isSelected()){
        selectedShape->select(true);
        _selectedShapeList.append(selectedShape);
        emit shapeSelected(true);
    }
    else if (selectedShape) {
        selectedShape->select(false);
        _selectedShapeList.removeAll(selectedShape);
        if(_selectedShapeList.empty())
            emit shapeSelected(false);
    }
    update();
}

/// todo tangent
void VGEDocument::mousePressEvent(QMouseEvent *event) {
    qDebug() << "Document - mouse PRESS; mod: " << _mode;
    if (event->button() == Qt::LeftButton){
        if (_mode == vge::SelectShape) {
            emit sendMsgToUI("Выберите фигуру", false);
            searchPixel(event->pos());
            updateImage();
            if (!_selectedShapeList.empty()){
                emit sendMsgToUI("Выберите ещё фигуры", false);
            }
        }
        else if (_mode == vge::DeleteShape) {
            emit sendMsgToUI("Выберите фигуру для удаления", false);
            unSelect();
            searchPixel(event->pos());
            if (!_selectedShapeList.empty()){
                _shapeList.removeAll(_selectedShapeList.last());
                unSelect();
            }
            updateImage();
        }
        else if (_mode == vge::DrawLine || _mode == vge::Move){
            emit sendMsgToUI("Нарисуйте линию", false);
            _tmpShape = new VGELine(this);
            _tmpShape->handleMousePressEvent(event);
            updateImage();
        }
        else if (_mode == vge::DrawRectangle){
            emit sendMsgToUI("Нарисуйте прямоугольник", false);
            _tmpShape = new VGERectangle(this);
            _tmpShape->handleMousePressEvent(event);
            updateImage();
        }
        else if (_mode == vge::Clipping){
            emit sendMsgToUI("Нарисуйте прямоугольную область", false);
            VGERectangle  * rect = new VGERectangle(this);
            rect->change();
            _tmpShape = qobject_cast<VGEShape *>(rect);
            _tmpShape->handleMousePressEvent(event);
            updateImage();
        }
        else if (_mode == vge::DrawCircle) {
            emit sendMsgToUI("Проведите радиус", false);
            _tmpShape = new VGECircle(this);
            _tmpShape->handleMousePressEvent(event);
            updateImage();
        }
        else if (_mode == vge::DrawHypocycloid) {
            emit sendMsgToUI("Проведите радиус", false);
            if (_hypoCount == 0) {
                _tmpShape = new VGEHypocycloid(this);
            }
            _tmpShape->handleMousePressEvent(event);
            updateImage();
        }
        else if (_mode == vge::MakeTangent) {
            if (_selectedShapeList.isEmpty()){
                _previousWasFail = true;
                emit sendMsgToUI("Ничего не выбрано!", true);
                emit switchToSelection();
            }
            else if (_selectedShapeList.size() != 2) {
                _previousWasFail = true;
                emit sendMsgToUI("Выбрано недостаточно окружностей", true);
                emit switchToSelection();
            }
            else {
                VGECircle * circl1 = qobject_cast<VGECircle *>(_selectedShapeList.first());
                VGECircle * circl2 = qobject_cast<VGECircle *>(_selectedShapeList.last());
                if (circl1 && circl2) {
                    auto points = tangent(circl1, circl2, event->pos());
                    VGELine *line = new VGELine(this);
                    line->setFP(points.first);
                    line->setLP(points.second);
                    _shapeList.append(line);
                    updateImage();
                }
                else {
                    _previousWasFail = true;
                    emit sendMsgToUI("Выбраны не окружности", true);
                    emit switchToSelection();
                }
            }
        }
    }
    qDebug() << "Shapes: " << _shapeList.size();
}


void VGEDocument::mouseMoveEvent(QMouseEvent *event) {
    emit updateCoord(event->pos());
    // qDebug() << "Document - mouse MOVE; mod: " << _mode;
    if (_mode == vge::Move || _mode == vge::DrawLine || _mode == vge::DrawRectangle ||
        _mode == vge::DrawCircle || _mode == vge::DrawHypocycloid || _mode == vge::Clipping)
    {
        if (event->buttons() & Qt::LeftButton){
            _tmpShape->handleMouseMoveEvent(event);
        }
        updateImage();
    }
}

/// todo tangent
void VGEDocument::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << "Document - mouse RELEASE; mod: " << _mode;
    if (_mode == vge::DrawLine   || _mode == vge::DrawRectangle || _mode == vge::DrawCircle) {
        if (event->button() == Qt::LeftButton){
            _tmpShape->handleMouseReleaseEvent(event);
            _shapeList.append(_tmpShape);
            _tmpShape = nullptr;
        }
        updateImage();
    }
    else if(_mode == vge::DrawHypocycloid) {
        if (_hypoCount == 0) {
             _tmpShape->handleMouseReleaseEvent(event);
             _hypoCount++;
        }
        else {
            _tmpShape->handleMouseReleaseEvent(event);
            _shapeList.append(_tmpShape);
            _tmpShape = nullptr;
            _hypoCount = 0;
        }
        updateImage();
    }
    else if (_mode == vge::Move) {
        VGELine* linePtr = qobject_cast<VGELine *>(_tmpShape);
        QPointF a = linePtr->getFP();
        QPointF b = linePtr->getSP();
        QPointF vec(b.x() - a.x(), b.y() - a.y());
        for (auto shape : _selectedShapeList){
            shape->move(vec);
        }
        delete _tmpShape;
        _tmpShape = nullptr;
        updateImage();
    }
    else if (_mode == vge::Clipping) {
        // todo
        VGERectangle *rect = qobject_cast<VGERectangle *>(_tmpShape);
        QList<VGEShape *> stayList = rect->clip();
        QList<VGEShape *> delList = {};
        for (auto &item :  _shapeList) {
            for (auto it : stayList) {
                if (item->str().compare(it->str()) != 0) {
                    delList.append(item);
                }
            }
        }
        delList.clear();
        emit switchToSelection();
        updateImage();
    }
}


void VGEDocument::paintEvent(QPaintEvent *event)
{
    const QRect paintRect = event->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawImage(paintRect, *_image, paintRect);
}


void VGEDocument::setEditorMode(vge::editorMode mode) {
    qDebug() << __FUNCTION__;

    _mode = mode;
    if (_mode == vge::SelectShape && !_previousWasFail) {
        emit sendMsgToUI("Выберите фигуру", false);
    }
    _previousWasFail = false;
    if (_tmpShape) {
        delete _tmpShape;
        _tmpShape = nullptr;
    }
    updateImage();

    if (_mode == vge::SetUp) {
        if (_selectedShapeList.empty()) {
            _previousWasFail = true;
            emit switchToSelection();
        }
        else if (!_setupAction) {
            VGEShape *shape = _selectedShapeList.last();
            // line
            if (qobject_cast<VGELine *>(shape)) {
                VGELine *line = qobject_cast<VGELine *>(shape);
                _setupAction = new VGEShapeSetUp(nullptr, vge::Line, shape->getColor(), line->getFP(), line->getSP());
                connect(_setupAction, &VGEShapeSetUp::finished, this, &VGEDocument::acceptParamsClose);
                connect(_setupAction, &VGEShapeSetUp::updateShape, this, &VGEDocument::receiveParams);
                _setupAction->show();
            }
            // rectangle
            else if (qobject_cast<VGERectangle *>(shape)) {
                VGERectangle *rect = qobject_cast<VGERectangle *>(shape);
                _setupAction = new VGEShapeSetUp(nullptr, vge::Rectangle, shape->getColor(), rect->getFP(), rect->getSP());
                connect(_setupAction, &VGEShapeSetUp::finished, this, &VGEDocument::acceptParamsClose);
                connect(_setupAction, &VGEShapeSetUp::updateShape, this, &VGEDocument::receiveParams);
                _setupAction->show();
            }
            // circle
            else if (qobject_cast<VGECircle *>(shape)) {
                VGECircle *circle = qobject_cast<VGECircle *>(shape);
                _setupAction = new VGEShapeSetUp(nullptr, vge::Circle, shape->getColor(),
                                        circle->getCenter(), circle->getRadius());
                connect(_setupAction, &VGEShapeSetUp::finished, this, &VGEDocument::acceptParamsClose);
                connect(_setupAction, &VGEShapeSetUp::updateShape, this, &VGEDocument::receiveParams);
                _setupAction->show();
            }
            // hypocycloid
            else if (qobject_cast<VGEHypocycloid *>(shape)) {
                VGEHypocycloid *hypo = qobject_cast<VGEHypocycloid *>(shape);
                _setupAction = new VGEShapeSetUp(nullptr, vge::Hypocycloid, shape->getColor(),
                                        hypo->getCenter(), hypo->getRadiusOut(), hypo->getRadiusInn());
                connect(_setupAction, &VGEShapeSetUp::finished, this, &VGEDocument::acceptParamsClose);
                connect(_setupAction, &VGEShapeSetUp::updateShape, this, &VGEDocument::receiveParams);
                _setupAction->show();
            }
        }
    }
    else if (_setupAction) {
        _setupAction->close();
        _setupAction->deleteLater();
        _setupAction = nullptr;
    }
}


void VGEDocument::acceptParamsClose() {
    if (_setupAction) {
        _setupAction->close();
        _setupAction->deleteLater();
        _setupAction = nullptr;
    }
    emit switchToSelection();
}


void VGEDocument::receiveParams(QColor color, qreal coef, QPointF first, QPointF last, qreal rOuter, qreal rInner) {
    VGEShape *shape = _selectedShapeList.last();
    shape->setColor(color);
    if (qobject_cast<VGELine *>(shape)){
        auto *line = qobject_cast<VGELine *>(shape);
        line->setFP(first);
        line->setLP(last);
        if (coef != 100.00) {
            line->scale(coef / 100);
        }
    }
    else if (qobject_cast<VGERectangle *>(shape)){
        auto *rect = qobject_cast<VGERectangle *>(shape);
        rect->setFP(first);
        rect->setLP(last);
        if (coef != 100.00) {
            rect->scale(coef / 100);
        }
    }
    else if (qobject_cast<VGECircle *>(shape)){
        auto *circle = qobject_cast<VGECircle *>(shape);
        circle->setCenter(first);
        circle->setRadius(rOuter);
        if (coef != 100.00) {
            circle->scale(coef / 100);
        }
    }
    else if (qobject_cast<VGEHypocycloid *>(shape)){
        auto *hypo = qobject_cast<VGEHypocycloid *>(shape);
        hypo->setCenter(first);
        hypo->setRadiusOut(rOuter);
        hypo->setRadiusInn(rInner);
        if (coef != 100.00) {
            hypo->scale(coef / 100);
        }
    }
    acceptParamsClose();
    updateImage();
}
