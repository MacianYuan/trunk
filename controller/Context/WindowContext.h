//
// Created by 何继胜 on 2017/4/28.
//

#ifndef OSLA_NEW_WINDOWCONTEXT_H
#define OSLA_NEW_WINDOWCONTEXT_H

#include <QtCore/QTime>

#include <protobuf/message.pb.h>
#include <controller/UI/TableViewDialog.h>
#include "SupportFeature.h"
#include "CardReader.h"
#include "SerialPort.h"
#include "controller/UI/AdminDialog.h"
#include "UI/upgradingdialog.h"
#include "../upgrade/QUpgrade.h"

class FtpParse;
class NetConnectionManage;
class UsbNetConnection;
class CASConnection;
class PowerDelay;
class InstrumentStatus;
class UserInformation;
class BookInformation;
class ReservationInformation;
class EntrustBeginDialog;
class SettingsDialog;
class QUpgrade;
class QUpgradePlan;

struct EXTRA_CONFIG
{
    //预约用户欠费允许开机开关：为1时，已预约用户即使欠费也能刷卡开机(默认0)
    bool enableDepositUser;
};

class WindowContext : public QObject {
    Q_OBJECT
public:
    WindowContext(QObject* parent);
    ~WindowContext();
    bool init();
    bool InitFtp();
    bool InitGlobal();
    void reloadFtpParams();

    bool LoginCheck();
    bool BookingCheck();
    //判断当前预约用户是不是到了可以刷卡时间
    bool isBookingUserInBookingTime();
    bool ExportData();
    void FactoryReset();
    void CheckExpiredBookings();
    const std::string& getDeviceVersion();
    bool UpdateNowUserBooking(int bookingId);

    //=======================属性======================
    FtpParse *getFtp() const;
    NetConnectionManage *getNetConnectionManage() const;
    UsbNetConnection *getUsbNetConnection() const;
    PowerDelay *getPowerDelay() const;
    bool isUseAlertTime() const;
    InstrumentStatus *getInstrumentStatus() const;
    UserInformation *getUserInformation() const;
    BookInformation *getBookInformation() const;
    long getTimeCheckNum() const;
    biotech::osla::Booking& getNowUserBooking();
    SupportFeature *getSupportFeature() const;
    void setUseAlertTime(bool useAlertTime);
    AdminDialog *getAdminDialog() const;
    SettingsDialog *getSettingsDialog() const;
    CardReader *getCardReader() const;
    int getCardGuard() const;
    void setCardGuard(int cardGuard);
    bool isCardEnabled() const;
    void setCardEnabled(bool cardEnabled);
    ReservationInformation *getReservationInformation() const;
    const biotech::osla::GlobalConfig &getGlobalConfig() const;
    long getServiceTime() const;
    long getCaptureTime() const;
    bool isLooseMode() const;
    int getUnsyncCount() const;
    QUpgrade *getUpgrade() const;       ///added by lwp,2018-1-15
    QUpgradePlan *getUpgradePlan() const;

    void showUpgradeDlg(void);
    void hidUpgradeDlg(void);
    void enableScreenSaver(void);
    void disableScreenSaver(void);
    bool isPaymentAaccountEnabled();
public slots:
    void RequestUnbinding();
    bool BookingLock();
    bool ReservationLock();
signals:
    void sendRequestUnbinding();

private slots:
    void onStateChange(enum UPGRADE_STATE state, int reason);
private:
    AdminDialog *adminDialog;            /// 管理员页面
    SettingsDialog *settingsDialog;      /// 设置页面
    UpgradingDialog *upgradeDialog;     /// 升级提示对话框

    CardReader *cardReader;              /// 刷卡器管理
    QTime cardTiming;                    /// 刷卡定时器
    int cardGuard{ 2000 };             /// 刷卡时间间隔
    bool cardEnabled {false};            /// 是否可以刷卡

    FtpParse* ftp;                                     /// ftp管理
    NetConnectionManage* netConnectionManage;          /// 网络管理

    QUpgrade *upgrade;                                 /// 升级管理
    QUpgradePlan *upgradePlan;                         /// 升级计划

    UsbNetConnection* usbNetConnection;                /// 使用扩展的网卡通信
    PowerDelay* powerDelay;                            /// 上电保护管理
    bool useAlertTime{false};                          /// 是否使用报警？？？？
    InstrumentStatus* instrumentStatus;                /// 设备状态管理
    UserInformation* userInformation;                  /// 用户信息管理
    BookInformation* bookInformation;                  /// 预约信息管理
    ReservationInformation* reservationInformation;    /// 专享信息管理
    long timeCheckNum{ 0 };

    biotech::osla::Booking nowUserBooking;             /// 用于处理提示预约使用过期的问题，当前用户使用的预约信息
    SupportFeature* supportFeature;                     /// 支持哪些功能，从功能码中读取

    biotech::osla::GlobalConfig globalConfig;           /// 全局配置信息
    struct EXTRA_CONFIG extraConfig;                    /// 额外的参数，从global config中的workTime字段中获取得到的
    long serviceTime{30};                                    /// ntp校时时间间隔
    long captureTime{300};                                    /// 采集器采集时间间隔
    bool looseMode{true};                                /// 宽松模式，没有预约可以上机，为true
};


#endif //OSLA_NEW_WINDOWCONTEXT_H
