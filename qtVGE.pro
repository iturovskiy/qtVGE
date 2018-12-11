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
    vgedocument.cpp

HEADERS += \
    configs.h \
    vgeline.h \
    vgeshape.hpp \
    vgemainwindow.h \
    vgedocument.h

FORMS += \
    vgemainwindow.ui
