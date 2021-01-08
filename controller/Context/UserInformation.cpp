
#include "controller/Context/UserInformation.h"
#include <QtCore/QDateTime>
#include <controller/Base/GlobalData.h>
#include <deviceinfo/enum.h>
#include <tools/log.h>
#include <controller/Base/NormalTools.h>
#include "sqlworker/sqlworker.h"
#include "controller/Context/ServiceEvent.h"
#include <deviceinfo/version.h>
#include "tools/IniParse.h"

static void UserClear(User &user) {
    user.id = 0;
    user.status = 0;
    user.firstName.clear();
    user.lastName.clear();
    user.fullName.clear();
    user.phone.clear();
    user.isInBlacklist = false;
    user.right = 0;
    user.longTimeRight = 0;
    user.rightExpireTime = 0;
    user.account.credit = 0;
    user.account.deposit = 0;
    user.account.groupId = 0;
    user.account.id = 0;
    user.account.status = 0;
    user.card.serial = "";
    user.card.status = 0;
    user.card.type = 0;
    user.timestamp = 0;
}

static void UserCpy(User &from, User &to) {
    to.id = from.id;
    to.status = from.status;
    to.firstName = from.firstName;
    to.lastName = from.lastName;
    to.fullName = from.fullName;
    to.phone = from.phone;
    to.isInBlacklist = from.isInBlacklist;
    to.right = from.right;
    to.longTimeRight = from.longTimeRight;
    to.rightExpireTime = from.rightExpireTime;
    memcpy(&to.account, &from.account, sizeof(to.account));
    to.card.serial = from.card.serial;
    to.card.status = from.card.status;
    to.card.type = from.card.type;
    //memcpy(&to.card, &from.card, sizeof(to.card));
    to.timestamp = from.timestamp;
}

UserInformation::UserInformation(QObject *parent) : QObject(parent) {
    UserClear(m_NewUser);
    UserClear(m_CurrentUser);
    session.Clear();

    updateSettings();
}

UserInformation::~UserInformation() {

}

void UserInformation::updateSettings()
{
    IniParse osla(OSLA_CONFIG);
    userQualification.readUserQualification(osla);
}

bool UserInformation::InitFromDB() {
    do {
        SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
        session = m_SqlWork->getLastSessionUncompleted();
        if (0 == session.id()) {
            break;
        }
        /// 如果有会话未完成，说明在启动之前，是有用户在使用的
        DEBUG_LOG("go before new, because of restart");
        m_CurrentUser = m_SqlWork->getUserInfoById(session.user_id());
        DEBUG_LOG("restart, before use id is : " << m_CurrentUser.id);
        uint currentDateTime_t = QDateTime::currentDateTime().toTime_t();
        timeElapsed = currentDateTime_t - session.start_time(); // 计算已经使用的时间
        session.set_end_time(currentDateTime_t); // 更新使用记录的结束时间为当前时间
        m_SqlWork->addSession(session, false); // 使用记录更新写入数据库
        if (session.type() == biotech::osla::SESSION_TYPE_TRAINING ||
            session.type() == biotech::osla::SESSION_TYPE_EDUCATION) {
            m_CountUserInTraining = m_SqlWork->userInTrainingCount(session.id());
        }
    } while (false);

    return true;
}

bool UserInformation::GetNewUserInfo(uint cardSerial) {
    /// 从数据库获取该卡对应的用户信息
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    m_NewUser = m_SqlWork->getUserInfoByCardSerial(cardSerial);
//    if (0 == m_NewUser.id) { // 数据库查询错误
//        UserClear(m_NewUser);
//        //PROMPTINFO(parent(), tr("数据库查询错误"), "", true, 500);
//        //setErrorInfo(tr("数据库查询错误"));
//        //DEBUG_LOG("MainWindow: Database query error");
//        //return false;
//    }
    m_SqlWork->getUserIsInPenaltyStop(m_NewUser, time(NULL));
    DEBUG_LOG("MainWindow: User" << m_NewUser.id << "found, user is in penalty stop : "
                                 << (false == m_NewUser.isInBlacklist ? "false" : "true"));
    return true;
}

bool UserInformation::GetNewSessionFromNewUser() {
    session.set_id(0);
    session.set_start_time(m_NewUser.timestamp);
    session.set_end_time(m_NewUser.timestamp); // session初始结束时间=开始时间，之后定期更新
    session.set_user_id(m_NewUser.id);
    session.set_card_serial(m_NewUser.card.serial);
    session.set_user_right(m_NewUser.right);
    session.set_account_id(m_NewUser.account.id);
    session.set_group_id(m_NewUser.account.groupId);
    session.set_penalty_flag(false);
    session.set_long_time_right(m_NewUser.longTimeRight);

    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    if (false == m_SqlWork->addSession(session, false)) {
        PROMPTINFO(parent(), tr("Database write error"), "", true, 500);//数据库写入错误
        ///setErrorInfo(tr("数据库写入错误"));
        session.Clear();
        return false;
    }
    DEBUG_LOG("New Session ID:" << m_SqlWork->formatIdString((quint64) session.id()).toLocal8Bit().data() << "started");

    return true;
}

User &UserInformation::GetNewUser() {
    return m_NewUser;
}

User &UserInformation::GetCurrentUser() {
    return m_CurrentUser;
}

void UserInformation::StartSession() {
    UserClear(m_CurrentUser);
    UserCpy(m_NewUser, m_CurrentUser);
    UserClear(m_NewUser);
}

void UserInformation::EndSession() {
    UserClear(m_CurrentUser);
    m_CountUserInTraining = 0;
    session.Clear();
    timeElapsed = 0;
}

bool UserInformation::AddTraningUser(qint64 sessionId) {
    biotech::osla::UserInTraining userInTraining;
    userInTraining.set_session_id(sessionId);
    userInTraining.set_card_serial(m_NewUser.card.serial);
    userInTraining.set_user_id(m_NewUser.id);
    userInTraining.set_check_in_time(m_NewUser.timestamp);
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    if (false == m_SqlWork->addUserInTraining(userInTraining)) {
        return false;
    }
    m_CountUserInTraining++;
    return true;
}

int UserInformation::GetCountInTraining() {
    return m_CountUserInTraining;
}

void UserInformation::SetCountInTraining(int num) {
    m_CountUserInTraining = num;
}

void UserInformation::ClearCountInTraining() {
    m_CountUserInTraining = 0;
}

bool UserInformation::CurrentUserIsNewUser() {
    return (m_NewUser.card.serial == m_CurrentUser.card.serial);
}

bool UserInformation::CurrentUserIsAdmin() {
    return m_CurrentUser.card.type == biotech::osla::CARD_TYPE_ADMIN;
}

bool UserInformation::NewUserIsAdmin() {
    return m_NewUser.card.type == biotech::osla::CARD_TYPE_ADMIN;
}

bool UserInformation::NewUserIsAdmin(uint userId) {
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    User user = m_SqlWork->getUserInfoById(userId);
    return user.card.type == biotech::osla::CARD_TYPE_ADMIN;
}

bool UserInformation::UserQualificationCheck()
{
    ConditionNode arr[] = {
        {
            ((m_NewUser.right == biotech::osla::USER_RIGHT_UNAUTH)
               && (NTG()->IsWorkingTime(m_NewUser.timestamp))
               && !userQualification.getUnauthorizedUserInWorkTime()),
            tr("Inconsistent user qualification: unauthorized user"),//用户资格不符: 未授权用户
            "media/unauthorized.mp3", biotech::osla::USER_ACTION_FAILED_INSUFFICIENT_RIGHT
        },
        {
            ((m_NewUser.right == biotech::osla::USER_RIGHT_UNAUTH)
               && !(NTG()->IsWorkingTime(m_NewUser.timestamp))
               && !userQualification.getUnauthorizedUserNotInWorkTime()),
            tr("Inconsistent user qualification: unauthorized user"),//用户资格不符: 未授权用户
            "media/unauthorized.mp3", biotech::osla::USER_ACTION_FAILED_INSUFFICIENT_RIGHT
        },
        {
            ((m_NewUser.right == biotech::osla::USER_RIGHT_NORMAL)
               && (NTG()->IsWorkingTime(m_NewUser.timestamp))
               && !userQualification.getNormalUserInWorkTime()),
            tr("Inconsistent user qualification: unauthorized user"),//用户资格不符: 未授权用户
            "media/unauthorized.mp3", biotech::osla::USER_ACTION_FAILED_INSUFFICIENT_RIGHT
        },
        {
            ((m_NewUser.right == biotech::osla::USER_RIGHT_NORMAL)
               && !(NTG()->IsWorkingTime(m_NewUser.timestamp))
               && !userQualification.getNormalUserNotInWorkTime()),
            tr("Inconsistent user qualification: unauthorized user"),//用户资格不符: 未授权用户
            "media/unauthorized.mp3", biotech::osla::USER_ACTION_FAILED_INSUFFICIENT_RIGHT
        },
        {
            ((m_NewUser.right == biotech::osla::USER_RIGHT_SENIOR)
               && (NTG()->IsWorkingTime(m_NewUser.timestamp))
               && !userQualification.getSeniorUserInWorkTime()),
            tr("Inconsistent user qualification: unauthorized user"),//用户资格不符: 未授权用户
            "media/unauthorized.mp3", biotech::osla::USER_ACTION_FAILED_INSUFFICIENT_RIGHT
        },
        {
            ((m_NewUser.right == biotech::osla::USER_RIGHT_SENIOR)
               && !(NTG()->IsWorkingTime(m_NewUser.timestamp))
               && !userQualification.getSeniorUserNotInWorkTime()),
            tr("Inconsistent user qualification: unauthorized user"),//用户资格不符: 未授权用户
            "media/unauthorized.mp3", biotech::osla::USER_ACTION_FAILED_INSUFFICIENT_RIGHT
        }
    };

    for (int i = 0; i < (int) (sizeof(arr) / sizeof(arr[0])); ++i) {
        if (true == arr[i].status) {
            PROMPTINFO(parent(), arr[i].info, arr[i].soundPath, true, 500);
            ServiceEvent::GetInstance()->AddUserEvent(m_NewUser, biotech::osla::USER_ACTION_START_SESSION,
                                                      arr[i].errorCode);
            return false;
        }
    }

    return true;
}

//bool UserInformation::UserQualificationCheck() {
//    ConditionNode arr[] = {
//            {
//                    (m_NewUser.right == biotech::osla::USER_RIGHT_UNAUTH),
//                    (m_NewUser.rightExpireTime >= m_NewUser.timestamp) ?
//                    tr("用户资格不符: 未授权用户 (有效期至%1)").arg(
//                            QDateTime::fromTime_t(m_NewUser.rightExpireTime).toString("yyyy/M/d hh:mm")) :
//                    tr("用户资格不符: 未授权用户"),
//                    "media/unauthorized.mp3", biotech::osla::USER_ACTION_FAILED_INSUFFICIENT_RIGHT
//            },

//            {
//                    (m_NewUser.right == biotech::osla::USER_RIGHT_NORMAL && !NTG()->IsWorkingTime(m_NewUser.timestamp)),
//                    (m_NewUser.rightExpireTime >= m_NewUser.timestamp) ?
//                    tr("用户资格不符: 普通用户 (有效期至%1)").arg(
//                            QDateTime::fromTime_t(m_NewUser.rightExpireTime).toString("yyyy/M/d hh:mm")) :
//                    tr("用户资格不符: 非工作时间"),
//                    "media/unauthorized.mp3", biotech::osla::USER_ACTION_FAILED_INSUFFICIENT_RIGHT
//            }
//    };

//    for (int i = 0; i < (int) (sizeof(arr) / sizeof(arr[0])); ++i) {
//        if (true == arr[i].status) {
//            PROMPTINFO(parent(), arr[i].info, arr[i].soundPath, true, 500);
//            ServiceEvent::GetInstance()->AddUserEvent(m_NewUser, biotech::osla::USER_ACTION_START_SESSION,
//                                                      arr[i].errorCode);
//            return false;
//        }
//    }

//    return true;
//}

bool UserInformation::NewUserCardCheck() {
    ConditionNode arr[] = {
        //未知用户
            {(m_NewUser.id == biotech::osla::UNKNOWN_USER_ID),
                    tr("Unknown user"),     "media/unidentified.mp3", biotech::osla::USER_ACTION_FAILED_UNKNOWN_USER},
        //用户已停用或作废
            {(m_NewUser.status != biotech::osla::USER_STATUS_ACTIVE),
                    tr("The user has been deactivatrd or invalidated"), "media/invaliduser.mp3",  biotech::osla::USER_ACTION_FAILED_UNKNOWN_USER},
            {(m_NewUser.card.type == biotech::osla::CARD_TYPE_TEMPORARY &&
              (m_NewUser.card.status == 1 || m_NewUser.card.status == 2)),
             //临时卡无效
                    tr("Temporary card invalid"),    "media/invalidcard.mp3",  biotech::osla::USER_ACTION_FAILED_INVALID_TEMP_CARD}
    };

    for (int i = 0; i < (int) (sizeof(arr) / sizeof(arr[0])); ++i) {
        if (true == arr[i].status) {
            PROMPTINFO(parent(), arr[i].info, arr[i].soundPath, true, 500);
            ServiceEvent::GetInstance()->AddUserEvent(m_NewUser, biotech::osla::USER_ACTION_START_SESSION,
                                                      arr[i].errorCode);
            return false;
        }
    }

    return true;
}

bool UserInformation::NewUserAccountCheck(bool aIsPaymentAccountEnabled, bool denyDeposit) {
    ConditionNode arr[] = {
            {(aIsPaymentAccountEnabled && (m_NewUser.account.id == 0)),
             //无付款账户
                    tr("No payment account"),       "media/accountmissing.mp3",   biotech::osla::USER_ACTION_FAILED_ACCOUNT_MISSING},
            {(aIsPaymentAccountEnabled && (m_NewUser.account.status != biotech::osla::ACCOUNT_STATUS_ACTIVE)),
             //付款账户已冻结或作废
                    tr("The payment account has been frozen or invalidated"),  "media/invalidaccount.mp3",   biotech::osla::USER_ACTION_FAILED_INVALID_ACCOUNT},
            {(aIsPaymentAccountEnabled &&
             ((m_NewUser.account.deposit + m_NewUser.account.credit <= 0) && denyDeposit)),
             //付款账户已超支，请充值
                    tr("The payment account has overspent,please recharge it"), "media/accountoverdrawn.mp3", biotech::osla::USER_ACTION_FAILED_INSUFFICIENT_BALANCE},
            {(true == m_NewUser.isInPenaltyStop),
             //用户处于罚停期
                    tr("The user is in the penalty suspension period(%1-%2)")
                            .arg(QDateTime::fromTime_t(m_NewUser.inPenaltyStopStart).toString("yyyy.MM.dd hh:mm"))
                            .arg(QDateTime::fromTime_t(m_NewUser.inPenaltyStopEnd).toString("yyyy.MM.dd hh:mm")),
                                       "media/InPenaltyStop.mp3",    biotech::osla::USER_ACTION_FAILED_PENALTYSTOP},
    };

    for (int i = 0; i < (int) (sizeof(arr) / sizeof(arr[0])); ++i) {
        if (true == arr[i].status) {
            PROMPTINFO(parent(), arr[i].info, arr[i].soundPath, true, 500);
            ServiceEvent::GetInstance()->AddUserEvent(m_NewUser, biotech::osla::USER_ACTION_START_SESSION,
                                                      arr[i].errorCode);
            return false;
        }
    }

    return true;
}

biotech::osla::Session &UserInformation::GetSessionInfo() {
    return session;
}

bool UserInformation::UpdateSessionEndTime() {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    uint currentDateTime_t = currentDateTime.toTime_t();
    session.set_end_time(currentDateTime_t);
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    m_SqlWork->addSession(session, false);

    return true;
}

uint &UserInformation::GetDeviceTimeElapsed() {
    return timeElapsed;
}

