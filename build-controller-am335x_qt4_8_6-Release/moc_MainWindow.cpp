/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../controller/UI/MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      29,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      24,   11,   11,   11, 0x05,
      35,   11,   11,   11, 0x05,
      53,   11,   11,   11, 0x05,
      72,   11,   11,   11, 0x05,
      82,   79,   11,   11, 0x05,
     119,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     139,   11,   11,   11, 0x0a,
     161,   11,   11,   11, 0x0a,
     194,  183,   11,   11, 0x0a,
     244,  215,   11,   11, 0x0a,
     288,   11,   11,   11, 0x0a,
     310,  303,   11,   11, 0x0a,
     332,   11,   11,   11, 0x0a,
     360,  355,   11,   11, 0x0a,
     389,  382,   11,   11, 0x0a,
     413,   11,   11,   11, 0x0a,
     434,  430,   11,   11, 0x0a,
     456,   11,   11,   11, 0x0a,
     481,   11,   11,   11, 0x0a,
     504,  355,   11,   11, 0x0a,
     528,   11,   11,   11, 0x08,
     544,   11,   11,   11, 0x08,
     576,  569,   11,   11, 0x08,
     616,  607,   11,   11, 0x08,
     647,   11,   11,   11, 0x08,
     685,  674,   11,   11, 0x08,
     704,   11,   11,   11, 0x08,
     726,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0startSync()\0stopSync()\0"
    "stateChanged(int)\0requestUnbinding()\0"
    "quit()\0,,\0sendDetectedFromNetResp(int,int,int)\0"
    "qrImageUrl_update()\0ShowCurrentUserInfo()\0"
    "SetCurrentStateInfo()\0cardSerial\0"
    "onCardDetected(uint)\0id,userid,cardId,userrole,op\0"
    "onCardDetectedFromNet(int,int,uint,int,int)\0"
    "readSettings()\0online\0setNetworkState(bool)\0"
    "updateCurrentBooking()\0text\0"
    "setErrorInfo(QString)\0result\0"
    "onBindingReleased(bool)\0FocusUnbinding()\0"
    "num\0onShowWorkStatus(int)\0"
    "onUpgradeParamsUpdated()\0"
    "onUpgradePlanUpdated()\0showWaringText(QString)\0"
    "doRegularTask()\0onSettingsDialogClosed()\0"
    "status\0onInstrumentStatusUpdated(int)\0"
    "interval\0OnSwapCardIntervalChanged(int)\0"
    "onNetworkSettingsChanged()\0dialogCode\0"
    "doAdminAction(int)\0showBookingSchedule()\0"
    "on_qrImageUrl_update()\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->startSync(); break;
        case 1: _t->stopSync(); break;
        case 2: _t->stateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->requestUnbinding(); break;
        case 4: _t->quit(); break;
        case 5: _t->sendDetectedFromNetResp((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: _t->qrImageUrl_update(); break;
        case 7: _t->ShowCurrentUserInfo(); break;
        case 8: _t->SetCurrentStateInfo(); break;
        case 9: _t->onCardDetected((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 10: _t->onCardDetectedFromNet((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< uint(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 11: _t->readSettings(); break;
        case 12: _t->setNetworkState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->updateCurrentBooking(); break;
        case 14: _t->setErrorInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->onBindingReleased((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->FocusUnbinding(); break;
        case 17: _t->onShowWorkStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->onUpgradeParamsUpdated(); break;
        case 19: _t->onUpgradePlanUpdated(); break;
        case 20: _t->showWaringText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->doRegularTask(); break;
        case 22: _t->onSettingsDialogClosed(); break;
        case 23: _t->onInstrumentStatusUpdated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->OnSwapCardIntervalChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->onNetworkSettingsChanged(); break;
        case 26: _t->doAdminAction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->showBookingSchedule(); break;
        case 28: _t->on_qrImageUrl_update(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 29)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 29;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::startSync()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void MainWindow::stopSync()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void MainWindow::stateChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::requestUnbinding()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void MainWindow::quit()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void MainWindow::sendDetectedFromNetResp(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::qrImageUrl_update()
{
    QMetaObject::activate(this, &staticMetaObject, 6, 0);
}
QT_END_MOC_NAMESPACE
