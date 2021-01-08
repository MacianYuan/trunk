/****************************************************************************
** Meta object code from reading C++ file 'SslConnection.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sslconnection/SslConnection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SslConnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SslConnection[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,
      77,   69,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      97,   14,   14,   14, 0x08,
     110,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SslConnection[] = {
    "SslConnection\0\0error\0"
    "connectionError(SslConnection::ConnectionError)\0"
    "message\0newMessage(FrameSP)\0readSocket()\0"
    "onDisconnected()\0"
};

void SslConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SslConnection *_t = static_cast<SslConnection *>(_o);
        switch (_id) {
        case 0: _t->connectionError((*reinterpret_cast< const SslConnection::ConnectionError(*)>(_a[1]))); break;
        case 1: _t->newMessage((*reinterpret_cast< FrameSP(*)>(_a[1]))); break;
        case 2: _t->readSocket(); break;
        case 3: _t->onDisconnected(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SslConnection::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SslConnection::staticMetaObject = {
    { &QSslSocket::staticMetaObject, qt_meta_stringdata_SslConnection,
      qt_meta_data_SslConnection, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SslConnection::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SslConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SslConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SslConnection))
        return static_cast<void*>(const_cast< SslConnection*>(this));
    return QSslSocket::qt_metacast(_clname);
}

int SslConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSslSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SslConnection::connectionError(const SslConnection::ConnectionError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SslConnection::newMessage(FrameSP _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
