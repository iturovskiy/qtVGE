#ifndef CONFIGS_H
#define CONFIGS_H

#include <QColor>


namespace vge {

    // 150ppi A4
    static const int IMAGE_H = 1754;
    static const int IMAGE_W = 2480;

    // default window params
    static const int WINDOW_H = 400;
    static const int WINDOW_W = 600;

    static const int DEFAULT_WIDTH = 2;

    // default background color
    static const QColor BG_DEFAULT_COLOR(0xFF, 0xFF, 0xFF, 0xFF); // white

    // default pen color
    static const QColor SHAPE_DEFAULT_COLOR(0x00, 0x0, 0x0, 0xFF); // black

    // red
    static const QColor RED_COLOR(0xFF, 0x00, 0x00, 0xFF);

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

}

#endif // CONFIGS_H
