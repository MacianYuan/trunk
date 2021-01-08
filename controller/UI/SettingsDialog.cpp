#include <QtGui/QApplication>
#include <deviceinfo/enum.h>
#include <deviceinfo/tq2440.h>
#include <deviceinfo/version.h>

#include <QtGui/QMessageBox>
#include <QtCore/QProcess>
#include <QtGui/QWSServer>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QDebug>

#include <tools/IniParse.h>
#include <tools/log.h>
#include <imframe/imframe.h>

#include "controller/UI/SettingsDialog.h"
#include "controller/Base/NormalTools.h"
#include "controller/Context/NetConnectionManage.h"
#include "controller/Context/ServiceEvent.h"
#include "controller/Context/WindowContext.h"
#include "controller/Context/InstrumentStatus.h"
#include "ui_SettingsDialog.h"
#include "tools/cscanapresults.h"
#include "tools/cwlanconfigfile.h"
#include "controller/Base/GlobalData.h"
#include <QDebug>
#include "common.h"
QString text;
int cbo_isComplete = 0;

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog) {
    ui->setupUi(this);

    //don't show
    ui->noBookingCanLoginCheckBox->setVisible(false);

    IniParse osla(OSLA_CONFIG);

    if(osla.GetBool("Wlan.hideNet",true) == true){
        ui->AddWlan_button->setEnabled(true);
    }else if (osla.GetBool("Wlan.hideNet",true) == false){
        ui->AddWlan_button->setEnabled(false);
    }

    ui->languageCombox->addItem("English","en_US");
    ui->languageCombox->addItem("简体中文","zh_CN");//简体中文


    GlobalData::currentLocale = osla.GetString("Global.Language").c_str();
    //qDebug()<< GlobalData::currentLocale;
    if (GlobalData::currentLocale.isEmpty()) {
        GlobalData::currentLocale = "en_US";
        ui->languageCombox->setCurrentIndex(0);
    }
    if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0){
        ui->languageCombox->setCurrentIndex(1);
    }

//    if(appTranslator.load("controller_" + GlobalData::currentLocale, ":/lang")){
//          qDebug()<< "load success";
//    }
//    else{
//         qDebug()<< "load fail";
//    }

//    qApp->installTranslator(&appTranslator);

    connect(ui->languageCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(switchLanguage(int))); // 关联到切换语言的槽

    //ui->advLabel->setVisible(false);
    //ui->line_3->setVisible(false);
    ui->casCheckBox->setVisible(false);
    ui->casAddrLabel->setVisible(false);
    ui->casAddrSpinBox->setVisible(false);

    setWindowModality(Qt::WindowModal);

    cbo_isComplete = 0;

    instrumentStatusButton = new QButtonGroup(this);
    instrumentStatusButton->addButton(ui->normalButton, biotech::osla::INSTRUMENT_STATUS_ACTIVE);
    instrumentStatusButton->addButton(ui->faultyButton, biotech::osla::INSTRUMENT_STATUS_FAULTY);
    instrumentStatusButton->addButton(ui->inactiveButton, biotech::osla::INSTRUMENT_STATUS_INACTIVE);

    QRegExp ipRegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator *ipRegExpValidator = new QRegExpValidator(ipRegExp, this);
    ui->ipEdit->setValidator(ipRegExpValidator);
    ui->netmaskEdit->setValidator(ipRegExpValidator);
    ui->gatewayEdit->setValidator(ipRegExpValidator);
    ui->serverHostEdit->setValidator(ipRegExpValidator);
    ui->ntpEdit->setValidator(ipRegExpValidator);

    QIntValidator *portIntValidator = new QIntValidator(1, 65535, this);
    ui->serverPortEdit->setValidator(portIntValidator);
    //15秒
    ui->autoCloseComboBox->addItem(tr("15Sec"), 15);
    ui->autoCloseComboBox->addItem(tr("30Sec"), 30);
    ui->autoCloseComboBox->addItem(tr("60Sec"), 60);

    ui->screenDimComboBox->addItem(tr("Never"), 0);
    ui->screenDimComboBox->addItem(tr("1Min"), 60);
    ui->screenDimComboBox->addItem(tr("2Min"), 120);
    ui->screenDimComboBox->addItem(tr("5min"), 300);

    ui->screenOffComboBox->addItem(tr("Never"), 0);
    ui->screenOffComboBox->addItem(tr("5Min"), 300);
    ui->screenOffComboBox->addItem(tr("10Min"), 600);
    ui->screenOffComboBox->addItem(tr("30Min"), 1800);


    char *able = getenv("FTP_UPLOAD_ABLE");
    if (false == osla.GetBool("Ftp.enable")) {
        DEBUG_LOG("don't find sd card, can't export data");
        able = NULL;
    }

    ui->exportButton->setEnabled(false);
    if (!(NULL == able || 0 == atoi(able))) {
        ui->exportButton->setEnabled(true);
    }
    ui->saveStatusButton->setEnabled(false);
    ui->saveNetchoiceButton->setEnabled(false);
    ui->saveNetworkButton->setEnabled(false);
    ui->saveSystemButton->setEnabled(false);
    ui->AddWlan_button->setEnabled(false);

    setVolume();
    setScreenSaverIntervals();

    settingsChanged = false;
    inUse = false;

    ///tab页面切换
    connect(ui->instrumentButton, SIGNAL(clicked()), this, SLOT(showInstrumentPage()));
    connect(ui->netchoiceButton, SIGNAL(clicked()), this, SLOT(showNetchoicePage()));
    connect(ui->networkButton, SIGNAL(clicked()), this, SLOT(showNetworkPage()));
    connect(ui->systemButton, SIGNAL(clicked()), this, SLOT(showSystemPage()));
    connect(ui->timeButton, SIGNAL(clicked()), this, SLOT(showTimePage()));
    connect(ui->advanceButton, SIGNAL(clicked()), this, SLOT(showAdvancePage()));
    connect(ui->aboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(closeSetting()));

    /// 仪器响应
    connect(instrumentStatusButton, SIGNAL(buttonClicked(int)), this, SLOT(onInstrumentStatusChanged(int)));
    connect(ui->unbindButton, SIGNAL(clicked()), this, SLOT(unbindInstrument()));
    connect(ui->saveStatusButton, SIGNAL(clicked()), this, SLOT(saveInstrumentStatus()));
    connect(ui->exportButton, SIGNAL(clicked()), this, SLOT(exportData()));

    ///网络类型响应
    connect(ui->wlanPskcomboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(onNetChoiceChanged()));
    connect(ui->wlanPskEdit, SIGNAL(textChanged(QString)), this, SLOT(onNetChoiceChanged()));
    connect(ui->lanButton, SIGNAL(toggled(bool)), this, SLOT(onNetChoiceChanged()));
    connect(ui->wlanButton, SIGNAL(toggled(bool)), this, SLOT(onNetModeToggled(bool)));

    connect(ui->HideWireless_radioButton, SIGNAL(toggled(bool)), this, SLOT(onhideNetToggled(bool)));

    connect(ui->saveNetchoiceButton, SIGNAL(clicked()), this, SLOT(saveNetchoice()));

    /// 网络详细地址响应
    connect(ui->autoButton, SIGNAL(toggled(bool)), this, SLOT(onDHCPToggled(bool)));
    connect(ui->autoButton, SIGNAL(toggled(bool)), this, SLOT(onNetworkChanged()));
    connect(ui->ipEdit, SIGNAL(textChanged(QString)), this, SLOT(onNetworkChanged()));
    connect(ui->netmaskEdit, SIGNAL(textChanged(QString)), this, SLOT(onNetworkChanged()));
    connect(ui->gatewayEdit, SIGNAL(textChanged(QString)), this, SLOT(onNetworkChanged()));
    connect(ui->serverHostEdit, SIGNAL(textChanged(QString)), this, SLOT(onNetworkChanged()));
    connect(ui->serverPortEdit, SIGNAL(textChanged(QString)), this, SLOT(onNetworkChanged()));
    connect(ui->ntpEdit, SIGNAL(textChanged(QString)), this, SLOT(onNetworkChanged()));
    connect(ui->saveNetworkButton, SIGNAL(clicked()), this, SLOT(saveNetwork()));

    /// 系统详细信息响应
    connect(ui->noBookingCanLoginCheckBox, SIGNAL(toggled(bool)), this, SLOT(onSystemChanged()));
    connect(ui->voiceCheckBox, SIGNAL(toggled(bool)), this, SLOT(onVoiceToggled(bool)));
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(onVolumeChanged(int)));
    connect(ui->swapCardIntervalSlider, SIGNAL(valueChanged(int)), this, SLOT(onSwapCardIntervalChanged(int)));
    connect(ui->autoCloseComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSystemChanged()));
    connect(ui->screenDimComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSystemChanged()));
    connect(ui->screenOffComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSystemChanged()));
    connect(ui->casCheckBox, SIGNAL(toggled(bool)), this, SLOT(onSystemChanged()));
    connect(ui->casAddrSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onSystemChanged()));
    connect(ui->saveSystemButton, SIGNAL(clicked()), this, SLOT(saveSystem()));

    connect(ui->saveTimeButton, SIGNAL(clicked()), this, SLOT(saveTime()));

    //升级相关界面
    windowContext = NULL;
    connect(ui->upgradeButton, SIGNAL(clicked()), this, SLOT(onUpgradeButtonClick()));
    dlProgressDlg = NULL;

    //重启按钮
    connect(ui->restartButton, SIGNAL(clicked()), this, SLOT(onRestartButtonClick()));

    /// 定时检测设备是否需要重新绑定
    unBindTimer = new QTimer();
    connect(unBindTimer, SIGNAL(timeout()), this, SLOT(checkUnbindInstrument()));
    unBindTimer->start(2000);

    /// 在设置的时候，需要检测新的wifi热点
    //scanWlanAPTimer = new QTimer();
    //connect(scanWlanAPTimer, SIGNAL(timeout()), this, SLOT(scanWlanAP()));

    timer = new QTimer(this);
    now = QDateTime::currentDateTime();
    SetShowTime(now);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
    timer->start(1000);
}

SettingsDialog::~SettingsDialog() {
    if (dlProgressDlg)
    {
        delete dlProgressDlg;
    }
    if(wlandialog->isHidden() == false)
        wlandialog->close();
    unBindTimer->stop();
    delete unBindTimer;
    //scanWlanAPTimer->stop();
    //delete scanWlanAPTimer;
    delete ui;

}

// 动态切换语言
void SettingsDialog::switchLanguage(int langIndex)
{
   // GlobalData::currentLocale = ui->languageCombox->itemData(langIndex).toString();
   // qDebug()<< "switchlanguage" << GlobalData::currentLocale;

    //设置成功，点击确定系统重启生效
    if (QMessageBox::question(this, tr("Notice"), tr("Set successfully, click OK system restart to take effect?"),
                              QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Ok)
    {
        GlobalData::currentLocale = ui->languageCombox->itemData(langIndex).toString();
        //qDebug()<< "switchLanguage" << GlobalData::currentLocale;

        IniParse osla(OSLA_CONFIG);
        osla.Set("Global.Language", GlobalData::currentLocale.toStdString());
        osla.SaveFile();

        SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
        sqlWorker->updateEnumTextAndLanguage();
       // qDebug()<< "switchlanguage  ok";

        system("/bin/restart.sh &");
        qApp->quit();
    }
    else{

        qDebug()<< "switchlanguage fail";
    }

}


bool SettingsDialog::isSettingsChanged() const {
    return settingsChanged;
}

void SettingsDialog::resetSettingsChanged(bool changed) {
    settingsChanged = changed;
}

void SettingsDialog::setInUse(bool isInUse) {
    inUse = isInUse;
    ui->unbindButton->setEnabled(!inUse);
}

void SettingsDialog::SetNetConnectionManage(NetConnectionManage *nm) {
    netConnectionManage = nm;
}

void SettingsDialog::setWindowContext(WindowContext *context)
{
    windowContext = context;
}

void SettingsDialog::showEvent(QShowEvent *) {
    /// 先刷一遍数据
    showInstrumentPage();
    showNetchoicePage();
    showNetworkPage();
    showSystemPage();
    showTimePage();
    showInstrumentPage();
    netConnectionManage->WifiScan(true);
    //scanWlanAPTimer->start(1000);
}

void SettingsDialog::closeEvent(QCloseEvent *ev) {
    //scanWlanAPTimer->stop();
    netConnectionManage->WifiScan(false);
    QDialog::closeEvent(ev);
}

void SettingsDialog::showInstrumentPage() {
    InputType::setFlag(InputTypeDefine::ALL_INPUT_V);
    //temp::flag = 1;
    ui->saveStatusButton->setEnabled(false);
    IniParse osla(OSLA_CONFIG);
    currentStatus = osla.GetLong("Instrument.Status");
    instrumentStatusButton->button(currentStatus)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);
}

void SettingsDialog::showNetchoicePage() {
    InputType::setFlag(InputTypeDefine::ALL_INPUT_V);

    flashNetTypeButton();
    flashWlanPSKCombox("");
    ui->wlanPskEdit->setText("");
    ui->saveNetchoiceButton->setEnabled(false);

    ui->stackedWidget->setCurrentIndex(1);
}

void SettingsDialog::showNetworkPage() {
    InputType::setFlag(InputTypeDefine::NUM_INPUT_FOR_USE_HOST);
    if (QWidget *focusWidget = QWidget::focusWidget()) {
        focusWidget->clearFocus();
    }

    flashNetDHCP();
    flashNetConfigLineEdit();
    IniParse osla(OSLA_CONFIG);
    ui->serverHostEdit->setText(osla.GetString("Network.SslHost").c_str());
    ui->serverPortEdit->setText(osla.GetString("Network.SslPort").c_str());
    ui->ntpEdit->setText(osla.GetString("Network.NtpHost").c_str());

    ui->saveNetworkButton->setEnabled(false);
    ui->stackedWidget->setCurrentIndex(2);
}

void SettingsDialog::showSystemPage() {
    IniParse client(CLIENT_CONFIG);
    ui->voiceCheckBox->setChecked(client.GetBool("Voice.Enabled"));
    ui->volumeSlider->setValue(client.GetLong("Voice.Volume"));
    ui->volumeValueLabel->setText(QString::number(ui->volumeSlider->value()));

    ui->swapCardIntervalSlider->setValue(client.GetLong("Reader.Guarding", 5 * 1000) / 1000);
    ui->swapCardIntervalValueLabel->setText(QString::number(ui->swapCardIntervalSlider->value()));

    uint closeComboBox = client.GetLong("Security.AutoCloseDialog");
    if ( (closeComboBox == 15)||(closeComboBox == 30)||(closeComboBox == 60)){
        ui->autoCloseComboBox->setCurrentIndex(ui->autoCloseComboBox->findData((uint)closeComboBox ));
    }else{
        ui->autoCloseComboBox->setCurrentIndex(ui->autoCloseComboBox->findData((uint)15));
        client.Set("Security.AutoCloseDialog",
                   ui->autoCloseComboBox->itemData(ui->autoCloseComboBox->currentIndex()).toInt());
    }

    uint dimComboBox = client.GetLong("Energy.ScreenDim");
    if ( (dimComboBox == 0)||(dimComboBox == 60)||(dimComboBox == 120) || (dimComboBox == 300)){
        ui->screenDimComboBox->setCurrentIndex(ui->screenDimComboBox->findData((uint)dimComboBox ));
    }else{
        ui->screenDimComboBox->setCurrentIndex(ui->screenDimComboBox->findData((uint)60));
        client.Set("Energy.ScreenDim", ui->screenDimComboBox->itemData(ui->screenDimComboBox->currentIndex()).toInt());
    }

    uint offComboBox = client.GetLong("Energy.ScreenOff");
    if( (offComboBox == 0)||(offComboBox == 300)||(offComboBox == 600) || (offComboBox == 1800)){
        ui->screenOffComboBox->setCurrentIndex(ui->screenOffComboBox->findData((uint)offComboBox));
    }else{
        ui->screenOffComboBox->setCurrentIndex(ui->screenOffComboBox->findData((uint)300));
        client.Set("Energy.ScreenOff", ui->screenOffComboBox->itemData(ui->screenOffComboBox->currentIndex()).toInt());
    }
    client.SaveFile();

    IniParse osla(OSLA_CONFIG);
    ui->noBookingCanLoginCheckBox->setChecked(osla.GetBool("Instrument.LooseMode", true));

    ui->casCheckBox->setChecked(client.GetBool("CAS.Enabled"));
    ui->casAddrSpinBox->setValue(client.GetLong("CAS.ClientAddr"));

    ui->saveSystemButton->setEnabled(false);
    ui->stackedWidget->setCurrentIndex(3);
}

void SettingsDialog::showTimePage() {
    now = QDateTime::currentDateTime();
    SetShowTime(now);
    ui->saveTimeButton->setEnabled(!inUse);
    ui->stackedWidget->setCurrentIndex(4);
}

void SettingsDialog::showAdvancePage()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void SettingsDialog::showAbout() {
    emit checkUpdate();
    std::ifstream inf;
    inf.open("/etc/Version", std::ios::in);
    char line[1024] = {0};
    inf.getline(line, sizeof(line));
    inf.close();
     //tr("实验仪器开放共享管理系统"),tr("仪器控制终端 %1\n基于 Qt %2\n软件版本 %3\n安徽朋德信息科技有限公司 版权所有")
    QMessageBox::about(this, tr("Management and sharing system for laboratory device"),
                       tr("Instrument control terminal %1\nBase on Qt %2\nsoftware version %3\n Anhui Pengde information Technology Co.LTD all right reserved")
                               .arg(qApp->applicationVersion()).arg(QT_VERSION_STR).arg(line));
}

bool SettingsDialog::closeSetting() {
    return this->close();
}

void SettingsDialog::onInstrumentStatusChanged(int status) {
    ui->saveStatusButton->setEnabled(status != currentStatus);
}

void SettingsDialog::unbindInstrument() {
    //tr("注意"), tr("确定要解除终端与仪器的关联并恢复出厂设置吗？"),
    QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                              tr("Are you sure you want to disassociate the terminal from the instrument and restore the exit Settings"),
                                              QMessageBox::Yes | QMessageBox::No, this);
    messageBox->setDefaultButton(QMessageBox::No);
    messageBox->setAttribute(Qt::WA_DeleteOnClose);
    if (messageBox->exec() == QMessageBox::Yes) {
        emit requestUnbinding();
        settingsChanged = true;
    }
}

void SettingsDialog::exportData() {
    emit requestExportData();
}

void SettingsDialog::saveInstrumentStatus() {
    int selectedStatus = instrumentStatusButton->checkedId();
    if (currentStatus == biotech::osla::INSTRUMENT_STATUS_ACTIVE && inUse) {
        //tr("注意"),tr("改变仪器状态将结束当前用户的使用, 是否继续？"),
        QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                                  tr("Changing the state of the instrument will end the current user use,whether or not to continue?"),
                                                  QMessageBox::Yes | QMessageBox::No, this);
        messageBox->setDefaultButton(QMessageBox::No);
        messageBox->setAttribute(Qt::WA_DeleteOnClose);
        if (messageBox->exec() != QMessageBox::Yes) {
            instrumentStatusButton->button(currentStatus)->setChecked(true);
            ui->saveStatusButton->setEnabled(false);
            return;
        }
    }
    currentStatus = selectedStatus;
    IniParse osla(OSLA_CONFIG);
    osla.Set("Instrument.Status", currentStatus);
    osla.SaveFile();

    emit instrumentStatusUpdated(currentStatus);
    ui->saveStatusButton->setEnabled(false);

    settingsChanged = true;
    QProcess::startDetached("sync");
}

void SettingsDialog::onNetChoiceChanged() {
    ui->AddWlan_button->setEnabled(false);
    ui->saveNetchoiceButton->setEnabled(true);
}

void SettingsDialog::onNetModeToggled(bool checked) {
    ui->wlanPskcomboBox->setEnabled(checked);
    ui->wlanPskEdit->setEnabled(checked);
}

void SettingsDialog::onhideNetToggled(bool checked) {
    ui->AddWlan_button->setEnabled(checked);
    ui->saveNetchoiceButton->setEnabled(true);

    IniParse osla(OSLA_CONFIG);
    osla.Set("Wlan.hideNet",checked);
    osla.SaveFile();
}

void SettingsDialog::onWlanPasswdComplte(QString s) {
    ui->saveNetchoiceButton->setEnabled(true);
    text = s;
    if (ui->wlanPskcomboBox->currentIndex() > 0) { //判断点击的是否下拉条2
        cbo_isComplete = 1;
    }
}

void SettingsDialog::saveNetchoice() {
    if (false == ui->HideWireless_radioButton->isChecked()){
        if (true == ui->wlanButton->isChecked() &&
            (0 == ui->wlanPskcomboBox->currentIndex() ||
             (8 > ui->wlanPskEdit->text().length() && ui->wlanPskEdit->text().length() > 0))) {
            //tr("注意"), tr("使用无线，输入信息有误，请查看热点和密码"));
            QMessageBox::about(this, tr("Notice"), tr("Please check the AP ssid and password"));
            return;
        }

        if (true == ui->wlanButton->isChecked() && 0 == ui->wlanPskEdit->text().length()) {
            //tr("请确认是否使用该wifi热点(Wifi无密码)？"),
            QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                                      tr("Please confirm whether to use the wifi AP(Wifi no password)?"),
                                                      QMessageBox::Yes | QMessageBox::No, this);
            messageBox->setDefaultButton(QMessageBox::No);
            messageBox->setAttribute(Qt::WA_DeleteOnClose);
            if (messageBox->exec() != QMessageBox::Yes) {
                return;
            }
        }
    }else if (true == ui->HideWireless_radioButton->isChecked()){

    }

    saveNetTypeConfig();
    if ((true == ui->wlanButton->isChecked()) && (false == ui->HideWireless_radioButton->isChecked())) {
        saveWlanConfig();
    }

    ui->saveNetchoiceButton->setEnabled(false);
    QProcess::execute("net_clear");
    QProcess::startDetached("sync");
}

void SettingsDialog::onDHCPToggled(bool checked) {
    ui->ipEdit->setEnabled(!checked);
    ui->netmaskEdit->setEnabled(!checked);
    ui->gatewayEdit->setEnabled(!checked);
}

void SettingsDialog::onNetworkChanged() {
    bool isCompleted = (ui->autoButton->isChecked()
                        || (ui->ipEdit->hasAcceptableInput()
                            && ui->netmaskEdit->hasAcceptableInput()
                            && ui->gatewayEdit->hasAcceptableInput()))
                       && ui->serverHostEdit->hasAcceptableInput()
                       && ui->serverPortEdit->hasAcceptableInput()
                       && ui->ntpEdit->hasAcceptableInput();
    ui->saveNetworkButton->setEnabled(isCompleted);
}

void SettingsDialog::saveNetwork() {
 //   bool networkChanged = false;
    //bool serverChanged = false;
    //bool ntpChanged = false;

    saveNetTypeConfig();

    //if (!dhcp && (ipAddr != ui->ipEdit->text() || netmask != ui->netmaskEdit->text() || gateway != ui->gatewayEdit->text()))
    //if (!useDHCP && (true == ui->ipEdit->isModified() || true == ui->netmaskEdit->isModified() || true == ui->gatewayEdit->isModified()))
    if (true == ui->manualButton->isChecked() &&
        (true == ui->ipEdit->isModified() ||
         true == ui->netmaskEdit->isModified() ||
         true == ui->gatewayEdit->isModified())) {
        saveNetDetailConfig();
    }

    QProcess::execute("net_clear");
    saveHostConfig();
    emit networkSettingsChanged();
    ui->saveNetworkButton->setEnabled(false);
    QProcess::startDetached("sync");
}

void SettingsDialog::onVoiceToggled(bool checked) {
    ui->volumeLabel->setEnabled(checked);
    ui->volumeSlider->setEnabled(checked);
    ui->volumeValueLabel->setEnabled(checked);
    ui->saveSystemButton->setEnabled(true);
}

void SettingsDialog::onVolumeChanged(int value) {
    ui->volumeValueLabel->setText(QString::number(value));
    ui->saveSystemButton->setEnabled(true);
}

void SettingsDialog::onSwapCardIntervalChanged(int value) {
    ui->swapCardIntervalValueLabel->setText(QString::number(value));
    ui->saveSystemButton->setEnabled(true);
}

void SettingsDialog::onSystemChanged() {
    ui->saveSystemButton->setEnabled(true);
}

void SettingsDialog::saveSystem() {
    DEBUG_LOG("");
    IniParse client(CLIENT_CONFIG);
    client.Set("Voice.Enabled", ui->voiceCheckBox->isChecked());
    client.Set("Voice.Volume", ui->volumeSlider->value());
    client.Set("Security.AutoCloseDialog",
               ui->autoCloseComboBox->itemData(ui->autoCloseComboBox->currentIndex()).toInt());
    client.Set("Energy.ScreenDim", ui->screenDimComboBox->itemData(ui->screenDimComboBox->currentIndex()).toInt());
    client.Set("Energy.ScreenOff", ui->screenOffComboBox->itemData(ui->screenOffComboBox->currentIndex()).toInt());
    client.Set("Reader.Guarding", ui->swapCardIntervalValueLabel->text().toInt() * 1000);
    client.Set("CAS.Enabled", ui->casCheckBox->isChecked());
    client.Set("CAS.ClientAddr", ui->casAddrSpinBox->value());
    client.SaveFile();

    setVolume();
    setScreenSaverIntervals();

    IniParse osla(OSLA_CONFIG);
    bool beforeLooseMode = osla.GetBool("Instrument.LooseMode", true);
    if (ui->noBookingCanLoginCheckBox->isChecked() != beforeLooseMode) {
        osla.Set("Instrument.LooseMode", ui->noBookingCanLoginCheckBox->isChecked());
        ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_LOOSE_MODE, (ui->noBookingCanLoginCheckBox->isChecked() ? 1 : 0));
        osla.SaveFile();
    }

    settingsChanged = true;
    ui->saveSystemButton->setEnabled(false);
    QProcess::startDetached("sync");
    emit swapCardIntervalChanged(ui->swapCardIntervalValueLabel->text().toInt() * 1000);
    emit reloadSetting();
}

void SettingsDialog::checkUnbindInstrument() {
    if (0 == needReset()) {
        emit FocusUnBinding();
    }
}

void SettingsDialog::scanWlanAP() {
    if (ui->wlanButton->isChecked() == true) {
        QString wlanPsk = ui->wlanPskcomboBox->currentText();
        flashWlanPSKCombox(wlanPsk);
    }
}

void SettingsDialog::setVolume() {
    int vol = 0;
    IniParse client(CLIENT_CONFIG);
    if (!client.GetBool("Voice.Enabled")) {
        vol = 0;
    } else {
        vol = client.GetLong("Voice.Volume");
        vol = vol > 100 ? 100 : (vol < 0 ? 0 : vol);
    }
    volume(vol);
}

void SettingsDialog::setScreenSaverIntervals() {
    int screenSaverTiming[4];
    IniParse client(CLIENT_CONFIG);
    screenSaverTiming[0] = client.GetLong("Security.AutoCloseDialog") * 1000;
    screenSaverTiming[1] = client.GetLong("Energy.ScreenDim") * 1000;
    screenSaverTiming[2] = client.GetLong("Energy.ScreenOff") * 1000;
    screenSaverTiming[3] = 0;
    QWSServer::setScreenSaverIntervals(screenSaverTiming);
    //QWSServer::setScreenSaverBlockLevel(1);
}

/**
 * @brief 显示升级进度对话框
 */
void SettingsDialog::showDlProcessDlg()
{
    if (!dlProgressDlg)
    {
        //tr("正在下载文件中..."), tr("取消"), 0, 100, this)
        dlProgressDlg = new QProgressDialog(tr("Downloading a file..."), tr("Cancel"), 0, 100, this);
        connect(dlProgressDlg, SIGNAL(canceled()), this, SLOT(onDownloadCancelButtonClick()), Qt::QueuedConnection);
        connect(windowContext->getUpgrade(), SIGNAL(downloadProcess(int,int)),
                this, SLOT(onDownloadProcess(int,int)), Qt::QueuedConnection);
        //先断开信号槽连接，防止多次连接
        disconnect(windowContext->getUpgrade(), SIGNAL(stateChange(UPGRADE_STATE,int)),
                   this, SLOT(onStateChange(UPGRADE_STATE,int)));
        connect(windowContext->getUpgrade(), SIGNAL(stateChange(UPGRADE_STATE,int)),
                this, SLOT(onStateChange(UPGRADE_STATE,int)), Qt::QueuedConnection);
        dlProgressDlg->setValue(0);
        dlProgressDlg->setAutoClose(false);
        dlProgressDlg->setWindowModality(Qt::ApplicationModal);
        dlProgressDlg->show();
    }
}

/**
 * @brief 隐藏升级进度对话框
 */
void SettingsDialog::hideDlProcessDlg()
{
    if (dlProgressDlg)
    {
        disconnect(dlProgressDlg, SIGNAL(canceled()), this, SLOT(onDownloadCancelButtonClick()));
        disconnect(windowContext->getUpgrade(), SIGNAL(downloadProcess(int,int)),
                this, SLOT(onDownloadProcess(int,int)));
        delete dlProgressDlg;
        dlProgressDlg = NULL;
    }
}

void SettingsDialog::flashWlanPSKCombox(QString beforeSelect) {

    bool isEnable = ui->saveNetchoiceButton->isEnabled();
    int nowIndex = 0;
    int i = 0;
    ui->wlanPskcomboBox->clear();
    QString defaultStr = tr("SSID");//选择SSID
    ui->wlanPskcomboBox->addItem(defaultStr);
    if (defaultStr == beforeSelect) {
        nowIndex = 0;
    }

    CScanAPResults scanAPResults;

    scanAPResults.setResultFile(TQ2440_WLAN_CONF);
    foreach (SSID_INFO ssidInfo, scanAPResults.getAPList()) {
        if (ssidInfo.ssid == beforeSelect)
        {
            nowIndex = i + 1;
        }
        i++;

        int hex;
        QByteArray ba ;
        if(ssidInfo.ssid.contains("\\x",Qt::CaseInsensitive)){

            int index1 = ssidInfo.ssid.indexOf("\\x",0);
            QString str1 = ssidInfo.ssid.mid(0,index1);
            //qDebug()<<str1;
            int index2 = ssidInfo.ssid.lastIndexOf("\\x",-1);
            //qDebug()<<"ssidInfo.ssid.length()"<<ssidInfo.ssid.length();
            //qDebug()<<"index2"<<index2;

            for(int i = index1;i < index2+4;i+=4){
                hex = ssidInfo.ssid.mid(i+2,2).toInt(0,16);
                ba.append(hex);
            }
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
            QString text = codec->toUnicode(ba.data());
            //qDebug()<<"text"<<text;

            QString str2 =  ssidInfo.ssid.mid(index2+4,ssidInfo.ssid.length());
            //qDebug()<<str2;
            ui->wlanPskcomboBox->addItem("[" + QString::number(ssidInfo.level) + "]" + str1 + text + str2);
        }

        else{
            ui->wlanPskcomboBox->addItem("[" + QString::number(ssidInfo.level) + "]" + ssidInfo.ssid);
        }



    }

    ui->wlanPskcomboBox->setCurrentIndex(nowIndex);
    ui->saveNetchoiceButton->setEnabled(isEnable);
}

void SettingsDialog::flashNetTypeButton() {
    ui->saveNetchoiceButton->setEnabled(false);

    static int USE_WIFI = 0;
    static int USE_ETH = 1;
    bool netType = USE_WIFI;

    QFile ntcf(TQ2440_NETTYPE_CONF);
    if (ntcf.open(QIODevice::ReadOnly)) {
        QTextStream stream(&ntcf);
        QStringList items;
        while (!stream.atEnd()) {
            items.append(stream.readLine());
        }
                foreach(QString item, items) {
                QStringList list = item.split("=");
                if (list[0] == "NET") {
                    netType = (list[1] == "0" ? USE_WIFI : USE_ETH);
                    break;
                }
            }
        ntcf.close();
    } else {
        DEBUG_LOG("Failed to open" << TQ2440_NETTYPE_CONF);
    }
    if(netType == 0){
        IniParse osla(OSLA_CONFIG);
        if(osla.GetBool("Wlan.hideNet",true) == true){
            ui->HideWireless_radioButton->setChecked(true);
            ui->wlanButton->setChecked(false);
            ui->AddWlan_button->setEnabled(true);
        }else if (osla.GetBool("Wlan.hideNet",true) == false){
            ui->HideWireless_radioButton->setChecked(false);
            ui->wlanButton->setChecked(true);
            ui->AddWlan_button->setEnabled(false);
        }
    }


    ui->lanButton->setChecked(USE_ETH == netType ? true : false);

    onNetModeToggled(USE_WIFI == netType ? true : false);
}

void SettingsDialog::flashNetConfigLineEdit() {
    QFile netConfigFile(TQ2440_NET_CONF);
    if (netConfigFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&netConfigFile);
        QStringList items;
        while (!stream.atEnd()) {
            items.append(stream.readLine());
        }

                foreach(QString item, items) {
                QStringList list = item.split("=");
                if (list[0] == "IPADDR") {
                    ui->ipEdit->setText(list[1]);
                } else if (list[0] == "NETMASK") {
                    ui->netmaskEdit->setText(list[1]);
                } else if (list[0] == "GATEWAY") {
                    ui->gatewayEdit->setText(list[1]);
                }
            }

        //ipAddr = ui->ipEdit->text();
        //netmask = ui->netmaskEdit->text();
        //gateway = ui->gatewayEdit->text();
        netConfigFile.close();
    } else {
        DEBUG_LOG("Failed to open" << TQ2440_NET_CONF);
    }
}

void SettingsDialog::flashNetDHCP() {
    bool useDHCP = true;

    QFile netTypeConfigFile(TQ2440_NETTYPE_CONF);
    if (netTypeConfigFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&netTypeConfigFile);
        QStringList items;
        while (!stream.atEnd()) {
            items.append(stream.readLine());
        }
                foreach(QString item, items) {
                QStringList list = item.split("=");
                if (list[0] == "DHCP") {
                    if (list[1] == "0") {
                        useDHCP = false;
                    } else {
                        useDHCP = true;
                    }
                }
                break;
            }
        netTypeConfigFile.close();
    } else {
        DEBUG_LOG("Failed to open" << TQ2440_NETTYPE_CONF);
    }

    ui->autoButton->setChecked(useDHCP);
    ui->manualButton->setChecked(!useDHCP);
    onDHCPToggled(useDHCP);
}

void SettingsDialog::saveNetDetailConfig() {
    QFile netConfigFile(TQ2440_NET_CONF);
    if (netConfigFile.open(QIODevice::WriteOnly)) {
        //ipAddr = ui->ipEdit->text();
        //netmask = ui->netmaskEdit->text();
        //gateway = ui->gatewayEdit->text();
        QTextStream out(&netConfigFile);
        out << "IPADDR=" << ui->ipEdit->text().toLatin1() << endl;
        out << "NETMASK=" << ui->netmaskEdit->text().toLatin1() << endl;
        out << "GATEWAY=" << ui->gatewayEdit->text().toLatin1() << endl;
        DEBUG_LOG(TQ2440_NET_CONF << " updated");
        netConfigFile.close();
    } else {
        PROMPTINFO(this, "保存配置文件[nettype.conf]失败");
        //ui->ipEdit->setText(ipAddr);
        //ui->netmaskEdit->setText(netmask);
        //ui->gatewayEdit->setText(gateway);
    }
    QProcess::startDetached("sync");
}

void SettingsDialog::saveNetTypeConfig() {
    QFile netTypeConfigFile(TQ2440_NETTYPE_CONF);
    if (netTypeConfigFile.open(QIODevice::WriteOnly)) {
        //useDHCP = ui->autoButton->isChecked();
        QTextStream out(&netTypeConfigFile);
        out << "DHCP=" << (ui->autoButton->isChecked() ? 1 : 0) << endl;
        if(ui->HideWireless_radioButton->isChecked() == false){
             out << "NET=" << (ui->wlanButton->isChecked() ? 0 : 1) << endl;
        }else if(ui->HideWireless_radioButton->isChecked() == true){
             out << "NET=" << 0 << endl;
        }
        DEBUG_LOG(TQ2440_NETTYPE_CONF << "updated");
    } else {
        PROMPTINFO(this, "保存配置文件[nettype.conf]失败");
        DEBUG_LOG("Failed to open" << TQ2440_NETTYPE_CONF);
        //ui->autoButton->setChecked(useDHCP);
    }
    netTypeConfigFile.close();
    QProcess::startDetached("sync");
}

void SettingsDialog::saveHostConfig() {
    IniParse osla(OSLA_CONFIG);
    if (QString(osla.GetString("Network.SslHost").c_str()) != ui->serverHostEdit->text() ||
        QString(osla.GetString("Network.SslPort").c_str()) != ui->serverPortEdit->text()) {
        osla.Set("Network.SslHost", ui->serverHostEdit->text().toStdString());
        osla.Set("Network.SslPort", ui->serverPortEdit->text().toStdString());
        ui->serverPortEdit->isModified();
    }
    osla.Set("Network.NtpHost", ui->ntpEdit->text().toStdString());
    osla.SaveFile();

    NTG()->StartNtpService();
    emit serverUpdated();

    QFile::remove("/storage/conf/client.ini");
    QFile::copy("/etc/client.ini", "/storage/conf/client.ini");
    IniParse zhClient("/storage/conf/client.ini");
    zhClient.Set("HostInfo.IP", ui->serverHostEdit->text().toStdString());
    zhClient.SaveFile();
    QProcess::execute("killall zhClient");
    QProcess::execute("zhClient -d true -c /storage/conf/client.ini");

    QProcess::startDetached("sync");
}

void SettingsDialog::saveWlanConfig() {
    CWlanConfigFile wlanConfigFile;
    CScanAPResults scanAPResults;

    scanAPResults.setResultFile(TQ2440_WLAN_CONF);
    QString ssid = CWlanConfigFile::removeLevleForSsid(ui->wlanPskcomboBox->currentText());
    wlanConfigFile.setSsid(ssid);
    wlanConfigFile.setBssid(scanAPResults.getAPbssid(ssid));
    wlanConfigFile.setPassword(ui->wlanPskEdit->text());
    wlanConfigFile.writeConfigFile(TQ2440_WLAN_SET);

    QProcess::startDetached("sync");
}

void SettingsDialog::onTimerTimeout() {
    now = now.addSecs(1);
    QTime ok = now.time();
    if (0 == ok.second()) {
        SetShowTime(now);
    }
}

void SettingsDialog::UpdateYearCombox(int index) {
    QDate info = now.date();
    int februaryDays = (true == QDate::isLeapYear(index + 1970) ? 29 : 28);
    int daysArr[] = {31, februaryDays, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int day = info.day();
    if (day > daysArr[info.month() - 1]) {
        day = daysArr[info.month() - 1];
    }

    info.setYMD(index + 1970, info.month(), day);
    now.setDate(info);
    SetShowTime(now);
}

void SettingsDialog::UpdateMonthCombox(int index) {
    QDate info = now.date();
    int februaryDays = (true == QDate::isLeapYear(info.year()) ? 29 : 28);
    int daysArr[] = {31, februaryDays, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    int day = info.day();
    if (day > daysArr[index]) {
        day = daysArr[index];
    }

    info.setYMD(info.year(), index + 1, day);
    now.setDate(info);
    SetShowTime(now);
}

void SettingsDialog::UpdateDayCombox(int index) {
    QDate info = now.date();
    info.setYMD(info.year(), info.month(), index + 1);
    now.setDate(info);
    SetShowTime(now);
}

void SettingsDialog::UpdateHourCombox(int index) {
    QTime info = now.time();
    info.setHMS(index, info.minute(), info.second());
    now.setTime(info);
    SetShowTime(now);
}

void SettingsDialog::UpdateMinuteCombox(int index) {
    QTime info = now.time();
    info.setHMS(info.hour(), index, info.second());
    now.setTime(info);
    SetShowTime(now);
}

/**
 * @brief 按下升级按钮
 */
void SettingsDialog::onUpgradeButtonClick()
{
    if (!windowContext)
    {
        return;
    }

    windowContext->getUpgrade()->addUpgradeEvent(biotech::osla::SYSTEM_EVENT_UPGRADE_MANUAL);
    //检查仪器是否在使用中
    if (windowContext->getInstrumentStatus()->GetInUseStatus())
    {
        DEBUG_LOG("upgrade: manual failed, insturment is using!");
        windowContext->getUpgrade()->addUpgradeEvent(biotech::osla::SYSTEM_EVENT_UPGRADE_USING);
       //仪器正在使用中，不允许升级!
        QMessageBox::warning(this, tr("Notice"), tr("The instrument is in use and no upgrades are allowed!"), QMessageBox::Ok);
        return;
    }

    if (windowContext->getUpgrade()->isExecuting())
    {
        //正在升级中，请稍后再试
       windowContext->getUpgrade()->addUpgradeEvent(biotech::osla::SYSTEM_EVENT_UPGRADE_ANOTHER_PROC);
        DEBUG_LOG("upgrade: manual failed, insturment is updating!");
        QMessageBox::warning(this, tr("Notice"), tr("Upgrade in progress,please try again later"), QMessageBox::Ok);
        return;
    }

    if (!windowContext->getUpgrade()->isValidParms())
    {
        //没有配置适合的升级参数，请从Web管理界面配置。
        DEBUG_LOG("upgrade: manual failed, upgrade params is invalid!");
        windowContext->getUpgrade()->addUpgradeEvent(biotech::osla::SYSTEM_EVENT_UPGRADE_PARAMS_ERR);
        QMessageBox::warning(this, tr("Notice"), tr("No appropriate upgrade parameters are configured,\nplease confirgured,please configure from the web administration interface"), QMessageBox::Ok);
        return;
    }

    //检查是否有未同步记录
    if (windowContext->getUnsyncCount() > 0)
    {
        //仪器有未同步记录，确认升级吗？
        DEBUG_LOG("upgrade: manual failed, insturment has unsync data!");
        if (QMessageBox::question(this, tr("Notice"), tr("The instrument is not synchronized.Are you sure to upgrade"),
                              QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Cancel)
        {
            windowContext->getUpgrade()->addUpgradeEvent(biotech::osla::SYSTEM_EVENT_UPGRADE_USER_CANNEL);
            return;
        }
    }

    QString newVer;
    QString fromVer;
    QString currentVer;

    currentVer = windowContext->getUpgrade()->getDeviceVer();
    windowContext->getUpgrade()->getVer(&newVer, &fromVer);
    if (!windowContext->getUpgrade()->compareVer(currentVer, fromVer))
    {//当前版本:%1\n最低版本:%2\n当前版本过低，不能升级
        windowContext->getUpgrade()->addUpgradeEvent(biotech::osla::SYSTEM_EVENT_UPGRADE_VER_LOWER);
        QMessageBox::warning(this, tr("Notice"), tr("Current version:%1\nMinium version:%2\nThe current version is too low to upgrade").arg(currentVer).arg(fromVer));
        return;
    }

    if (windowContext->getUpgrade()->compareVer(currentVer, newVer))
    {//当前版本:%1\n最新版本:%2\n已经是最新版本，不能升级
        windowContext->getUpgrade()->addUpgradeEvent(biotech::osla::SYSTEM_EVENT_UPGRADE_VER_NEWEST);
        QMessageBox::warning(this, tr("Notice"), tr("Current version:%1\nLatest version:%2\nit is the latest version and cannot be upgraded").arg(currentVer).arg(newVer));
        return;
    }//当前版本:%1\n新版本:%2\n是否要升级？"
    if (QMessageBox::question(this, tr("Notice"), tr("Current version:%1\nNew version:%2\nWhether to upgrade?").arg(currentVer).arg(newVer),
                              QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Ok)
    {
        if (windowContext->getUpgrade()->startUpgrade())
        {
            DEBUG_LOG("upgrade: manual upgrade start!");
             windowContext->disableScreenSaver();
             windowContext->setCardEnabled(false);
             showDlProcessDlg();
        }
    }
}

/**
 * @brief 下载进度显示
 * @param dltotal
 * @param dlnow
 */
void SettingsDialog::onDownloadProcess(int dltotal, int dlnow)
{
    if (dltotal > 0)
    {
        long long per = dlnow;
        per *= 100;
        per = per / dltotal;
        dlProgressDlg->setValue(per);
    }
}

/**
 * @brief 升级线程状态改变时，显示相关提示
 * @param state
 * @param reason
 */
void SettingsDialog::onStateChange(UPGRADE_STATE state, int reason)
{
    switch (state)
    {
        case STATE_FAILED:
            DEBUG_LOG("upgrade: failed, " << windowContext->getUpgrade()->getFailedReasonString((enum UPGRADE_FAILED_REASON)reason).toStdString());
            hideDlProcessDlg();
            windowContext->setCardEnabled(true);
            windowContext->enableScreenSaver();
            QMessageBox::warning(this, tr("Notice"), windowContext->getUpgrade()->getFailedReasonString((enum UPGRADE_FAILED_REASON)reason),
                                 QMessageBox::Ok);
            break;
        case STATE_PACK_DOWNLOADED:
            hideDlProcessDlg();
            break;
        case STATE_UPGRADING:
            hideDlProcessDlg();
            break;
        default:
            DEBUG_LOG("upgrade: error,unknown state:!" << state);
            break;
    }
}

/**
 * @brief 点击了下载进度对话框的取消按钮
 */
void SettingsDialog::onDownloadCancelButtonClick()
{
    DEBUG_LOG("upgrade: error,  manual stop upgrade");
    windowContext->getUpgrade()->stopUpgrade();
}

void SettingsDialog::onRestartButtonClick()
{
    if (QMessageBox::question(this, tr("Notice"), tr("Are you sure you want to restart the terminal？"),
                              QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Ok)
    {
        system("/bin/restart.sh &");
        qApp->quit();
    }
}

void SettingsDialog::onLanguageCommoxClick()
{//您试图改变语言，点击确定系统将重启生效
    if (QMessageBox::question(this, tr("Notice"), tr("If you attempt to change the language, click OK to restart the system?"),
                              QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Ok)
    {
        system("/bin/restart.sh &");
        qApp->quit();
    }
}

void SettingsDialog::SetShowTime(QDateTime info) {
    DisConnectTimeCombox();
    QDate hello = info.date();
    QStringList year;
    for (int i = 1970; i <= 2070; ++i) {
       QString temp = QString(tr("%1 Year").arg(i));
        //temp.sprintf("%4d年", i);
        year.append(temp);
    }
    ui->yearCombox->clear();
    ui->yearCombox->addItems(year);
    ui->yearCombox->setCurrentIndex(hello.year() - 1970);

    QStringList month;
    for (int i = 1; i <= 12; ++i) {
        QString temp = QString(tr("%1 Month").arg(i));
        //temp.sprintf("%02d月", i);
        month.append(temp);
    }
    ui->monthCombox->clear();
    ui->monthCombox->addItems(month);
    ui->monthCombox->setCurrentIndex(hello.month() - 1);

    int februaryDays = (true == hello.isLeapYear(hello.year()) ? 29 : 28);
    int daysArr[] = {31, februaryDays, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    QStringList day;
    for (int i = 1; i <= daysArr[hello.month() - 1]; ++i) {
        QString temp = QString(tr("%1 Day").arg(i));
       // temp.sprintf("%02d日", i);
        day.append(temp);
    }
    ui->dayCombox->clear();
    ui->dayCombox->addItems(day);
    ui->dayCombox->setCurrentIndex(hello.day() - 1);

    QTime world = info.time();

    QStringList hour;
    for (int i = 0; i <= 23; ++i) {
        QString temp = QString(tr("%1 Hour").arg(i));
        //temp.sprintf("%02d时", i);
        hour.append(temp);
    }
    ui->hourCombox->clear();
    ui->hourCombox->addItems(hour);
    ui->hourCombox->setCurrentIndex(world.hour());

    QStringList minute;
    for (int i = 0; i <= 59; ++i) {
        QString temp = QString(tr("%1 Minute").arg(i));
        // temp.sprintf("%02d分", i);
        minute.append(temp);
    }
    ui->minuteCombox->clear();
    ui->minuteCombox->addItems(minute);
    ui->minuteCombox->setCurrentIndex(world.minute());
    ConnectTimeCombox();
}

void SettingsDialog::ConnectTimeCombox() {
    connect(ui->yearCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateYearCombox(int)));
    connect(ui->monthCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateMonthCombox(int)));
    connect(ui->dayCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateDayCombox(int)));
    connect(ui->hourCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateHourCombox(int)));
    connect(ui->minuteCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateMinuteCombox(int)));
}

void SettingsDialog::DisConnectTimeCombox() {
    disconnect(ui->yearCombox, NULL, this, NULL);
    disconnect(ui->monthCombox, NULL, this, NULL);
    disconnect(ui->dayCombox, NULL, this, NULL);
    disconnect(ui->hourCombox, NULL, this, NULL);
    disconnect(ui->minuteCombox, NULL, this, NULL);
}

void SettingsDialog::saveTime() {
    if (QProcess::execute(
            QString("date -s %1-%2").arg(now.date().toString("yyyy.MM.dd")).arg(now.time().toString("hh:mm"))) == 0) {
        QProcess::execute("hwclock -w --utc");
    }
}


void SettingsDialog::on_tsCalButton_clicked()
{//tr("注意"), tr("进行触摸屏校准需要重起终端，你确定吗？")
    if (QMessageBox::warning(this, tr("Notice"), tr("To calibrate the touch screen,\nyou need to restart the terminal,Are you sure?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        system("touch /storage/data/ts_cal");
        system("reboot");
    }
}

void SettingsDialog::on_copyLogButton_clicked()
{
    if (!windowContext->getInstrumentStatus()->GetInUseStatus())
    {//tr(""), tr("请先开机，再复制终端日志。"));
        QMessageBox::information(this, tr(""), tr("Please boot the terminal log before copying it"));
        return;
    }
    system("cp /tmp/controller.log /storage/sd0/ftp/sambaRootPath/");
    system("cp /tmp/wizard.log /storage/sd0/ftp/sambaRootPath/");
    //tr("终端日志复制完成。"))
    QMessageBox::information(this, tr(""), tr("Terminial log replication is complete"));
}

void SettingsDialog::on_formatCardButton_clicked()
{//tr("注意"), tr("进行格式化SD卡需要重起终端，你确定吗？")
    if (QMessageBox::warning(this, tr("Notice"), tr("To formant the SD card,\nyou need to restart the terminal,Are you sure"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        system("touch /storage/data/format_sd");
        system("reboot");
    }
}
void SettingsDialog::on_hideSsidInputError(bool status)
{
    ui->saveNetchoiceButton->setEnabled(false);
}

void SettingsDialog::on_AddWlan_button_clicked()
{
    wlandialog = new AddWlanDialog(this);
    connect(wlandialog,SIGNAL(hideSsidInputError(bool)),this,SLOT(on_hideSsidInputError(bool)));
    wlandialog->setAttribute(Qt::WA_QuitOnClose,false);
    //wlandialog->setModal(true);
    wlandialog->open();
}
