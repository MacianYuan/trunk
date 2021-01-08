
#include <QtGui/QMessageBox>

#include <deviceinfo/enum.h>
#include <sqlworker/sqlworker.h>
#include <tools/log.h>

#include "controller/Context/AdminAction.h"
#include "controller/Base/NormalTools.h"
#include "controller/UI/TableViewDialog.h"
#include "controller/UI/EntrustBeginDialog.h"
#include "controller/UI/EntrustEndDialog.h"
#include "controller/Context/InstrumentStatus.h"
#include "controller/Context/UserInformation.h"
#include "controller/Context/ServiceEvent.h"
#include "controller/UI/SettingsDialog.h"

AdminAction::AdminAction(QObject *parent) : QObject(parent) {
}

void AdminAction::Init(WindowContext *c, SessionManage *s) {
    this->windowContext = c;
    this->sessionManage = s;
}

bool AdminAction::AdminActionStartEducation() {
    bool status = false;

    do {
        InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
        if (true == instrumentStatus->GetInUseStatus() && true == IsContinueUseInstrument()) {
            break;
        }
        UserInformation *userInformation = windowContext->getUserInformation();
        User &newUser = userInformation->GetNewUser();

        if (false == instrumentStatus->InstrumentStatusCheck(newUser)) {
            break;
        }

        SessionNode node = {biotech::osla::SESSION_TYPE_EDUCATION,
                            InstrumentStatus::EDUCATION, biotech::osla::USER_ACTION_START_EDUCATION};

        if (false == sessionManage->StartSession(node)) {
            break;
        }

        NTG()->SetDeviceWorkStatusLed(true);
        status = true;
    } while (false);

    return status;
}

bool AdminAction::AdminActionStartTraning() {
    bool status = false;

    do {
        InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
        if (true == instrumentStatus->GetInUseStatus() && true == IsContinueUseInstrument()) {
            break;
        }

        UserInformation *userInformation = windowContext->getUserInformation();
        User &newUser = userInformation->GetNewUser();

        if (false == instrumentStatus->InstrumentStatusCheck(newUser)) {
            break;
        }

        SessionNode node = {biotech::osla::SESSION_TYPE_TRAINING,
                            InstrumentStatus::TRAINING,
                            biotech::osla::USER_ACTION_START_TRAINING};

        if (false == sessionManage->StartSession(node)) {
            break;
        }

        NTG()->SetDeviceWorkStatusLed(true);
        status = true;
    } while (false);

    return status;
}

bool AdminAction::AdminActionStartMaintenance() {
    bool status = false;

    do {
        InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
        if (true == instrumentStatus->GetInUseStatus() && true == IsContinueUseInstrument()) {
            break;
        }

        /// 注意仪器故障时也可以维护开机
        UserInformation *userInformation = windowContext->getUserInformation();
        User &newUser = userInformation->GetNewUser();
        if (false == instrumentStatus->InstrumentStatusCheckExpectFaulty(newUser)) {
            break;
        }

        SessionNode node = {biotech::osla::SESSION_TYPE_MAINTENANCE,
                            InstrumentStatus::MAINTENANCE,
                            biotech::osla::USER_ACTION_START_MAINTENANCE};

        if (false == sessionManage->StartSession(node)) {
            break;
        }

        NTG()->SetDeviceWorkStatusLed(true);
        status = true;
    } while (false);

    return status;
}

bool AdminAction::AdminActionStartAssistance() {
    bool status = false;

    do {
        // 如果仪器在使用中，给出提醒
        InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
        if (true == instrumentStatus->GetInUseStatus() && true == IsContinueUseInstrument()) {
            break;
        }

        /// 检查仪器状态
        UserInformation *userInformation = windowContext->getUserInformation();
        User &newUser = userInformation->GetNewUser();
        if (false == instrumentStatus->InstrumentStatusCheck(newUser)) {
            break;
        }

        SessionNode node = {biotech::osla::SESSION_TYPE_ASSISTANCE,
                            InstrumentStatus::NORMAL,
                            biotech::osla::USER_ACTION_START_ASSISTANCE};

        if (false == sessionManage->StartSession(node)) {
            break;
        }

        NTG()->SetDeviceWorkStatusLed(true);
        status = true;
    } while (false);

    return status;
}

bool AdminAction::AdminActionEndSession() {
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    if (false == instrumentStatus->GetInUseStatus()) {
        return false;
    }

    /// 更新使用记录的完成标志
    UserInformation *userInformation = windowContext->getUserInformation();
    User &currentUser = userInformation->GetCurrentUser();
    biotech::osla::Session &session = userInformation->GetSessionInfo();

    if (session.type() == biotech::osla::SESSION_TYPE_ENTRUST) {
        EntrustEndDialog *dialog = new EntrustEndDialog((QWidget *) parent());
        dialog->init(windowContext, sessionManage);
        dialog->showFullScreen();
        DEBUG_LOG("");
    } else {
        sessionManage->EndSession();
    }
    DEBUG_LOG("");

    return true;
}

bool AdminAction::AdminActionConfigSystem() {
    DEBUG_LOG("");
    SettingsDialog *settingsDialog = windowContext->getSettingsDialog();
    settingsDialog->showFullScreen();
    return true;
}

bool AdminAction::AdminActionViewRecords() {
    DEBUG_LOG("");
    TableViewDialog *tableViewDialog = new TableViewDialog((QWidget *) parent());
    tableViewDialog->showFullScreen();
    return true;
}

bool AdminAction::AdminActionEntrust() {
    DEBUG_LOG("");
    EntrustBeginDialog *dialog = new EntrustBeginDialog((QWidget *) parent());
    dialog->init(windowContext, sessionManage);
    dialog->showFullScreen();
    return true;
}

bool AdminAction::AdminActionDefault() {
    DEBUG_LOG("");
    UserInformation *userInformation = windowContext->getUserInformation();
    User &newUser = userInformation->GetNewUser();
    ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_NULL_OP,
                                              biotech::osla::USER_ACTION_NULL_RESULT);
    return true;
}

bool AdminAction::IsContinueUseInstrument() {
    DEBUG_LOG("");
    UserInformation *userInformation = windowContext->getUserInformation();
    User &newUser = userInformation->GetNewUser();
    //注意.   仪器正在使用中，是否结束当前实验？
    QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                              tr("The instrument is in use.Is the current experiment over."),
                                              QMessageBox::Yes | QMessageBox::No, (QWidget *) (parent()));
    messageBox->setDefaultButton(QMessageBox::No);
    messageBox->setAttribute(Qt::WA_DeleteOnClose);

    if (messageBox->exec() != QMessageBox::Yes) { // 如果不继续，则直接退出
        ServiceEvent::GetInstance()->AddUserEvent(newUser,
                                                  biotech::osla::USER_ACTION_NULL_OP,
                                                  biotech::osla::USER_ACTION_NULL_RESULT);
        return true;
    }

    // 如果继续，先结束当前使用
    User &currentUser = userInformation->GetCurrentUser();
    biotech::osla::Session &session = userInformation->GetSessionInfo();
    if (session.type() == biotech::osla::SESSION_TYPE_ENTRUST) {
        EntrustEndDialog *dialog = new EntrustEndDialog((QWidget *) parent());
        dialog->init(windowContext, sessionManage);
        dialog->showFullScreen();
        dialog->exec();
        DEBUG_LOG("");
    } else {
        sessionManage->EndSession();
    }

    return false;
}

bool AdminAction::AdminCardSwiping() {
    DEBUG_LOG("");
    // 其他管理员正在使用仪器，给出提醒
    UserInformation *userInformation = windowContext->getUserInformation();
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    User &newUser = userInformation->GetNewUser();
    User &currentUser = userInformation->GetCurrentUser();

    if (true == instrumentStatus->GetInUseStatus() && true == userInformation->CurrentUserIsAdmin() &&
        currentUser.id != newUser.id) {
        //注意  仪器管理员%1正在使用本仪器，是否继续？
        QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                                  tr("The instrument manager(%1)is using the instrument?").arg(currentUser.fullName),
                                                  QMessageBox::Yes | QMessageBox::No, (QWidget *) parent());
        messageBox->setDefaultButton(QMessageBox::No);
        messageBox->setAttribute(Qt::WA_DeleteOnClose);

        if (messageBox->exec() != QMessageBox::Yes) {
            return false;
        }
    }

    AdminDialog *adminDialog = windowContext->getAdminDialog();
    adminDialog->setWindowTitle(tr("The instrument manager: %1").arg(newUser.fullName));
    adminDialog->setUsing(instrumentStatus->GetInUseStatus());
    adminDialog->open();
    return true;
}

bool AdminAction::TypeCardSwiping(int type) {
    DEBUG_LOG("");

    ///  检测签到有效期 ------ 检查之前是否签到过
    UserInformation *userInformation = windowContext->getUserInformation();
    SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    const biotech::osla::GlobalConfig &globalConfig = windowContext->getGlobalConfig();

    User &newUser = userInformation->GetNewUser();
    biotech::osla::Session session = userInformation->GetSessionInfo();

    uint card = 0;
    sscanf(newUser.card.serial.c_str(), "%u", &card);
    if (!sqlWorker->hasCheckinRecord(session.id(), card)) {
        if (true == userInformation->AddTraningUser(session.id())) {
            //签到成功 未知用户
            QString info = tr("%1 Sign in successfully").arg(
                    (newUser.id == biotech::osla::UNKNOWN_USER_ID) ? tr("Unknown user") : newUser.fullName);
            PROMPTINFO(parent(), info, "media/checkin.mp3", true, 500);
            ServiceEvent::GetInstance()->AddUserEvent(newUser, type, biotech::osla::USER_ACTION_SUCCEED);
            emit ShowWorkStatus(userInformation->GetCountInTraining());
        }
    } else {
        PROMPTINFO(parent(), tr("You have signed in"), "media/checkinalready.mp3", true, 500);
        ServiceEvent::GetInstance()->AddUserEvent(newUser, type,
                                                  biotech::osla::USER_ACTION_FAILED_CHECKIN_ITERATIVELY);
    }

    return true;
}

bool AdminAction::EducationCardSwiping() {
    DEBUG_LOG("");
    return TypeCardSwiping(biotech::osla::USER_ACTION_EDUCATION_CHECKIN);
}


bool AdminAction::TrainingCardSwiping() {
    DEBUG_LOG("");

    return TypeCardSwiping(biotech::osla::USER_ACTION_TRAINING_CHECKIN);
}

bool AdminAction::MaintenanceCardSwiping() {
    DEBUG_LOG("");
    UserInformation *userInformation = windowContext->getUserInformation();
    User &newUser = userInformation->GetNewUser();
    PROMPTINFO(parent(), tr("Instrument maintenance"), "media/maintaining.mp3", true, 500);//仪器维护中
    ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_START_SESSION,
                                              biotech::osla::USER_ACTION_FAILED_INSTRUMENT_INUSE);
    return true;
}

bool AdminAction::doAdminActionInfo() {
    struct _TempNode_ {
        bool status;
        std::function<bool()> fun;
    };

    UserInformation *userInformation = windowContext->getUserInformation();
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();

    _TempNode_ arr_1[] = {
            { /// 管理员刷卡，特殊处理
                    true == userInformation->NewUserIsAdmin(),
                    std::bind(&AdminAction::AdminCardSwiping, this)
            },
            { /// 仪器处于培训状态
                    InstrumentStatus::TRAINING == instrumentStatus->GetWorkStatus(),
                    std::bind(&AdminAction::TrainingCardSwiping, this)
            },
            { /// 仪器处于教学状态
                    InstrumentStatus::EDUCATION == instrumentStatus->GetWorkStatus(),
                    std::bind(&AdminAction::EducationCardSwiping, this)

            },
            { /// 仪器处于维护状态
                    InstrumentStatus::MAINTENANCE == instrumentStatus->GetWorkStatus(),
                    std::bind(&AdminAction::MaintenanceCardSwiping, this)
            },
    };

    for (int i = 0; i < (int) (sizeof(arr_1) / sizeof(arr_1[0])); ++i) {
        if (true == arr_1[i].status) {
            arr_1[i].fun();
            return true;
        }
    }

    return false;
}



