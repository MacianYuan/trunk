#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H
#include <map>
#include <QtGui/QDialog>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>
#include <QProgressDialog>
#include "../upgrade/QUpgrade.h"
#include <QtCore/QTranslator>
#include <QtGui/QApplication>
#include "AddWlanDialog.h"
class NetConnectionManage;

namespace Ui {
class SettingsDialog;
}

class QButtonGroup;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    bool isSettingsChanged() const;
    void resetSettingsChanged(bool changed = false);
    void setInUse(bool isInUse);
    void SetNetConnectionManage(NetConnectionManage* nm);
    void setWindowContext(WindowContext *context);

protected:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);
signals:
    void FocusUnBinding();
    void requestUnbinding();
    void requestExportData();
    void instrumentStatusUpdated(int status);
    void networkSettingsChanged();
    void serverUpdated();
    void checkUpdate();
    void swapCardIntervalChanged(int interval);
    void reloadSetting();
    void userManualUpgrade();       //用户手动升级请求(点升级按钮)
private slots:
    /// tab页面切换
    void showInstrumentPage();
    void showNetworkPage();
    void showNetchoicePage();
    void showSystemPage();
    void showTimePage();
    void showAdvancePage();
    void showAbout();
    bool closeSetting();
    /// 仪器响应
    void onInstrumentStatusChanged(int status);
    void unbindInstrument();
    void exportData();
    void saveInstrumentStatus();
    ///网络类型响应
    void onNetChoiceChanged();
    void onNetModeToggled(bool checked);
    void onhideNetToggled(bool checked);
    void onWlanPasswdComplte(QString);
    void saveNetchoice();
    /// 网络详细地址响应
    void onDHCPToggled(bool checked);
    void onNetworkChanged();
    void saveNetwork();
    /// 系统详细信息响应
    void onVoiceToggled(bool checked);
    void onVolumeChanged(int value);
    void onSwapCardIntervalChanged(int value);
    void onSystemChanged();
    void saveSystem();
    /// 定时检测
    void checkUnbindInstrument();
    void scanWlanAP();
    void saveTime();

    /// 时间设置
    void onTimerTimeout();
    void UpdateYearCombox(int index);
    void UpdateMonthCombox(int index);
    void UpdateDayCombox(int index);
    void UpdateHourCombox(int index);
    void UpdateMinuteCombox(int index);

    //升级相关响应处理
    void onUpgradeButtonClick(void);
    void onDownloadProcess(int dltotal, int dlnow);
    void onStateChange(enum UPGRADE_STATE state, int reason);
    void onDownloadCancelButtonClick(void);
    //重启按钮处理
    void onRestartButtonClick(void);
    void onLanguageCommoxClick(void);
    void on_tsCalButton_clicked();

    void on_copyLogButton_clicked();

    void on_formatCardButton_clicked();
    void switchLanguage(int langIndex);

    void on_AddWlan_button_clicked();
    void on_hideSsidInputError(bool status);
private:
    void flashWlanPSKCombox(QString beforeSelect);
    void flashNetTypeButton();
    void flashNetConfigLineEdit();
    void flashNetDHCP();
    void saveNetDetailConfig();
    void saveNetTypeConfig();
    void saveHostConfig();
    void saveWlanConfig();
private:
    void SetShowTime(QDateTime info);
    void ConnectTimeCombox();
    void DisConnectTimeCombox();

    void setVolume();
    void setScreenSaverIntervals();

    void showDlProcessDlg(void);
    void hideDlProcessDlg(void);
private:
    Ui::SettingsDialog *ui;
    QButtonGroup *instrumentStatusButton;

    int currentStatus;     /// 当前仪器的状态
    bool settingsChanged;  /// 配置是否发生过变化
    bool inUse;            /// 用户是否在使用设备

    QTimer* timer;
    QDateTime now;

    //bool useDHCP;
    //QString ipAddr;
    //QString netmask;
    //QString gateway;

    QTimer* unBindTimer;
    //QTimer* scanWlanAPTimer;

    NetConnectionManage* netConnectionManage;
    QTranslator appTranslator;
    WindowContext *windowContext;
    QProgressDialog *dlProgressDlg;

    AddWlanDialog *wlandialog;
};

#endif // SETTINGSDIALOG_H
