#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T10:18:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11


TARGET = audioTest
TEMPLATE = app


SOURCES += main.cpp mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS += -L$$PWD/../lib
LIBS += -lssl -lcrypto -lts
LIBS += -llog4cplus

INCLUDEPATH += $$PWD/../include $$PWD/..

TRANSLATIONS = controller_en_US.ts controller_zh_CN.ts
CODECFORTR = UTF-8

