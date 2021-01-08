/****************************************************************************
** Meta object code from reading C++ file 'SslService.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller/Service/SslService.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SslService.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SslService[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      30,   11,   11,   11, 0x05,
      44,   11,   11,   11, 0x05,
      61,   11,   11,   11, 0x05,
      82,   11,   11,   11, 0x05,
     108,   11,   11,   11, 0x05,
     135,  130,   11,   11, 0x05,
     183,  177,   11,   11, 0x05,
     206,   11,   11,   11, 0x05,
     229,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     250,   11,   11,   11, 0x0a,
     264,  258,   11,   11, 0x0a,
     284,   11,   11,   11, 0x0a,
     324,  307,   11,   11, 0x0a,
     361,   11,   11,   11, 0x0a,
     378,   11,   11,   11, 0x0a,
     392,   11,   11,   11, 0x0a,
     414,  408,   11,   11, 0x0a,
     438,  434,   11,   11, 0x0a,
     482,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SslService[] = {
    "SslService\0\0settingsUpdated()\0"
    "userUpdated()\0bookingUpdated()\0"
    "reservationUpdated()\0networkStateChanged(bool)\0"
    "bindingReleased(bool)\0,,,,\0"
    "cardDetectedFromNet(int,int,uint,int,int)\0"
    "error\0error(SslServiceError)\0"
    "upgradeParamsUpdated()\0upgradePlanUpdated()\0"
    "start()\0state\0setClientState(int)\0"
    "sendUnbindingRequest()\0id,status,bookid\0"
    "sendDetectedFromNetResp(int,int,int)\0"
    "doDisconnected()\0doConnected()\0"
    "doRegularTask()\0frame\0parseFrame(FrameSP)\0"
    "err\0onSocketError(QAbstractSocket::SocketError)\0"
    "onConnectTimeout()\0"
};

void SslService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SslService *_t = static_cast<SslService *>(_o);
        switch (_id) {
        case 0: _t->settingsUpdated(); break;
        case 1: _t->userUpdated(); break;
        case 2: _t->bookingUpdated(); break;
        case 3: _t->reservationUpdated(); break;
        case 4: _t->networkStateChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->bindingReleased((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->cardDetectedFromNet((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 7: _t->error((*reinterpret_cast< SslServiceError(*)>(_a[1]))); break;
        case 8: _t->upgradeParamsUpdated(); break;
        case 9: _t->upgradePlanUpdated(); break;
        case 10: _t->start(); break;
        case 11: _t->setClientState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->sendUnbindingRequest(); break;
        case 13: _t->sendDetectedFromNetResp((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 14: _t->doDisconnected(); break;
        case 15: _t->doConnected(); break;
        case 16: _t->doRegularTask(); break;
        case 17: _t->parseFrame((*reinterpret_cast< FrameSP(*)>(_a[1]))); break;
        case 18: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 19: _t->onConnectTimeout(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SslService::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SslService::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SslService,
      qt_meta_data_SslService, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SslService::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SslService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SslService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SslService))
        return static_cast<void*>(const_cast< SslService*>(this));
    return QObject::qt_metacast(_clname);
}

int SslService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void SslService::settingsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void SslService::userUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void SslService::bookingUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void SslService::reservationUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void SslService::networkStateChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SslService::bindingReleased(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SslService::cardDetectedFromNet(int _t1, int _t2, uint _t3, int _t4, int _t5)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void SslService::error(SslServiceError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void SslService::upgradeParamsUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void SslService::upgradePlanUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}
QT_END_MOC_NAMESPACE
