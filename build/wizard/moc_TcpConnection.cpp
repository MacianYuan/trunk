/****************************************************************************
** Meta object code from reading C++ file 'TcpConnection.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../wizard/TcpConnection.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TcpConnection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TcpConnection[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   15,   14,   14, 0x05,
      54,   49,   14,   14, 0x05,
      81,   75,   14,   14, 0x05,
     106,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
     123,   14,   14,   14, 0x08,
     136,   14,   14,   14, 0x08,
     150,   14,   14,   14, 0x08,
     175,  167,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_TcpConnection[] = {
    "TcpConnection\0\0error\0connectionError(SetupError)\0"
    "info\0showMessage(QString)\0state\0"
    "changeRegisterState(int)\0connectSslHost()\0"
    "readSocket()\0onConnected()\0onDisconnected()\0"
    "message\0parseMessage(QByteArray)\0"
};

void TcpConnection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TcpConnection *_t = static_cast<TcpConnection *>(_o);
        switch (_id) {
        case 0: _t->connectionError((*reinterpret_cast< const SetupError(*)>(_a[1]))); break;
        case 1: _t->showMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->changeRegisterState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->connectSslHost(); break;
        case 4: _t->readSocket(); break;
        case 5: _t->onConnected(); break;
        case 6: _t->onDisconnected(); break;
        case 7: _t->parseMessage((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TcpConnection::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TcpConnection::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_TcpConnection,
      qt_meta_data_TcpConnection, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TcpConnection::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TcpConnection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TcpConnection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TcpConnection))
        return static_cast<void*>(const_cast< TcpConnection*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int TcpConnection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void TcpConnection::connectionError(const SetupError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TcpConnection::showMessage(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TcpConnection::changeRegisterState(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TcpConnection::connectSslHost()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
