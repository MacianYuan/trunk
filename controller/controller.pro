#-------------------------------------------------
#
# Project created by QtCreator 2012-11-05T21:46:13
#
#-------------------------------------------------

QT += core gui network sql xml
QMAKE_CXXFLAGS += -std=c++11 -O0

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = controller
TEMPLATE = app
target.path=/storage/sd0/qtdebug
INSTALLS += target

FORMS += UI/AdminDialog.ui UI/MainWindow.ui UI/EntrustBeginDialog.ui UI/EntrustEndDialog.ui \
    UI/upgradingdialog.ui \
    UI/AddWlanDialog.ui
FORMS += UI/SettingsDialog.ui UI/TableViewDialog.ui

HEADERS += Base/ftp.h Base/GlobalData.h Base/NormalTools.h \
    UI/upgradingdialog.h \
    Context/userqualification.h \
    Base/imageview.h \
    UI/AddWlanDialog.h
HEADERS += Base/SmallDefine.h Context/AdminAction.h
HEADERS += Context/BookInformation.h Context/CardReader.h Context/CASConnection.h
HEADERS += Context/HeartBeatUsbNet.h Context/InstrumentStatus.h
HEADERS += Context/NetConnectionManage.h Context/PowerDelay.h Context/ReservationInformation.h
HEADERS += Context/SerialPort.h Context/ServiceEvent.h
HEADERS += Context/SessionManage.h Context/SupportFeature.h
HEADERS += Context/UsbNetConnection.h Context/UserInformation.h
HEADERS += Context/WindowContext.h Service/SslService.h
HEADERS += UI/AdminDialog.h UI/MainWindow.h
HEADERS += UI/MessageBox.h UI/ScheduleDialog.h
HEADERS += UI/ScreenSaver.h UI/SettingsDialog.h UI/QSqlQueryCheckboxModel.h
HEADERS += UI/TableViewDialog.h UI/EntrustBeginDialog.h UI/EntrustEndDialog.h

SOURCES += Base/ftp.cpp Base/GlobalData.cpp \
    UI/upgradingdialog.cpp \
    Context/userqualification.cpp \
    Base/imageview.cpp \
    UI/AddWanDialog.cpp
SOURCES += Base/NormalTools.cpp
SOURCES += Context/AdminAction.cpp Context/BookInformation.cpp
SOURCES += Context/CardReader.cpp Context/CASConnection.cpp
SOURCES += Context/HeartBeatUsbNet.cpp Context/InstrumentStatus.cpp
SOURCES += Context/NetConnectionManage.cpp Context/PowerDelay.cpp Context/ReservationInformation.cpp
SOURCES += Context/SerialPort.cpp Context/ServiceEvent.cpp
SOURCES += Context/SessionManage.cpp Context/SupportFeature.cpp
SOURCES += Context/UsbNetConnection.cpp Context/UserInformation.cpp
SOURCES += Service/SslService.cpp UI/AdminDialog.cpp
SOURCES += UI/MainWindow.cpp UI/MessageBox.cpp
SOURCES += UI/ScheduleDialog.cpp UI/ScreenSaver.cpp
SOURCES += UI/SettingsDialog.cpp UI/TableViewDialog.cpp UI/QSqlQueryCheckboxModel.cpp
SOURCES += Context/WindowContext.cpp UI/EntrustBeginDialog.cpp UI/EntrustEndDialog.cpp
SOURCES += main.cpp

RESOURCES += controller.qrc
TRANSLATIONS = controller_en_US.ts controller_zh_CN.ts

include(../3rdparty/qxt/qxt.pri)
include(../3rdparty/quazip/quazip.pri)
include(../3rdparty/qextserialport/src/qextserialport.pri)
include(../protobuf/message.pri)
include(../imframe/imframe.pri)
include(../splashframe/splashframe.pri)
include(../sslconnection/sslconnection.pri)
include(../sqlworker/sqlworker.pri)
include(../tools/tools.pri)
include(../object/object.pri)
include(../cmdparse/cmdparse.pri)
include(../deviceinfo/deviceinfo.pri)
include(../ftp/ftp.pri)
include (../upgrade/upgrade.pri)

INCLUDEPATH += $$PWD/.. $$PWD/../3rdparty $$PWD/../include
LIBS += -L$$PWD/../lib -lprotobuf-lite -lcurl
LIBS += -lssl -lcrypto -lts -llog4cplus -lboost_system

CODECFORTR = UTF-8

DISTFILES +=

