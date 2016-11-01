#-------------------------------------------------
#
# Project created by QtCreator 2015-12-11T13:14:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = untitled
TEMPLATE = app


SOURCES += main.cpp\
        controller.cpp \
    matrix.cpp \
    platform.cpp \
    mpoint.cpp

HEADERS  += controller.h \
    coordi.h \
    matrix.h \
    platform.h \
    plstate.h \
    mpoint.h

FORMS    += controller.ui
