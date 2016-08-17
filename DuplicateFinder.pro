#-------------------------------------------------
#
# Project created by QtCreator 2016-07-28T12:42:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DuplicateFinder
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    datamodel.cpp \
    dataitem.cpp

HEADERS  += mainwindow.h \
    datamodel.h \
    dataitem.h

FORMS    += mainwindow.ui

CONFIG  += c++11
