#ifndef CONFIGS_H
#define CONFIGS_H

#include <QColor>


namespace vge {

    // 150ppi A4
    static const int IMAGE_H = 1754;
    static const int IMAGE_W = 2480;

    // default window params
    static const int WINDOW_H = 720;
    static const int WINDOW_W = 1024;

    static const int DEFAULT_W = 2;
    static const int SEARCH_W = 6;

    // default background color
    static const QColor BG_DEFAULT_COLOR(0xFF, 0xFF, 0xFF, 0xFF); // white

    // default pen color
    static const QColor SHAPE_DEFAULT_COLOR(0x00, 0x0, 0x0, 0xFF); // black

    // red
    static const QColor RED_COLOR(0xFF, 0x00, 0x00, 0xFF);
    static const QColor PUR_COLOR(0xFF, 0x00, 0xFF, 0xFF);

    // modes
    enum editorMode{
        SelectShape,
        DrawLine,
        DrawRectangle,
        DrawCircle,
        DrawHypocycloid,
        MakeGroup,
        DeleteShape,
        Move,
        Scaling,
        Clipping,
        MakeTangent,
        SetUp
    };

    // shapes
    enum shapeType{
        Line,
        Circle,
        Rectangle,
        Hypocycloid,
        Group
    };

}

#endif // CONFIGS_H
