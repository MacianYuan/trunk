//
// Created by 何继胜 on 2017/5/2.
//

#include <deviceinfo/enum.h>
#include <controller/Base/NormalTools.h>
#include <deviceinfo/typedef.h>
#include <ftp/QTFtpInfo.h>
#include <tools/log.h>
#include <tools/IniParse.h>
#include <deviceinfo/version.h>
#include <controller/Base/SmallDefine.h>
#include "SessionManage.h"
#include "controller/Context/UserInformation.h"
#include "controller/Context/InstrumentStatus.h"
#include "controller/Context/PowerDelay.h"
#include "sqlworker/sqlworker.h"
#include "controller/Base/ftp.h"
#include "controller/Context/BookInformation.h"
#include "controller/Context/UsbNetConnection.h"
#include "ServiceEvent.h"

SessionManage::SessionManage(QObject *parent) : QObject(parent) {
}

bool SessionManage::RestoreSession() {
    /// 通过session回复user信息
    UserInformation *m_UserInformation = windowContext->getUserInformation();
    InstrumentStatus *m_InstrumentStatus = windowContext->getInstrumentStatus();
    PowerDelay *m_PowerDelay = windowContext->getPowerDelay();

    m_UserInformation->InitFromDB();
    biotech::osla::Session &session = m_UserInformation->GetSessionInfo();

    /// 如果这个用户已经不存在
    if (0 == session.id()) {
        m_InstrumentStatus->SetInUseStatus(false);
        m_PowerDelay->StartUpParse();
        return true;
    }

    m_InstrumentStatus->SetInUseStatus(true);

    // 恢复重启前的使用状态
    m_PowerDelay->SetInstrumentEnabled(true);
    emit ShowCurrentUserInfo(); // 更新界面当前用户信息
    m_InstrumentStatus->SetWorkStatusBySessionType(session.type()); // 根据使用记录的类型恢复当前工作状态
    emit SetCurrentStateInfo(); // 更新界面当前状态信息

    if (biotech::osla::SESSION_TYPE_ASSISTANCE == session.type() ||
        biotech::osla::SESSION_TYPE_TRAINING == session.type() ||
        biotech::osla::SESSION_TYPE_EDUCATION == session.type() ||
        biotech::osla::SESSION_TYPE_NORMAL == session.type() ||
        biotech::osla::SESSION_TYPE_ENTRUST == session.type() ||
        biotech::osla::SESSION_TYPE_MAINTENANCE == session.type()) {
        NTG()->SetDeviceWorkStatusLed(true);
    }

    ///根据当前用户，重新生成ftp目录
    SupportFeature *supportFeature = windowContext->getSupportFeature();
    if (true == supportFeature->isSupportFtp()) {
        //SqlWorker *sqlWorker = windowContext->getSqlWorker();
        QString userSambaDir;
        SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
        quint64 lastId = sqlWorker->getLastId("select ID from TUserEvent order by localid desc limit 1");
        User &currentUser = m_UserInformation->GetCurrentUser();
        FtpSessionInfo fsi;
        fsi.user_id = currentUser.id;
        fsi.instrument_id = m_InstrumentStatus->GetId();
        fsi.group_id = session.group_id();
        fsi.actived_account = session.account_id();
        FtpParse *ftp = windowContext->getFtp();
        ftp->UserLogin(lastId, fsi, userSambaDir);
    }

    windowContext->UpdateNowUserBooking(session.booking_id());

    // 是否有有效预约
    BookInformation *bookInformation = windowContext->getBookInformation();
    biotech::osla::Booking currentBooking = bookInformation->GetCurrentBook();

    if (currentBooking.id() == 0 || (uint) currentBooking.status() != biotech::osla::BOOKING_STATUS_VALID) {
        /// 如果预约状态不是BOOKING_STATUS_VALID，说明重启这个操作，是马上进行的，间隔时间不长;
        /// 如果是长时间的，在启动的时候，检查预约有效性的时候，就给清楚掉了
        return true;
    }

    windowContext->setUseAlertTime(false); /// 如果发生重启事件，使用超时可能会产生两次
    bookInformation->RefreshShowBookInfo(m_UserInformation, m_InstrumentStatus);

    return true;
}

bool SessionManage::StartSession(SessionNode &node) {
    DEBUG_LOG("");
    windowContext->setUseAlertTime(false);
    PowerDelay *powerDelay = windowContext->getPowerDelay();
    if (false == powerDelay->StartSesssion()) {
        return false;
    }

    UserInformation *userInformation = windowContext->getUserInformation();
    biotech::osla::Session &session = userInformation->GetSessionInfo();
    session.set_type(node.session_type);

    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    if (node.instruction_type > 0) {
        instrumentStatus->SetWorkStatus((InstrumentStatus::WorkingState) node.instruction_type);
    }

    if (false == userInformation->GetNewSessionFromNewUser()) {
        return false;
    }

    PromptStartSession();                     /// 给出提示信息
    instrumentStatus->SetInUseStatus(true);   /// 设定仪器正在使用的标志
    userInformation->StartSession();          /// 保存当前用户
    emit ShowCurrentUserInfo();               /// 显示当前新用户信息
    SendLogin();                              /// 向第三方服务器发送登陆命令
    powerDelay->SetInstrumentEnabled(true);   /// 设备上电


    /// 向服务器发送给新事件信息
    User &currentUser = userInformation->GetCurrentUser();
    ServiceEvent::GetInstance()->AddSessionEvent(session, false);
    ServiceEvent::GetInstance()->AddUserEvent(currentUser, node.action_type, biotech::osla::USER_ACTION_SUCCEED);
    emit SetCurrentStateInfo();

    /// 启动ftp功能
    SupportFeature *supportFeature = windowContext->getSupportFeature();
    if (true == supportFeature->isSupportFtp()) {
        QString userSambaDir;
        SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
        quint64 lastId = sqlWorker->getLastId("select ID from TUserEvent order by localid desc limit 1");
        FtpSessionInfo fsi;
        fsi.user_id = currentUser.id;
        fsi.instrument_id = instrumentStatus->GetId();
        fsi.group_id = session.group_id();
        fsi.actived_account = session.account_id();
        FtpParse *ftp = windowContext->getFtp();
        ftp->UserLogin(lastId, fsi, userSambaDir);
        DEBUG_LOG("user login success, this samba path is [" << userSambaDir.toLocal8Bit().data() << "]");
    }

    /// 更新booking信息
    BookInformation *bookInformation = windowContext->getBookInformation();
    bookInformation->UpdateBookInfo(userInformation, instrumentStatus, windowContext->getNowUserBooking());
    NTG()->SetDeviceWorkStatusLed(true);
    DEBUG_LOG(__FUNCTION__ << " end");

    return true;
}

void SessionManage::EndSession() {
    windowContext->setUseAlertTime(false);
    windowContext->getNowUserBooking().Clear();
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    instrumentStatus->SetInUseStatus(false);   /// 设置设备不在使用
    emit ShowCurrentUserInfo();                       /// 更新用户信息
    PromptEndSession();                          /// 提示退出信息

    /// 更新使用记录的完成标志
    UserInformation *userInformation = windowContext->getUserInformation();
    User &currentUser = userInformation->GetCurrentUser();
    biotech::osla::Session &session = userInformation->GetSessionInfo();

    session.set_end_time(QDateTime::currentDateTime().toTime_t());
    ServiceEvent::GetInstance()->AddSessionEvent(session, true);

    /// 更新培训用户记录的完成标志
    if (InstrumentStatus::TRAINING == instrumentStatus->GetWorkStatus() ||
        InstrumentStatus::EDUCATION == instrumentStatus->GetWorkStatus()) {
        ServiceEvent::GetInstance()->AddTraningEvent((quint64) session.id());
    }

    // 更新仪器状态
    instrumentStatus->SetWorkStatus(InstrumentStatus::NORMAL);
    IniParse ini(OSLA_CONFIG);
    instrumentStatus->SetInstrumentStatus(ini.GetLong("Instrument.Status"));
    emit SetCurrentStateInfo();

    SendLogout();
    PowerDelay *powerDelay = windowContext->getPowerDelay();
    powerDelay->EndSession();

    SupportFeature *supportFeature = windowContext->getSupportFeature();
    if (true == supportFeature->isSupportFtp()) {
        FtpParse *ftp = windowContext->getFtp();
        ftp->UserLogout();
    }
    ServiceEvent::GetInstance()->AddUserEvent(currentUser,
                                              biotech::osla::USER_ACTION_END_SESSION,
                                              biotech::osla::USER_ACTION_SUCCEED);
    userInformation->EndSession();
}


void SessionManage::SendLogin() {
    UserInformation *userInformation = windowContext->getUserInformation();
    User &currentUser = userInformation->GetCurrentUser();
    UsbNetConnection *usbNetConnection = windowContext->getUsbNetConnection();
    uint card = 0;
    sscanf(currentUser.card.serial.c_str(), "%u", &card);
    usbNetConnection->SendLoginRequest(card);
}

void SessionManage::SendLogout() {
    //此处功能无用
    UserInformation *userInformation = windowContext->getUserInformation();
    User &currentUser = userInformation->GetCurrentUser();
    UsbNetConnection *usbNetConnection = windowContext->getUsbNetConnection();
    uint card = 0;
    sscanf(currentUser.card.serial.c_str(), "%u", &card);
    usbNetConnection->SendLogoutRequest(card);
}

void SessionManage::SendHeartbeat() {
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    UsbNetConnection *usbNetConnection = windowContext->getUsbNetConnection();
    if (true == instrumentStatus->GetInUseStatus()) {
        usbNetConnection->SendHeartbeatInUse();
        usbNetConnection->SendToWin_FTP();
    } else {
        usbNetConnection->SendHeartbeatIdle();
    }
}


void SessionManage::Init(WindowContext *context) {
    ZY_ASSERT(nullptr != context, "context is null, goooooog");
    this->windowContext = context;
}


void SessionManage::PromptStartSession() {
    QString info;
    UserInformation *userInformation = windowContext->getUserInformation();
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    User &newUser = userInformation->GetNewUser();
    const biotech::osla::GlobalConfig &globalConfig = windowContext->getGlobalConfig();

    switch (instrumentStatus->GetWorkStatus()) {
        case InstrumentStatus::NORMAL:
        case InstrumentStatus::RESERVATION:
            if (newUser.card.type == biotech::osla::CARD_TYPE_ADMIN) {
                //tr("实验开始 (仪器管理员代刷)");
                info = tr("The experiment began \n(The instrument manager brushes for you)");
            } else if ((windowContext->isPaymentAaccountEnabled()) && (newUser.account.deposit < globalConfig.account_deposit_limit() * 100)) { // 对账户余额低于门限的用户给出提示
                //tr("实验开始 (付款账户余额已不足 %1 元)")
                info = tr("The experiment began \n(The balance of the payment account is less than %1)").arg(globalConfig.account_deposit_limit());

            } else if ((windowContext->isPaymentAaccountEnabled()) && (newUser.account.deposit < 0)) {
                //info = tr("实验开始 (付款账户已欠费)");
                //tr("实验开始 (付款账户余额:%1元)")
                info = tr("The experiment began \n(The balance of the payment account is less than %1)").arg(newUser.account.deposit / 100);
            } else {
                //info = tr("实验开始");
                info = tr("The experiment began");
            }

            PROMPTINFO(parent(), info, "media/start.mp3");
            break;

        case InstrumentStatus::TRAINING:
            PROMPTINFO(parent(), tr("Start Training"), "media/starttraining.mp3");
            break;

        case InstrumentStatus::EDUCATION:
            PROMPTINFO(parent(), tr("Start Education"), "media/StartEducation.mp3");
            break;

        case InstrumentStatus::ENTRUST:
            PROMPTINFO(parent(), tr("Start Entrust"), "media/EntrustBegin.mp3");
            break;

        case InstrumentStatus::MAINTENANCE:
            PROMPTINFO(parent(), tr("Start Maintenance"), "media/startmaintaining.mp3");
            break;
    }
}

void SessionManage::PromptEndSession() {
    UserInformation *userInformation = windowContext->getUserInformation();
    uint &timeElapsed = userInformation->GetDeviceTimeElapsed();
    QString info;
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    switch (instrumentStatus->GetWorkStatus()) {
        case InstrumentStatus::NORMAL:
        case InstrumentStatus::RESERVATION:
            info = tr("Experiment is over, time-consuming: %1").arg(NTG()->FormatTimeString(timeElapsed));
            PROMPTINFO(parent(), info, "media/end.mp3");
            break;

        case InstrumentStatus::TRAINING:
            info = tr("Training is over, time-consuming: %1").arg(NTG()->FormatTimeString(timeElapsed));
            PROMPTINFO(parent(), info, "media/endtraining.mp3");
            break;

        case InstrumentStatus::MAINTENANCE:
            info = tr("Maintaining is over, time-consuming: %1").arg(NTG()->FormatTimeString(timeElapsed));
            PROMPTINFO(parent(), info, "media/endmaintaining.mp3");
            break;

        case InstrumentStatus::EDUCATION:
            info = tr("Education is over, time-consuming: %1").arg(NTG()->FormatTimeString(timeElapsed));
            PROMPTINFO(parent(), info, "media/EndEducation.mp3");
            break;

        case InstrumentStatus::ENTRUST:
            info = tr("Entrust is over, time-consuming: %1").arg(NTG()->FormatTimeString(timeElapsed));
            PROMPTINFO(parent(), info, "media/EntrustEnd.mp3");
            break;
    }
}

int SessionManage::UpdateSessionStatus() {
    UserInformation *userInformation = windowContext->getUserInformation();
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    uint &timeElapsed = userInformation->GetDeviceTimeElapsed();

    // 使用时间到达提醒线
    // 请注意, 您已使用仪器超过 %1
    if (false == windowContext->isUseAlertTime() && instrumentStatus->GetAlertTime() != 0 && timeElapsed > (uint) instrumentStatus->GetAlertTime() * 60) {
        //"请注意, 您已使用仪器超过 %1")
        PROMPTINFO(parent(), tr("Please note that you have used more than %1od the instrument").arg(
                NTG()->FormatTimeString((uint) instrumentStatus->GetAlertTime() * 60)),
                   "media/UseInstrumentTimeOut.mp3", true, 500);
        User &currentUser = userInformation->GetCurrentUser();
        DEBUG_LOG("UserID:" << currentUser.id << ",CreateTime:" << currentUser.timestamp);
        SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
        quint64 userEventId = sqlWorker->getUserEventByUser(currentUser);
        ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_EXCEPTIONAL_USING_TIME, userEventId);
        windowContext->setUseAlertTime(true);
    }

    // 每分钟更新一次当前使用记录的结束时间，避免出现记录不完整
    if (timeElapsed % 30 == 0) {
        userInformation->UpdateSessionEndTime();
    }
    return timeElapsed++;
}
