INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += $$PWD/AccountList.h $$PWD/Command.h $$PWD/InstrumentDetail.h \ 
    $$PWD/upgradeparams.h \
    $$PWD/upgradeplan.h \
    $$PWD/tinsqualist.h
HEADERS += $$PWD/SyncFtpEvent.h $$PWD/SyncUserEvent.h $$PWD/Unbinding.h
HEADERS += $$PWD/BookingList.h $$PWD/GlobalConfig.h $$PWD/ReservationList.h 
HEADERS += $$PWD/SyncSession.h $$PWD/SyncUserInTraining.h $$PWD/UserList.h
HEADERS += $$PWD/CalendarList.h $$PWD/HeartBeat.h $$PWD/SessionOperation.h 
HEADERS += $$PWD/SyncSystemEvent.h $$PWD/TempCardBindingList.h $$PWD/UserRightList.h
HEADERS += $$PWD/SyncDeviceCaptureEvent.h $$PWD/EntrustList.h  $$PWD/SyncEntrustEvent.h

SOURCES += $$PWD/AccountList.cpp $$PWD/Command.cpp $$PWD/InstrumentDetail.cpp \
    $$PWD/upgradeparams.cpp \
    $$PWD/upgradeplan.cpp \
    $$PWD/tinsqualist.cpp
SOURCES += $$PWD/SyncFtpEvent.cpp $$PWD/SyncUserEvent.cpp $$PWD/Unbinding.cpp
SOURCES += $$PWD/BookingList.cpp $$PWD/GlobalConfig.cpp $$PWD/ReservationList.cpp 
SOURCES += $$PWD/SyncSession.cpp $$PWD/SyncUserInTraining.cpp $$PWD/UserList.cpp
SOURCES += $$PWD/CalendarList.cpp $$PWD/HeartBeat.cpp $$PWD/SessionOperation.cpp 
SOURCES += $$PWD/SyncSystemEvent.cpp $$PWD/TempCardBindingList.cpp $$PWD/UserRightList.cpp
SOURCES += $$PWD/SyncDeviceCaptureEvent.cpp $$PWD/EntrustList.cpp $$PWD/SyncEntrustEvent.cpp

