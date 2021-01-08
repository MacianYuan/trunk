//
// Created by 何继胜 on 2017/4/28.
//
#include <QtGui/QMessageBox>
#include <QtCore/QFile>
#include <QtGui/QApplication>

#include <deviceinfo/enum.h>
#include <deviceinfo/version.h>
#include <sqlworker/sqlworker.h>
#include <tools/IniParse.h>
#include <tools/common.h>
#include <tools/log.h>

#include "controller/Context/WindowContext.h"
#include "controller/Context/BookInformation.h"
#include "controller/Context/ReservationInformation.h"
#include "controller/Context/UsbNetConnection.h"
#include "controller/Context/PowerDelay.h"
#include "controller/UI/EntrustBeginDialog.h"
#include "controller/Base/ftp.h"
#include "controller/Context/NetConnectionManage.h"
#include "controller/Base/GlobalData.h"
#include "controller/Context/UserInformation.h"
#include "controller/Context/InstrumentStatus.h"
#include "controller/Base/NormalTools.h"
#include "controller/UI/SettingsDialog.h"
#include "ServiceEvent.h"
#include "upgrade/qupgradeplan.h"
#include "controller/UI/ScreenSaver.h"

static void UpdateUploadProgressFtpEvent(void *ctx, Progress &info);

WindowContext::WindowContext(QObject *parent) : QObject(parent) {

    //初始化升级对象, added by lwp, 2018-1-13
    upgrade = new QUpgrade(parent, this);
    upgrade->start();
    upgradePlan = new QUpgradePlan(upgrade, parent);
    upgradeDialog = NULL;
    connect(upgrade, SIGNAL(stateChange(UPGRADE_STATE,int)),
            this, SLOT(onStateChange(UPGRADE_STATE,int)));

    /// 初始化wifi管理，如是是使用wifi模式，负责网络连接工作
    netConnectionManage = new NetConnectionManage(parent);
    netConnectionManage->start();

    /// 生成管理员、设置页面、升级页面
    adminDialog = new AdminDialog((QWidget *) parent);
    settingsDialog = new SettingsDialog((QWidget *) parent);
    settingsDialog->SetNetConnectionManage(netConnectionManage);
    settingsDialog->setWindowContext(this);
    supportFeature = new SupportFeature();

    usbNetConnection = new UsbNetConnection(parent);
    powerDelay = new PowerDelay(parent);
    instrumentStatus = new InstrumentStatus(parent);
    userInformation = new UserInformation(parent);
    bookInformation = new BookInformation(parent);
    bookInformation->InitWindowContext(this);
    reservationInformation = new ReservationInformation(parent);
    cardReader = new CardReader(parent);

    nowUserBooking.Clear();
}

WindowContext::~WindowContext()
{
    if (upgradeDialog)
    {
        delete upgradeDialog;
    }
}

bool WindowContext::init() {
    supportFeature->Parse();

    /// 加载配置

    /// 刷卡器初始化
    IniParse ini(CLIENT_CONFIG);
    cardGuard = ini.GetLong("Reader.Guarding", 5 * 1000);
    cardReader->setGuardInterval(cardGuard);
    cardReader->open();
    cardEnabled = true;

    /// 初始化ftp登陆信息
    ftp = new FtpParse();
    ftp->SetNetStatus(false);
    InitFtp();
    InitGlobal();
}

FtpParse *WindowContext::getFtp() const {
    return ftp;
}

NetConnectionManage *WindowContext::getNetConnectionManage() const {
    return netConnectionManage;
}

UsbNetConnection *WindowContext::getUsbNetConnection() const {
    return usbNetConnection;
}

PowerDelay *WindowContext::getPowerDelay() const {
    return powerDelay;
}

bool WindowContext::isUseAlertTime() const {
    return useAlertTime;
}

InstrumentStatus *WindowContext::getInstrumentStatus() const {
    return instrumentStatus;
}

UserInformation *WindowContext::getUserInformation() const {
    return userInformation;
}

BookInformation *WindowContext::getBookInformation() const {
    return bookInformation;
}

long WindowContext::getTimeCheckNum() const {
    return timeCheckNum;
}

biotech::osla::Booking &WindowContext::getNowUserBooking() {
    return nowUserBooking;
}


SupportFeature *WindowContext::getSupportFeature() const {
    return supportFeature;
}

void WindowContext::setUseAlertTime(bool useAlertTime) {
    WindowContext::useAlertTime = useAlertTime;
}

AdminDialog *WindowContext::getAdminDialog() const {
    return adminDialog;
}

SettingsDialog *WindowContext::getSettingsDialog() const {
    return settingsDialog;
}


bool WindowContext::LoginCheck() {
    /// 用户卡校验
    if (false == userInformation->NewUserCardCheck()) {
        return false;
    }

    /// 设备状态校验
    if (false == instrumentStatus->InstrumentStatusCheck(userInformation->GetNewUser())) {
        return false;
    }

    /// 用户账户校验
    bool denyDeposit = true;
    if (extraConfig.enableDepositUser && isBookingUserInBookingTime())
    {
        denyDeposit = false;
    }
    if (false == userInformation->NewUserAccountCheck(isPaymentAaccountEnabled(), denyDeposit)) {
        return false;
    }

    return true;
}

bool WindowContext::BookingCheck() {
    struct _TempNode_ {
        bool status;
        std::function<bool()> fun;
    };

    /// 预约检查
    User &newUser = userInformation->GetNewUser();
    _TempNode_ arr_2[] = {
            // 是否预约锁定中,并且刷卡用户非预约用户
            {
                    (true == bookInformation->IsBookHolding(newUser.timestamp, instrumentStatus) &&
                     false == bookInformation->IsCurrentBookUser(newUser)),
                    std::bind(&WindowContext::BookingLock, this)
            },

            // 是否专享锁定中，并且判断当前用户是否在共享组中
            {
                    (false == bookInformation->IsBookHolding(newUser.timestamp, instrumentStatus) &&
                     InstrumentStatus::RESERVATION == instrumentStatus->GetWorkStatus() &&
                     newUser.account.groupId != (uint) (reservationInformation->getGroupId())),
                    std::bind(&WindowContext::ReservationLock, this)
            },

            // 用户资格判断
            {
                    (false == bookInformation->IsBookHolding(newUser.timestamp, instrumentStatus) &&
                     false == userInformation->UserQualificationCheck()),
                    // 不设置函数
            },
    };

    for (int i = 0; i < (int) (sizeof(arr_2) / sizeof(arr_2[0])); ++i) {
        if (true == arr_2[i].status) {
            if (nullptr != arr_2[i].fun) {
                arr_2[i].fun();
            }

            return false;
        }
    }


    return true;
}

bool WindowContext::isBookingUserInBookingTime()
{
    User &newUser = userInformation->GetNewUser();

    if (bookInformation->IsBookHolding(newUser.timestamp, instrumentStatus) &&
            bookInformation->IsCurrentBookUser(newUser))
    {
        return true;
    }

    return false;
}

bool WindowContext::BookingLock() {
    //预约锁定
    PROMPTINFO(parent(), tr("Make an appointment to lock"), "media/booking.mp3", true, 500);
    User &newUser = userInformation->GetNewUser();
    SE_G->AddUserEvent(newUser, biotech::osla::USER_ACTION_START_SESSION,
                       biotech::osla::USER_ACTION_FAILED_BOOKING_HOLDING);
    return true;
}

bool WindowContext::ReservationLock() {
    User &newUser = userInformation->GetNewUser();
    //专享锁定
    PROMPTINFO(parent(), tr("Exclusive lock"), "media/reservation.mp3", true, 500);
    SE_G->AddUserEvent(newUser, biotech::osla::USER_ACTION_START_SESSION,
                       biotech::osla::USER_ACTION_FAILED_RESERVATION_HOLDING);
    return true;
}

void WindowContext::onStateChange(UPGRADE_STATE state, int reason)
{
    static bool isUpdating = false;
    if (state == STATE_UPGRADING)
    {
        showUpgradeDlg();
        isUpdating = true;
    }
    else
    {
        if (isUpdating)
        {
            hidUpgradeDlg();
            isUpdating = false;
        }
    }
}

bool WindowContext::ExportData() {
    SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();
    QVector<int> countList = sqlWorker->unsyncedRecordCount();

    if (countList.contains(-1)) {
        //
        PROMPTINFO(parent(), tr("Database query error"));
        return false;
    }

    int total = 0;

    for (int i = 0; i < countList.size(); i++) {
        total += countList[i];
    }

    if (total > 0) {
        //tr("注意"),tr("客户端还有%1条数据记录未同步，是否继续导出？")
        QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                                  tr("The client still has %1 data record that is not synchronized.\nDo you want to continue exporting?").arg(total),
                                                  QMessageBox::Yes | QMessageBox::No, (QWidget *) parent());
        messageBox->setDefaultButton(QMessageBox::No);
        messageBox->setAttribute(Qt::WA_DeleteOnClose);

        if (messageBox->exec() != QMessageBox::Yes) {
            return false;
        }
    }

    // 禁止刷卡
    cardEnabled = false;

//    if (true == instrumentStatus->GetNetWorkStatus()) {
//        sessionManage->EndSession();
//    }

    QString exportDataPath;
    ftp->UserLoginMustStartSmb(0, FtpSessionInfo(), exportDataPath);

    IniParse ini(OSLA_CONFIG);
    QString sambaPath(ini.GetString("Ftp.sambapath", "/storage/sd0").c_str());
    QString recordePath = sambaPath + "/sambaRootPath";
    QString tempPath = sambaPath + "/sambaRootPathTemp";

    sqlWorker->exportRecordEvent(recordePath);
    QProcess::execute("rm -fr " + tempPath);
    QProcess::execute("mv " + recordePath + " " + tempPath);
    QProcess::execute("mkdir -p " + recordePath);

    //ftp->UserLogout();

    QProcess::execute("mv " + tempPath + " " + recordePath);
    //数据导出成功，请连接共享文件夹
    PROMPTINFO(parent(), tr("Data export successful,please connect to shared folder"));
    cardEnabled = true;

    return true;
}

void WindowContext::RequestUnbinding() {
    DEBUG_LOG(__FUNCTION__);
    // 检查仪器是否在使用中
    //仪器正在使用中，请结束使用后再试
    if (true == instrumentStatus->GetInUseStatus()) {
        PROMPTINFO(parent(), tr("The instrument is in use,please try again after use"));
        return;
    }

    // 检查网络状态
    //设备处于离线状态，强制解除仪器关联将无法得到服务器确认，是否继续？
    if (false == instrumentStatus->GetNetWorkStatus()) {
        QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                                  tr("If the device is offline,\nit cannot be confirmed by the server whether to continue?"),
                                                  QMessageBox::Yes | QMessageBox::No, (QWidget *) parent());
        messageBox->setDefaultButton(QMessageBox::No);
        messageBox->setAttribute(Qt::WA_DeleteOnClose);

        if (messageBox->exec() != QMessageBox::Yes) {
            return;
        }
    }

    // 检查未同步数据记录
    SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();
    QVector<int> countList = sqlWorker->unsyncedRecordCount();

    if (countList.contains(-1)) {
        //数据库查询错误
        PROMPTINFO(parent(), tr("Database query error"));
        return;
    }

    int total = 0;

    for (int i = 0; i < countList.size(); i++) {
        total += countList[i];
    }

    if (total > 0) {
        //客户端还有%1条数据记录未同步，强制解除仪器关联将丢失相关数据，是否继续
        QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                                  tr("There is still %1 data record on the client that is not synchronized.\nIf the instrument association is forced to be removed,relevant data will be lost,Shall we continue?").arg(total),
                                                  QMessageBox::Yes | QMessageBox::No, (QWidget *) parent());
        messageBox->setDefaultButton(QMessageBox::No);
        messageBox->setAttribute(Qt::WA_DeleteOnClose);

        if (messageBox->exec() != QMessageBox::Yes) {
            return;
        }
    }

    // 禁止刷卡
    cardEnabled = false;

    if (true == instrumentStatus->GetNetWorkStatus()) {
        PROMPTINFO(parent(), tr("Send an instrument decoupling request to the server..."));
        DEBUG_LOG(__FUNCTION__);
        emit sendRequestUnbinding();//离线解除仪器关联后请到服务器上手动重置仪器与本终端的关联
    } else {
        PROMPTINFO(parent(), tr("After disconnecting the instrument offline,\nplease reset the connextion between the instrument and the terminal manually on the server"));
        msleep(2000);
        FactoryReset();
    }
    return;
}

void WindowContext::FactoryReset() {
    //"仪器关联已解除，删除本地数据..."
    PROMPTINFO(parent(), tr("Instrument association cancelled.Local data deleted..."));

    // 删除仪器配置文件/SSL证书/数据库文件
    IniParse ini(OSLA_CONFIG);
    QFile::remove(ini.GetString("Network.SslCert").c_str());
    QFile::remove(OSLA_CONFIG);

    SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();
    sqlWorker->removeDatabase();

    msleep(2000);
    //本地数据已删除，程序即将重启
    PROMPTINFO(parent(), tr("Local data to delete,the program is about to restart..."));
    msleep(2000);

    // 关闭窗口
    NTG()->ClosePopupWindow();

    // 退出程序，启动wizard
    qApp->exit(773);
}

bool WindowContext::InitFtp() {
    bool supportFtp = supportFeature->isSupportFtp();
    DEBUG_LOG("this version hardware support ftp function: " << (supportFtp ? "true" : "false"));
    if (false == supportFtp) {
        return false;
    }

    DEBUG_LOG("init ftp info");
    ftp->Init(UpdateUploadProgressFtpEvent, NULL);

    if (true == instrumentStatus->GetInUseStatus()) {
        SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();
        quint64 lastId = sqlWorker->getLastId("select ID from TUserEvent order by localid desc limit 1");
        DEBUG_LOG("handler ftp exception");
        User &currentUser = userInformation->GetCurrentUser();
        biotech::osla::Session &session = userInformation->GetSessionInfo();
        FtpSessionInfo fsi;
        fsi.user_id = currentUser.id;
        fsi.instrument_id = instrumentStatus->GetId();
        fsi.group_id = session.group_id();
        fsi.actived_account = session.account_id();
        ftp->HandlerException(lastId, fsi);
    }

    DEBUG_LOG("start ftp thread");
    ftp->start();

    return true;
}

void WindowContext::CheckExpiredBookings() {
    // 把刷卡机关机期间过期的预约终止
    DEBUG_LOG("MainWindow: Checking for expired bookings");
    SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();
    int numRecords = sqlWorker->endExpiredBookings(QDateTime::currentDateTime());
    DEBUG_LOG("MainWindow:" << numRecords << "expired bookings terminated");
}

CardReader *WindowContext::getCardReader() const {
    return cardReader;
}

bool WindowContext::InitGlobal() {
    IniParse ini(OSLA_CONFIG);
    globalConfig.set_account_deposit_limit(ini.GetLong("Instrument.AccountDepositLimit"));
    globalConfig.set_working_time(ini.GetString("Instrument.WorkingTime"));
    //globalConfig.set_payment_account_enable(ini.GetBool("Global.PaymentAccountEnable", true));
    globalConfig.set_payment_account_enable(true);
    ini.Get("Global.CaptureTime", captureTime);
    ini.Get("Global.ServiceTime", serviceTime);
    ini.Get("Instrument.LooseMode", looseMode);

    extraConfig.enableDepositUser = ini.GetBool("Global.enableDepositUser", false);

    return true;
}

void WindowContext::reloadFtpParams()
{
    ftp->loadFtpParams();
}

int WindowContext::getCardGuard() const {
    return cardGuard;
}

bool WindowContext::isCardEnabled() const {
    return cardEnabled;
}

void WindowContext::setCardEnabled(bool cardEnabled) {
    WindowContext::cardEnabled = cardEnabled;
}

void WindowContext::setCardGuard(int cardGuard) {
    WindowContext::cardGuard = cardGuard;
}

const biotech::osla::GlobalConfig &WindowContext::getGlobalConfig() const {
    return globalConfig;
}

ReservationInformation *WindowContext::getReservationInformation() const {
    return reservationInformation;
}

const std::string &WindowContext::getDeviceVersion() {
    static std::string version;
    if (0 == version.length()) {
        std::ifstream fin{"/etc/Version", std::ios::in};
        char line[128] = {0};
        fin.read(line, sizeof(line));
        char var[128] = {0};
        sscanf(line, "%*[a-zA-Z0-9]-%*[a-zA-Z0-9]-%[^_]", var);
        fin.close();
        version = var;
    }
    return version;
}

bool WindowContext::UpdateNowUserBooking(int bookingId) {
    SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();
    nowUserBooking = sqlWorker->getBookingById(bookingId);
    DEBUG_LOG("now user booking id is : " << nowUserBooking.id());
    return true;
}

long WindowContext::getServiceTime() const {
    return serviceTime;
}

long WindowContext::getCaptureTime() const {
    return captureTime;
}

bool WindowContext::isLooseMode() const {
    return looseMode;
}

/**
 * @brief 检查未同步的记录
 * added by lwp, 2018-1-13
 * @return 未同步的记录条数
 */
int WindowContext::getUnsyncCount() const
{
    SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();
    QVector<int> countList = sqlWorker->unsyncedRecordCount();

    if (countList.contains(-1)) {
        //数据库查询错误
        PROMPTINFO(parent(), tr("Database query error"));
        return false;
    }

    int total = 0;

    for (int i = 0; i < countList.size(); i++) {
        total += countList[i];
    }

    return total;
}

QUpgrade *WindowContext::getUpgrade() const
{
    return upgrade;
}

QUpgradePlan *WindowContext::getUpgradePlan() const
{
    return upgradePlan;
}

/**
 * @brief 显示更新对话框
 */
void WindowContext::showUpgradeDlg()
{
    if (!upgradeDialog)
    {
        disableScreenSaver();
        upgradeDialog = new UpgradingDialog(NULL);
        upgradeDialog->setWindowModality(Qt::ApplicationModal);
        upgradeDialog->showFullScreen();
    }
}

void WindowContext::hidUpgradeDlg()
{
    if (upgradeDialog)
    {
        delete upgradeDialog;
        upgradeDialog = NULL;
        enableScreenSaver();
    }
}

void WindowContext::disableScreenSaver()
{
    ScreenSaver::setEnable(false);
}

bool WindowContext::isPaymentAaccountEnabled()
{
    if (globalConfig.has_payment_account_enable() && globalConfig.payment_account_enable())
    {
        return true;
    }

    return false;
}

void WindowContext::enableScreenSaver()
{
    ScreenSaver::setEnable(true);
}

void UpdateUploadProgressFtpEvent(void *ctx, Progress &info) {
    (void *) ctx;

    DEBUG_LOG("id : " << info.id);
    DEBUG_LOG("user id : " << info.userId);
    DEBUG_LOG("instrument id : " << info.instrumentId);
    DEBUG_LOG("actived account : " << info.actived_account);
    DEBUG_LOG("group id : " << info.group_id);
    DEBUG_LOG("path : " << info.path.toLocal8Bit().data());
    DEBUG_LOG("file name : " << info.fileName.toLocal8Bit().data());
    DEBUG_LOG("percentage : " << info.percentage);

    biotech::osla::FtpEvent ftpEvent;
    ftpEvent.set_id(info.id);
    ftpEvent.set_user_id(info.userId);
    ftpEvent.set_instrument_id(info.instrumentId);
    ftpEvent.set_group_id(info.group_id);
    ftpEvent.set_actived_account(info.actived_account);
    biotech::osla::FtpFileUpload *ffu = ftpEvent.add_file();
    //TODO:若路径包括中文字符，则info.path必须使用toUtf8()方法。但info.fileName也包括中文字符，
    //则必须调用toLatin1()方法，而不能用toUtf8()。原因未知。
    ffu->set_path(info.path.toUtf8().data(), info.path.toUtf8().size());
    ffu->set_file_name(info.fileName.toLatin1().data(), info.fileName.toLatin1().size());
    ffu->set_consume_time(info.consume_time);
    ffu->set_file_size(info.fileSize);
    ffu->set_create_time(info.createTime.toLocal8Bit().data());
    ffu->set_percentage(info.percentage);
    ServiceEvent::GetInstance()->AddFtpEvent(ftpEvent);
}
