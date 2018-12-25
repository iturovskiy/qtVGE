QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtVGE
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17
QMAKE_CXXFLAGS += -std=gnu++17


SOURCES += \
        main.cpp \
    vgeline.cpp \
    vgemainwindow.cpp \
    vgedocument.cpp \
    vgerectangle.cpp \
    vgehypocycloid.cpp \
    vgecircle.cpp \
    vgegroup.cpp \
    vgeshapesetup.cpp \
    vgeshape.cpp

HEADERS += \
    configs.h \
    vgeline.h \
    vgemainwindow.h \
    vgedocument.h \
    vgerectangle.h \
    vgehypocycloid.h \
    vgecircle.h \
    vgegroup.h \
    vgeshapesetup.h \
    vgeshape.h

FORMS +=
