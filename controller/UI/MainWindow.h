#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QtGui/QWidget>
#include <QtCore/QDateTime>
#include <QtCore/QPointer>
#include <QtCore/QProcess>
#include <QtNetwork/QUdpSocket>
#include <controller/Context/SupportFeature.h>
#include <controller/Context/WindowContext.h>
#include <controller/Context/SessionManage.h>
#include <controller/Context/AdminAction.h>

#include "Base/ftp.h"
#include "controller/Context/UsbNetConnection.h"
#include "controller/Context/NetConnectionManage.h"

#include "deviceinfo/typedef.h"
#include "protobuf/message.pb.h"
#include "Base/imageview.h"

class QTimer;
class QTcpSocket;

#include <QtNetwork/QFtp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *);
    WindowContext* getWindowContext();
    void changeTr(int language_label);
private:
    bool UserLogoutCardSwiping();
    bool UpdateTempHumidity(uint currentTime_t);

signals:
    void startSync();
    void stopSync();
    void stateChanged(int);
    void requestUnbinding();
    void quit();
    void sendDetectedFromNetResp(int, int, int);
    void qrImageUrl_update();
public slots:
    void ShowCurrentUserInfo();
    void SetCurrentStateInfo();
    void onCardDetected(uint cardSerial);
    void onCardDetectedFromNet(int id, int userid, uint cardId, int userrole, int op);
    void readSettings();
    void setNetworkState(bool online);
    void updateCurrentBooking();
    void setErrorInfo(QString text);
    void onBindingReleased(bool result);
    void FocusUnbinding();
    void onShowWorkStatus(int num);
    void onUpgradeParamsUpdated();
    void onUpgradePlanUpdated();
    void showWaringText(QString text);
private slots:
    void doRegularTask();
    void onSettingsDialogClosed();
    void onInstrumentStatusUpdated(int status);
    void OnSwapCardIntervalChanged(int interval);
    void onNetworkSettingsChanged();
    void doAdminAction(int dialogCode);
    void showBookingSchedule();
    void on_qrImageUrl_update();
private:
    Ui::MainWindow *ui;
    WindowContext* windowContext;
    SessionManage* sessionManage;
    AdminAction* adminAction;

    QWidget *qrWidget;
    ImageView *imageView;

    QFtp::State a;
    ///--------------------------------------------------
    QTime* cardTiming;
    QTimer* regularTaskTimer;

    //SqlWorker* sqlWorker;
    long timeCheckNum{ 0 };
    //biotech::osla::Booking m_NowUserBooking;             /// 用于处理提示预约使用过期的问题
};

#endif // MAINWINDOW_H
