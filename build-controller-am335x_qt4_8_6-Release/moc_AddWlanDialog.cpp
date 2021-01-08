/****************************************************************************
** Meta object code from reading C++ file 'AddWlanDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller/UI/AddWlanDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AddWlanDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AddWlanDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      40,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_AddWlanDialog[] = {
    "AddWlanDialog\0\0hideSsidInputError(bool)\0"
    "savewlanconfig()\0"
};

void AddWlanDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AddWlanDialog *_t = static_cast<AddWlanDialog *>(_o);
        switch (_id) {
        case 0: _t->hideSsidInputError((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->savewlanconfig(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData AddWlanDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject AddWlanDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_AddWlanDialog,
      qt_meta_data_AddWlanDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &AddWlanDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *AddWlanDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *AddWlanDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AddWlanDialog))
        return static_cast<void*>(const_cast< AddWlanDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int AddWlanDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void AddWlanDialog::hideSsidInputError(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
