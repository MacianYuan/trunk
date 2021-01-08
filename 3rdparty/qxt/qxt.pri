INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

DEFINES += QXT_STATIC

#HEADERS += $$PWD/qxtglobal.h
#HEADERS += $$PWD/qxtnamespace.h

HEADERS += $$PWD/qxtscheduleheaderwidget.h
HEADERS += $$PWD/qxtscheduleitemdelegate.h
HEADERS += $$PWD/qxtscheduleview.h
HEADERS += $$PWD/qxtscheduleviewheadermodel_p.h
HEADERS += $$PWD/qxtscheduleview_p.h
HEADERS += $$PWD/qxtstyleoptionscheduleviewitem.h

SOURCES += $$PWD/qxtscheduleitemdelegate.cpp
SOURCES += $$PWD/qxtscheduleview.cpp
SOURCES += $$PWD/qxtscheduleview_p.cpp
SOURCES += $$PWD/qxtscheduleviewheadermodel_p.cpp
SOURCES += $$PWD/qxtstyleoptionscheduleviewitem.cpp
SOURCES += $$PWD/qxtscheduleheaderwidget.cpp
