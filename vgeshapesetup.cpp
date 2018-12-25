#include "vgeshapesetup.h"


QColor colorFrom884(quint8 eightBit) {
    quint8 red8 = eightBit & 224;
    red8 = (red8 >> 5) * 255 / 7;
    quint8 green8 = eightBit & 28;
    green8 = (green8 >> 2) * 255 / 7;
    quint8 blue4 = eightBit & 3;
    blue4 = blue4 * 255 / 3;
    QColor color(red8, green8, blue4, 0xFF);
    return color;
}


inline quint8 colorTo844(QColor color){
    return (color.red() * 7 / 255) << 5 | (color.green() * 7 / 255) << 2 | (color.blue() * 3 / 255);
}


QPixmap pixmapFromColor(QColor color) {
    QPixmap pixmap(32, 32);
    pixmap.fill(color);
    return pixmap;
}


// line & rectangle
VGEShapeSetUp::VGEShapeSetUp(QWidget *parent, vge::shapeType type, QColor color,
QPointF first, QPointF last) :
                                QMainWindow(parent),
                                _shapeType(type),
                                _color(color),
                                _newColor(color),
                                _colorButton(new ColorButton(colorTo844(color)))
{
    init(first);
    _firstCoordXLabel->setText("First point: X");
    _firstCoordYLabel->setText("First point: Y");
    _lastCoordXLabel = new QLabel(this);
    _lastCoordXLabel->setText("Last point: X");
    _gridLayout->addWidget(_lastCoordXLabel, 2, 1);

    _lastCoordXSpinBox = new QDoubleSpinBox(this);
    _lastCoordXSpinBox->setRange(0, 20000.0);
    _lastCoordXSpinBox->setValue(last.x());
    _gridLayout->addWidget(_lastCoordXSpinBox, 2, 2);

    _lastCoordYLabel = new QLabel(this);
    _lastCoordYLabel->setText("Last point: Y");
    _gridLayout->addWidget(_lastCoordYLabel, 2, 3);

    _lastCoordYSpinBox = new QDoubleSpinBox(this);
    _lastCoordYSpinBox->setRange(0, 20000.0);
    _lastCoordYSpinBox->setValue(last.y());
    _gridLayout->addWidget(_lastCoordYSpinBox, 2, 4);
}

// circle
VGEShapeSetUp::VGEShapeSetUp(QWidget *parent,
vge::shapeType type, QColor color, QPointF center,
qreal radius) : QMainWindow(parent),
                _shapeType(type),
                _color(color),
                _newColor(color),
                _colorButton(new ColorButton(colorTo844(color)))
{
    init(center);
    _firstCoordXLabel->setText("Center point: X");
    _firstCoordYLabel->setText("Center point: Y");

    _radiusOutLabel = new QLabel(this);
    _radiusOutLabel->setText("Radius: ");
    _gridLayout->addWidget(_radiusOutLabel, 2, 1);
    _radiusOutSpinBox = new QDoubleSpinBox(this);
    _radiusOutSpinBox->setRange(0, 10000.0);
    _radiusOutSpinBox->setValue(radius);
    _gridLayout->addWidget(_radiusOutSpinBox, 2, 2);
}

// hypo
VGEShapeSetUp::VGEShapeSetUp(QWidget *parent,
vge::shapeType type, QColor color, QPointF center,
qreal oradius, qreal iradius) : QMainWindow(parent),
                                _shapeType(type),
                                _color(color),
                                _newColor(color),
                                _colorButton(new ColorButton(colorTo844(color)))
{
    init(center);
    _firstCoordXLabel->setText("Center point: X");
    _firstCoordYLabel->setText("Center point: Y");

    _radiusOutLabel = new QLabel(this);
    _radiusOutLabel->setText("Radius Out: ");
    _gridLayout->addWidget(_radiusOutLabel, 2, 1);
    _radiusOutSpinBox = new QDoubleSpinBox(this);
    _radiusOutSpinBox->setRange(0, 10000.0);
    _radiusOutSpinBox->setValue(oradius);
    _gridLayout->addWidget(_radiusOutSpinBox, 2, 2);

    _radiusInnLabel = new QLabel(this);
    _radiusInnLabel->setText("Radius Inn: ");
    _gridLayout->addWidget(_radiusInnLabel, 3, 1);
    _radiusInnSpinBox = new QDoubleSpinBox(this);
    _radiusInnSpinBox->setRange(0, 10000.0);
    _radiusInnSpinBox->setValue(iradius);
    _gridLayout->addWidget(_radiusInnSpinBox, 3, 2);
}


void VGEShapeSetUp::init(QPointF point) {
    _cancelButton->setText("&Cancel");
    _confirmButton->setText("&Confirm");
    _gridLayout->addWidget(_confirmButton, 100, 3);
    _gridLayout->addWidget(_cancelButton, 100, 4);
    qDebug() << point;

    _colorLabel->setText("Color:");
    _gridLayout->addWidget(_colorLabel, 50, 1);
    _gridLayout->addWidget(_colorButton, 50, 2);
    connect(_colorButton, &QPushButton::clicked, this, &VGEShapeSetUp::showColorGrid);
    connect(_colorSelectionWidget, &ColorGridWidget::closed, this, &VGEShapeSetUp::hideColorGrid);

    QGridLayout* grid = new QGridLayout(_colorSelectionWidget);
    for (quint8 i = 0; i < 4; i++) {
        for (quint8 j = 0; j < 8; j++) {
            for (quint8 k = 0; k < 8; k++) {
                quint8 clr844 = (k << 5) | (j << 2) | i;
                ColorButton* btn = new ColorButton((k << 5) | (j << 2) | i);
                btn->setCheckable(true);
                if (colorTo844(_color) == clr844){
                    btn->setChecked(true);
                } else {
                    btn->setChecked(false);
                }
                int y = (j*(1 - 2*(i >> 1))%8 + 16 *  (i >> 1));
                int x = (k*(1 - 2*(i &  1))%8 + 16 * (i & 1));
                grid->addWidget(btn, x, y);
                _colorGridButtons << btn;
                connect(btn, &ColorButton::changeColor, this, &VGEShapeSetUp::setColor);
            }
        }
    }

    _colorCancelButton->setText("&Cancel");
    _colorConfirmButton->setText("&Confirm");
    grid->addWidget(_colorConfirmButton, 17, 11, 1, 3);
    grid->addWidget(_colorCancelButton, 17, 14, 1, 3);
    connect(_colorConfirmButton, &QPushButton::clicked, this, &VGEShapeSetUp::saveColorGrid);
    connect(_colorCancelButton, &QPushButton::clicked, this, &VGEShapeSetUp::hideColorGrid);

    _colorSelectionWidget->setLayout(grid);
    _colorSelectionWidget->setWindowTitle("Colors");

    _mainWgt->setLayout(_gridLayout);
    setCentralWidget(_mainWgt);
    setWindowTitle("Shape preferences");

    _firstCoordXLabel->setText("Default: X");
    _gridLayout->addWidget(_firstCoordXLabel, 1, 1);
    _firstCoordXSpinBox->setRange(0, 20000.0);
    _firstCoordXSpinBox->setValue(point.x());
    _gridLayout->addWidget(_firstCoordXSpinBox, 1, 2);

    _firstCoordYLabel->setText("Default: Y");
    _gridLayout->addWidget(_firstCoordYLabel, 1, 3);
    _firstCoordYSpinBox->setRange(0, 20000.0);
    _gridLayout->addWidget(_firstCoordYSpinBox, 1, 4);
    _firstCoordYSpinBox->setValue(point.y());

    _coefficientLabel->setText("Scale Coefficient:");
    _gridLayout->addWidget(_coefficientLabel, 1, 5);
    _coefficientSpinBox->setRange(0, 200.0);
    _gridLayout->addWidget(_coefficientSpinBox, 1, 6);
    _coefficientSpinBox->setValue(100.0);

    connect(_cancelButton, &QPushButton::clicked, this, &VGEShapeSetUp::cancel);
    connect(_confirmButton, &QPushButton::clicked, this, &VGEShapeSetUp::save);

    qDebug() << "x=" << _firstCoordXSpinBox->value() << " y=" << _firstCoordYSpinBox->value();
}


void VGEShapeSetUp::showColorGrid() {
    _colorConsistency = false;
    _colorSelectionWidget->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    _colorSelectionWidget->show();
}


void VGEShapeSetUp::hideColorGrid() {
    qDebug() << "hiding collor grid";
    _colorConsistency = true;
    _colorSelectionWidget->hide();
    _newColor = _color;
    quint8 color = colorTo844(_color);

    for (auto btn : _colorGridButtons){
        if (btn->getColor() != color){
            btn->setChecked(false);
        } else {
            btn->setChecked(true);
        }
    }
}


void VGEShapeSetUp::saveColorGrid() {
    qDebug() << "saving collor grid";
    _color = _newColor;
    _colorConsistency = true;
    _colorSelectionWidget->hide();
    _colorButton->setIcon(pixmapFromColor(_color));
}


void VGEShapeSetUp::setColor(quint8 color) {
    _newColor = colorFrom884(color);
    for (auto btn : _colorGridButtons){
        if (btn->getColor() != color){
            btn->setChecked(false);
        }
    }
}


void VGEShapeSetUp::save() {
    QPointF first(_firstCoordXSpinBox->value(), _firstCoordYSpinBox->value());
    QPointF last;
    if(_lastCoordYSpinBox && _lastCoordXSpinBox){
        last = QPointF(float(_lastCoordXSpinBox->value()), float(_lastCoordYSpinBox->value()));
    }
    hide();
    emit updateShape(_color, _coefficientSpinBox->value(), first, last,
                     _radiusOutSpinBox ? _radiusOutSpinBox->value() : 1.0,
                     _radiusInnSpinBox ? _radiusOutSpinBox->value() : 1.0);
    _coefficientSpinBox->setValue(100.00);
}


void VGEShapeSetUp::cancel() {
    hide();
    emit finished();
}





ColorButton::ColorButton(quint8 color) : m_color(color)
{
    setFlat(true);
    setIcon(QIcon(pixmapFromColor(colorFrom884(m_color))));
    connect(this, &QPushButton::toggled, this, &ColorButton::checkButton);
}
