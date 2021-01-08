/****************************************************************************
** Meta object code from reading C++ file 'NetchoicePage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../wizard/NetchoicePage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetchoicePage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NetchoicePage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   15,   14,   14, 0x08,
      41,   14,   14,   14, 0x08,
      60,   14,   14,   14, 0x08,
      72,   14,   14,   14, 0x08,
      95,   89,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_NetchoicePage[] = {
    "NetchoicePage\0\0checked\0modeToggled(bool)\0"
    "wlan_edit(QString)\0wlanFlash()\0"
    "wlanConnecting()\0state\0"
    "onLockerOverEthCheckBoxStateChanged(int)\0"
};

void NetchoicePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NetchoicePage *_t = static_cast<NetchoicePage *>(_o);
        switch (_id) {
        case 0: _t->modeToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->wlan_edit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->wlanFlash(); break;
        case 3: _t->wlanConnecting(); break;
        case 4: _t->onLockerOverEthCheckBoxStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData NetchoicePage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject NetchoicePage::staticMetaObject = {
    { &QWizardPage::staticMetaObject, qt_meta_stringdata_NetchoicePage,
      qt_meta_data_NetchoicePage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NetchoicePage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NetchoicePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NetchoicePage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NetchoicePage))
        return static_cast<void*>(const_cast< NetchoicePage*>(this));
    return QWizardPage::qt_metacast(_clname);
}

int NetchoicePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
