#-------------------------------------------------
#
# Project created by QtCreator 2016-07-21T11:21:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-gui
TEMPLATE = app
DESTDIR = ../bin

LIBS += -L../../engine/lib -lengine

INCLUDEPATH += ../../engine/src

SOURCES += main.cpp\
        mainwindow.cpp \
    qpoolengine.cpp \
    qpoolobject.cpp \
    qpoolellipseobject.cpp \
    qpoolrectobject.cpp \
    engineticker.cpp \
    scenerefresher.cpp

HEADERS  += mainwindow.h \
    qpoolengine.h \
    qpoolobject.h \
    qpoolellipseobject.h \
    qpoolrectobject.h \
    engineticker.h \
    scenerefresher.h \
    customqgraphicsview.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11 -Wno-unused-parameter
