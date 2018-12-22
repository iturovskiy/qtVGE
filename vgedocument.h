#ifndef VGEDOCUMENT_H
#define VGEDOCUMENT_H

#include <QWidget>

#include "vgeshape.hpp"
#include "vgeline.h"
#include "vgeactions.h"


class VGEDocument : public QWidget {
    Q_OBJECT

public:
    explicit VGEDocument(QWidget *parent = nullptr);
    ~VGEDocument() override;


protected:
    void mousePressEvent(QMouseEvent* me) override;
    void mouseMoveEvent(QMouseEvent* me) override;
    void mouseReleaseEvent(QMouseEvent* me) override;
    void paintEvent(QPaintEvent* event) override;


private:
    void unSelect();
    void updateImage();
    void searchPixel(QPoint);


public slots:
    void setEditorMode(vge::editorMode emode);
    void receiveParams(QColor color, QPointF first, QPointF last, float rOuter, float rInner);
    void acceptParamsClose();


signals:
    void updateCoord(QPoint coord);
    void shapeSelected(bool state);
    void switchToSelection();
    void sendMsgToUI(QString text, bool isError);
    void showSetupWindow(QWidget* window);


private:
    QImage *_image;
    QList<VGEShape *> _shapeList;
    QList<VGEShape *> _selectedShapeList;
    VGEShape *_tmpShape = nullptr;
    VGEActions *_actions = nullptr;
    vge::editorMode _mode = vge::DrawLine;
    bool _previousWasFail = false;

};

#endif // VGEDOCUMENT_H
