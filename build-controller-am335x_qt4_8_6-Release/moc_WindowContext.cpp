/****************************************************************************
** Meta object code from reading C++ file 'WindowContext.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller/Context/WindowContext.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WindowContext.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WindowContext[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   14,   14,   14, 0x0a,
      62,   14,   57,   14, 0x0a,
      76,   14,   57,   14, 0x0a,
     107,   94,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_WindowContext[] = {
    "WindowContext\0\0sendRequestUnbinding()\0"
    "RequestUnbinding()\0bool\0BookingLock()\0"
    "ReservationLock()\0state,reason\0"
    "onStateChange(UPGRADE_STATE,int)\0"
};

void WindowContext::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WindowContext *_t = static_cast<WindowContext *>(_o);
        switch (_id) {
        case 0: _t->sendRequestUnbinding(); break;
        case 1: _t->RequestUnbinding(); break;
        case 2: { bool _r = _t->BookingLock();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->ReservationLock();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: _t->onStateChange((*reinterpret_cast< UPGRADE_STATE(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WindowContext::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WindowContext::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WindowContext,
      qt_meta_data_WindowContext, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WindowContext::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WindowContext::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WindowContext::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WindowContext))
        return static_cast<void*>(const_cast< WindowContext*>(this));
    return QObject::qt_metacast(_clname);
}

int WindowContext::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void WindowContext::sendRequestUnbinding()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
