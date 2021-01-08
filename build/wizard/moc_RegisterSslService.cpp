/****************************************************************************
** Meta object code from reading C++ file 'RegisterSslService.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../wizard/RegisterSslService.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RegisterSslService.h' doesn't include <QObject>."
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
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   12,   11,   11, 0x05,
      44,   39,   11,   11, 0x05,
      65,   11,   11,   11, 0x05,
      85,   79,   11,   11, 0x05,
     122,  113,   11,   11, 0x05,
     160,  156,   11,   11, 0x05,
     185,   11,   11,   11, 0x05,
     204,  196,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     251,  231,   11,   11, 0x0a,
     263,   11,   11,   11, 0x2a,
     271,   11,   11,   11, 0x0a,
     278,   11,   11,   11, 0x0a,
     295,   11,   11,   11, 0x0a,
     315,  309,   11,   11, 0x0a,
     339,  335,   11,   11, 0x0a,
     383,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SslService[] = {
    "SslService\0\0s\0changeRegisterState(int)\0"
    "info\0showMessage(QString)\0hideMessage()\0"
    "error\0connectionError(SetupError)\0"
    "label,ap\0downloadPageInfo(QString,QString)\0"
    "num\0downloadPageProcess(int)\0nextPage()\0"
    "name,id\0addInstrument(QString,int)\0"
    "isGetInstrumentList\0start(bool)\0start()\0"
    "stop()\0doDisconnected()\0doConnected()\0"
    "frame\0parseFrame(FrameSP)\0err\0"
    "onSocketError(QAbstractSocket::SocketError)\0"
    "onSslConnectTimerTimeout()\0"
};

void SslService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SslService *_t = static_cast<SslService *>(_o);
        switch (_id) {
        case 0: _t->changeRegisterState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->showMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->hideMessage(); break;
        case 3: _t->connectionError((*reinterpret_cast< const SetupError(*)>(_a[1]))); break;
        case 4: _t->downloadPageInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->downloadPageProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->nextPage(); break;
        case 7: _t->addInstrument((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->start((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->start(); break;
        case 10: _t->stop(); break;
        case 11: _t->doDisconnected(); break;
        case 12: _t->doConnected(); break;
        case 13: _t->parseFrame((*reinterpret_cast< FrameSP(*)>(_a[1]))); break;
        case 14: _t->onSocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 15: _t->onSslConnectTimerTimeout(); break;
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
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void SslService::changeRegisterState(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SslService::showMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SslService::hideMessage()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void SslService::connectionError(const SetupError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SslService::downloadPageInfo(const QString & _t1, const QString & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SslService::downloadPageProcess(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SslService::nextPage()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}

// SIGNAL 7
void SslService::addInstrument(const QString & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
