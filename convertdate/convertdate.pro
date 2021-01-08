#-------------------------------------------------
#
# Project created by QtCreator 2016-08-31T10:18:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11


TARGET = convertdate
TEMPLATE = app


SOURCES += main.cpp 


LIBS += -L$$PWD/../lib
LIBS += -lssl -lcrypto -lts

INCLUDEPATH += $$PWD/../include $$PWD/../deviceinfo $$PWD/..

TRANSLATIONS = controller_en_US.ts controller_zh_CN.ts
CODECFORTR = UTF-8

