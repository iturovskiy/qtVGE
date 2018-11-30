#ifndef CONFIGS_H
#define CONFIGS_H

#include <QColor>


namespace vge {

    // 150ppi A4
    const int IMAGE_H = 1754;
    const int IMAGE_W = 2480;

    // default window params
    const int WINDOW_H = 400;
    const int WINDOW_W = 600;

    // modes
    enum editorMode{
        DrawLine,
        DrawEllipse,
        DrawRectangle,
        DrawHypocycloid,
        SelectShape,
        MakeGroup,
        DeleteShape,
        EraseShape,
        ChangeParams,
        Move,
        Scaling,
        Clipping,
        MakeTangent
    };

    // shapes
    enum shapeType{
        Line,
        Ellipse,
        Rectangle,
        Hypocycloid
    };

    const int DEFAULT_WIDTH = 2;

    // default background color
    const QColor BG_DEFAULT_COLOR(0xFF, 0xFF, 0xFF, 0xFF); // white

    // default pen color
    const QColor SHAPE_DEFAULT_COLOR(0x00, 0x0, 0x0, 0xFF); // black

}

#endif // CONFIGS_H
