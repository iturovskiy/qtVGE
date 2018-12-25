#ifndef VGEDOCUMENT_H
#define VGEDOCUMENT_H


#include <QWidget>

#include "vgeshape.h"
#include "vgeline.h"
#include "vgerectangle.h"
#include "vgecircle.h"
#include "vgehypocycloid.h"
#include "vgeshapesetup.h"


class VGEDocument : public QWidget {
    Q_OBJECT

public:
    explicit VGEDocument(QWidget *parent = nullptr);
    ~VGEDocument() override;


protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;


private:
    void unSelect();
    void updateImage();
    void searchPixel(QPoint);


public slots:
    void setEditorMode(vge::editorMode mode);
    void receiveParams(QColor color, qreal coef,
                       QPointF first, QPointF last,
                       qreal rOuter, qreal rInner);
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
    VGEShapeSetUp *_setupAction = nullptr;
    vge::editorMode _mode = vge::SelectShape;
    bool _previousWasFail = false;
    quint8 _hypoCount = 0;

};


#endif // VGEDOCUMENT_H
