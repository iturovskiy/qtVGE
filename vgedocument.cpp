#include "vgedocument.h"


VGEDocument::VGEDocument(QWidget *parent) :
QWidget(parent),
_image(new QImage(vge::IMAGE_W, vge::IMAGE_H, QImage::Format_ARGB32_Premultiplied))
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


void VGEDocument::mousePressEvent(QMouseEvent *me) {
    if (me->button() == Qt::LeftButton){
        if (_mode == vge::DrawLine || _mode == vge::Move){
            emit sendMsgToUI("Нарисуйте линию", false);

            _tmpShape = new VGELine(this);
            _tmpShape->handleMousePressEvent(me);
            updateImage();

        }
        else if (_mode == vge::SelectShape) {
            searchPixel(me->pos());
            updateImage();
            if (!_selectedShapeList.empty()){
                emit sendMsgToUI("Выберите ещё фигуры", false);
            }
        }
        /* else if (_mode == QVP::drawEllipse){
            emit sendMsgToUI("Нарисуйте эллипс", false);

            m_tmpShape = new QVPEllipse(this);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();

        } elif (m_currentMode == QVP::drawEllipticCurve){
            emit sendMsgToUI("Нарисуйте эллипс и линиями ограничьте дугу", false);

            if (m_tmpShape == nullptr){
                m_tmpShape = new QVPEllipticArc(this);
            }
            m_tmpShape->handleMousePressEvent(me);
            updateImage();

        } elif (m_currentMode == QVP::makeOrtho){
            if (m_selectedShapesList.isEmpty()){
                previousWasFail = true;
                emit sendMsgToUI("Ничего не выбрано!", true);
                emit switchToSelection();
            }
            QVPLine * line = qobject_cast<QVPLine *>(m_selectedShapesList.last());
            if (line){
                emit sendMsgToUI("Нарисуйте линию ожидаемой длины", false);
                m_tmpShape = new QVPLine(this);
                m_tmpShape->handleMousePressEvent(me);
                updateImage();
            } else {
                previousWasFail = true;
                emit sendMsgToUI("Выбрана не линия", true);
                emit switchToSelection();

            }
        } elif (m_currentMode == QVP::clipRectangle){
            emit sendMsgToUI("Нарисуйте прямоугольник", false);

            m_tmpShape = new QVPLine(this, true);
            m_tmpShape->handleMousePressEvent(me);
            updateImage();
        }*/
    }
    qDebug() << "Shapes: " << _shapeList.size();
}


void VGEDocument::mouseMoveEvent(QMouseEvent *me) {
    emit updateCoord(me->pos());

        if (_mode == vge::DrawLine || _mode == vge::Move){
            if (me->buttons() & Qt::LeftButton){
                _tmpShape->handleMouseMoveEvent(me);
            }
            updateImage();
        }
        /*
         elif (m_currentMode == QVP::drawEllipticCurve && m_tmpShape != nullptr){
            qDebug() << "mme=" << me;
            m_tmpShape->handleMouseMoveEvent(me);
            updateImage();
        } */
}


void VGEDocument::mouseReleaseEvent(QMouseEvent *me) {
    if (_mode == vge::DrawLine  || _mode == vge::DrawEllipse || _mode == vge::DrawRectangle) {
            if (me->button() == Qt::LeftButton){
                _tmpShape->handleMouseReleaseEvent(me);
                _shapeList.append(_tmpShape);
                _tmpShape = nullptr;
            }
            updateImage();
        } else if (_mode == vge::Move){
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
        /* elif (m_currentMode == QVP::drawEllipticCurve){
            if (me->button() == Qt::LeftButton){
                m_tmpShape->handleMouseReleaseEvent(me);
                if (m_tmpShape->isReady()){
                    m_shapesList.append(m_tmpShape);
                    m_tmpShape = nullptr;
                }
            }
            updateImage();
        } elif (m_currentMode == QVP::makeOrtho){
            if (m_selectedShapesList.isEmpty()){
                previousWasFail = true;
                emit sendMsgToUI("Ничего не выбрано!", true);
                emit switchToSelection();
            }
            QVPLine* linePtr = qobject_cast<QVPLine *>(m_tmpShape);
            QPointF a = linePtr->getFirst();
            QPointF b = linePtr->getLast();
            double targetLenght = sqrt((b.x() - a.x())*(b.x() - a.x()) +
                                 (b.y() - a.y())*(b.y() - a.y()));
            QVPLine* selectedLinePtr = qobject_cast<QVPLine *>(m_selectedShapesList.last());
            QPointF sa = selectedLinePtr->getFirst();
            QPointF sb = selectedLinePtr->getLast();
            float x = sb.x() - sa.x();
            float y = sb.y() - sa.y();
            double lenght = sqrt(x*x + y*y);
            float scale = targetLenght/lenght;
            b.setX(a.x() + y * scale);
            b.setY(a.y() - x * scale);
            m_shapesList.append(new QVPLine(this, QVP::penColor, a, b));
            delete m_tmpShape;
            m_tmpShape = nullptr;
            updateImage();
            emit switchToSelection();
        } elif (m_currentMode == QVP::crossLine){
            QVPLine* linePtr = qobject_cast<QVPLine *>(m_tmpShape);
            QPointF a = linePtr->getFirst();
            QPointF b = linePtr->getLast();
            QStack<QVPShape*> removeStack;
            QList<QVPShape*> addList;
            for (auto shape : m_selectedShapesList){
                auto tmpList = shape->cutLine(a, b);
                if (!tmpList.isEmpty()){
                    removeStack.append(shape);
                    addList.append(tmpList);
                }
            }
            unSelect();

            while(!removeStack.isEmpty()){
                auto shape = removeStack.pop();
                delete shape;
                m_shapesList.removeAll(shape);
            }
            m_shapesList.append(addList);
            emit switchToSelection();
            updateImage();
        } elif (m_currentMode == QVP::clipRectangle){
            QVPLine* linePtr = qobject_cast<QVPLine *>(m_tmpShape);
            QPointF a = linePtr->getFirst();
            QPointF b = linePtr->getLast();
            QStack<QVPShape*> removeStack;
            QList<QVPShape*> addList;
            for (auto shape : m_selectedShapesList){
                auto tmpList = shape->cutRect(a, b);
                if (!tmpList.isEmpty()){
                    removeStack.append(shape);
                    addList.append(tmpList);
                }
            }
            unSelect();

            while(!removeStack.isEmpty()){
                auto shape = removeStack.pop();
                delete shape;
                m_shapesList.removeAll(shape);
            }
            m_shapesList.append(addList);
            emit switchToSelection();
            updateImage();
        } */
}


void VGEDocument::paintEvent(QPaintEvent *event)
{
    const QRect paintRect = event->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawImage(paintRect, *_image, paintRect);
}


// todo
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
        else if (!_actions) {
            VGEShape *shape = _selectedShapeList.last();
            // line
            if (qobject_cast<VGELine *>(shape)) {
                VGELine* line = qobject_cast<VGELine *>(shape);
                /*
                m_shapeActions = new QVPShapeActions(nullptr, QVP::line, shape->getColor(),
                                                     shape->getWidth(), line->getFirst(), line->getLast());

                connect(m_shapeActions, &QVPShapeActions::finished, this, &QVPDocument::acceptParamsClose);
                connect(m_shapeActions, &QVPShapeActions::updateShape, this, &QVPDocument::receiveParams);
                */
            }
            /*
            else if (qobject_cast<VGERectangle *>(shape)) {

            }
            else if (qobject_cast<VGEEllipse *>(shape)) {

            }
            else if (qobject_cast<VGEHypocycloid *>(shape)) {

            }
            */
        }
    }
    else if (_actions) {
        _actions->close();
        _actions->deleteLater();
        _actions = nullptr;
    }
}


void VGEDocument::acceptParamsClose() {
    if (_actions) {
        _actions->close();
        _actions->deleteLater();
        _actions = nullptr;
    }
    emit switchToSelection();
}


void VGEDocument::receiveParams(QColor color, QPointF first, QPointF last, float rOuter, float rInner) {
    VGEShape* shape = _selectedShapeList.last();
    shape->setColor(color);
    if (qobject_cast<VGELine *>(shape)){
        VGELine* line = qobject_cast<VGELine *>(shape);
        line->setFP(first);
        line->setLP(last);
    }
    /*
    else if (qobject_cast<QVPEllipse *>(shape)){
        QVPEllipse* ellipse = qobject_cast<QVPEllipse *>(shape);
        ellipse->setCenter(first);
        ellipse->setA(a);
        ellipse->setB(b);

    }
    else if (qobject_cast<QVPEllipticArc *>(shape)){
        QVPEllipticArc* arc = qobject_cast<QVPEllipticArc *>(shape);
        arc->setCenter(first);
        arc->setA(a);
        arc->setB(b);
        arc->setAng1(ang1);
        arc->setAng2(ang2);

    }
    */
    acceptParamsClose();
    updateImage();
}
