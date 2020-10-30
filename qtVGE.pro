QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtVGE
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17
QMAKE_CXXFLAGS += -std=gnu++17


SOURCES += \
    main.cpp \
    shapes/vgeshape.cpp \
    shapes/vgeline.cpp \
    shapes/vgerectangle.cpp \
    shapes/vgehypocycloid.cpp \
    shapes/vgecircle.cpp \
    shapes/vgegroup.cpp \
    gui/vgemainwindow.cpp \
    gui/vgedocument.cpp \
    gui/vgeshapesetup.cpp \
    gui/treewidget.cpp 

HEADERS += \
    configs.h \
    shapes/vgeshape.h \
    shapes/vgeline.h \
    shapes/vgerectangle.h \
    shapes/vgehypocycloid.h \
    shapes/vgecircle.h \
    shapes/vgegroup.h \
    gui/vgemainwindow.h \
    gui/vgedocument.h \
    gui/vgeshapesetup.h \
    gui/treewidget.h 

FORMS += \
    gui/widget.ui
