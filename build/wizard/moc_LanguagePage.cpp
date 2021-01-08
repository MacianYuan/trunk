/****************************************************************************
** Meta object code from reading C++ file 'LanguagePage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../wizard/LanguagePage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LanguagePage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LanguagePage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      41,   31,   13,   13, 0x08,
      67,   61,   13,   13, 0x08,
      89,   61,   13,   13, 0x08,
     112,   61,   13,   13, 0x08,
     133,   61,   13,   13, 0x08,
     155,   61,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LanguagePage[] = {
    "LanguagePage\0\0onTimerTimeout()\0langIndex\0"
    "switchLanguage(int)\0index\0"
    "UpdateYearCombox(int)\0UpdateMonthCombox(int)\0"
    "UpdateDayCombox(int)\0UpdateHourCombox(int)\0"
    "UpdateMinuteCombox(int)\0"
};

void LanguagePage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LanguagePage *_t = static_cast<LanguagePage *>(_o);
        switch (_id) {
        case 0: _t->onTimerTimeout(); break;
        case 1: _t->switchLanguage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->UpdateYearCombox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->UpdateMonthCombox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->UpdateDayCombox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->UpdateHourCombox((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->UpdateMinuteCombox((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData LanguagePage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LanguagePage::staticMetaObject = {
    { &QWizardPage::staticMetaObject, qt_meta_stringdata_LanguagePage,
      qt_meta_data_LanguagePage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LanguagePage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LanguagePage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LanguagePage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LanguagePage))
        return static_cast<void*>(const_cast< LanguagePage*>(this));
    return QWizardPage::qt_metacast(_clname);
}

int LanguagePage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWizardPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
