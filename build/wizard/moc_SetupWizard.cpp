/****************************************************************************
** Meta object code from reading C++ file 'SetupWizard.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../wizard/SetupWizard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SetupWizard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SetupWizard[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   13,   12,   12, 0x05,
      42,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   12,   12,   12, 0x0a,
      58,   12,   12,   12, 0x0a,
      79,   67,   12,   12, 0x0a,
     140,  128,   12,   12, 0x0a,
     189,   13,   12,   12, 0x0a,
     223,  218,   12,   12, 0x0a,
     246,   12,   12,   12, 0x0a,
     264,  262,   12,   12, 0x0a,
     300,  291,   12,   12, 0x0a,
     340,  336,   12,   12, 0x0a,
     367,   12,   12,   12, 0x0a,
     380,   12,   12,   12, 0x0a,
     407,  399,   12,   12, 0x0a,
     439,  436,   12,   12, 0x0a,
     463,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SetupWizard[] = {
    "SetupWizard\0\0setupError\0error(SetupError)\0"
    "exit()\0accept()\0reject()\0socketError\0"
    "displaySocketError(QAbstractSocket::SocketError)\0"
    "socketState\0"
    "displaySocketState(QAbstractSocket::SocketState)\0"
    "handleSetupError(SetupError)\0info\0"
    "onShowMessage(QString)\0onHideMessage()\0"
    "s\0onChangeRegisterState(int)\0label,ap\0"
    "onDownloadPageInfo(QString,QString)\0"
    "num\0onDownloadPageProcess(int)\0"
    "onNextPage()\0onConnectSslHost()\0name,id\0"
    "onAddInstrument(QString,int)\0id\0"
    "onCurrentIdChanged(int)\0onBeforeBack()\0"
};

void SetupWizard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SetupWizard *_t = static_cast<SetupWizard *>(_o);
        switch (_id) {
        case 0: _t->error((*reinterpret_cast< SetupError(*)>(_a[1]))); break;
        case 1: _t->exit(); break;
        case 2: _t->accept(); break;
        case 3: _t->reject(); break;
        case 4: _t->displaySocketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: _t->displaySocketState((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 6: _t->handleSetupError((*reinterpret_cast< SetupError(*)>(_a[1]))); break;
        case 7: _t->onShowMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onHideMessage(); break;
        case 9: _t->onChangeRegisterState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->onDownloadPageInfo((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 11: _t->onDownloadPageProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->onNextPage(); break;
        case 13: _t->onConnectSslHost(); break;
        case 14: _t->onAddInstrument((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->onCurrentIdChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->onBeforeBack(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SetupWizard::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SetupWizard::staticMetaObject = {
    { &QWizard::staticMetaObject, qt_meta_stringdata_SetupWizard,
      qt_meta_data_SetupWizard, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SetupWizard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SetupWizard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SetupWizard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SetupWizard))
        return static_cast<void*>(const_cast< SetupWizard*>(this));
    return QWizard::qt_metacast(_clname);
}

int SetupWizard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizard::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void SetupWizard::error(SetupError _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SetupWizard::exit()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
