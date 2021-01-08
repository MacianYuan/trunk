/****************************************************************************
** Meta object code from reading C++ file 'EntrustBeginDialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller/UI/EntrustBeginDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EntrustBeginDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EntrustBeginDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x08,
      34,   19,   19,   19, 0x08,
      56,   19,   19,   19, 0x08,
      78,   19,   19,   19, 0x08,
      93,   19,   19,   19, 0x08,
     118,   19,   19,   19, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_EntrustBeginDialog[] = {
    "EntrustBeginDialog\0\0showEntrust()\0"
    "onPrevButtonClicked()\0onNextButtonClicked()\0"
    "startSession()\0chooseClick(QModelIndex)\0"
    "selectDontOverClicked()\0"
};

void EntrustBeginDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EntrustBeginDialog *_t = static_cast<EntrustBeginDialog *>(_o);
        switch (_id) {
        case 0: _t->showEntrust(); break;
        case 1: _t->onPrevButtonClicked(); break;
        case 2: _t->onNextButtonClicked(); break;
        case 3: _t->startSession(); break;
        case 4: _t->chooseClick((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 5: _t->selectDontOverClicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EntrustBeginDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EntrustBeginDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_EntrustBeginDialog,
      qt_meta_data_EntrustBeginDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EntrustBeginDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EntrustBeginDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EntrustBeginDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EntrustBeginDialog))
        return static_cast<void*>(const_cast< EntrustBeginDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int EntrustBeginDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
