#include <functional>

#include <QtGui/QMessageBox>
#include <QtGui/QWSServer>
#include <QtCore/QTimer>
#include <QtGui/QStandardItem>

#include <deviceinfo/enum.h>
#include <deviceinfo/version.h>
#include <deviceinfo/tq2440.h>
#include <sqlworker/sqlworker.h>
#include <tools/IniParse.h>
#include <tools/common.h>
#include <tools/log.h>
#include <protobuf/message.pb.h>
#include <3rdparty/qxt/qxtnamespace.h>
#include <tools/WatchDog.h>

#include "controller/UI/MainWindow.h"
#include "controller/UI/ScheduleDialog.h"
#include "controller/Context/PowerDelay.h"
#include "controller/Context/InstrumentStatus.h"
#include "controller/Context/ServiceEvent.h"
#include "controller/Context/UserInformation.h"
#include "controller/Context/BookInformation.h"
#include "controller/Context/ReservationInformation.h"
#include "controller/Context/SessionManage.h"
#include "controller/Base/GlobalData.h"
#include "controller/Base/NormalTools.h"
#include "ui_MainWindow.h"
#include "controller/UI/SettingsDialog.h"
#include "PublicDefine.h"
#include "upgrade/qupgradeplan.h"
#include <QTime>
#include <QDebug>


#define unlikely(x) __builtin_expect((x),0)

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->warningLabel->setVisible(false);
    ui->timerLCD->display("0000:00:00");

    IniParse osla(OSLA_CONFIG);
    GlobalData::currentLocale = osla.GetString("Global.Language").c_str();
    if (GlobalData::currentLocale.isEmpty()) {
        GlobalData::currentLocale = "en_US";
    }
    if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0){
        ui->label->setStyleSheet(QString::fromUtf8("background-image: url(:/images/label_zh.png);"));
    }
    else{
        ui->label->setStyleSheet(QString::fromUtf8("background-image: url(:/images/label_en.png);"));
    }
    ui->label->show();

//    //二维码地址
//    qrWidget = new QWidget(this);
//    imageView = new ImageView(qrWidget);
//    imageView->setGeometry(550,0,90,90);
//    imageView->setLoadingPixmap(QPixmap("://images/btn_reset.png"));
//    imageView->setFailedPixmap(QPixmap("://images/btn_cancel.png"));

//    QString url =  osla.GetString("Instrument.TerImgUrl").c_str();
//    if(!url.isEmpty()){
//        imageView->setUrl(url);
//    }

    //二维码地址
    qrWidget = new QWidget(this);
    imageView = new ImageView(qrWidget);

    imageView->setGeometry(550,0,90,90);
    qrWidget->show();

    /// 启动的时候，让指示灯点亮，此时网络还没有连接
    NTG()->SetPilotLamp(true);
    //sqlWorker = new SqlWorker(nullptr, "MainWindow");
    //SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();

    windowContext = new WindowContext(this);
    windowContext->init();
    sessionManage = new SessionManage(this);
    sessionManage->Init(windowContext);
    connect(sessionManage, SIGNAL(ShowCurrentUserInfo()), this, SLOT(ShowCurrentUserInfo()));
    connect(sessionManage, SIGNAL(SetCurrentStateInfo()), this, SLOT(SetCurrentStateInfo()));

    adminAction = new AdminAction(this);
    adminAction->Init(windowContext, sessionManage);

    ///-------------------------------------------------------------------------------------------------------

    connect(ui->moreBookingsButton, SIGNAL(clicked()), this, SLOT(showBookingSchedule()));
    AdminDialog *adminDialog = windowContext->getAdminDialog();
    connect(adminDialog, SIGNAL(finished(int)), this, SLOT(doAdminAction(int)));

    SettingsDialog *settingsDialog = windowContext->getSettingsDialog();
    connect(settingsDialog, SIGNAL(rejected()), this, SLOT(onSettingsDialogClosed()));
    connect(settingsDialog, SIGNAL(instrumentStatusUpdated(int)), this, SLOT(onInstrumentStatusUpdated(int)));
    connect(settingsDialog, SIGNAL(swapCardIntervalChanged(int)), this, SLOT(OnSwapCardIntervalChanged(int)));
    connect(settingsDialog, SIGNAL(reloadSetting()), this, SLOT(readSettings()));
    connect(settingsDialog, SIGNAL(networkSettingsChanged()), this, SLOT(onNetworkSettingsChanged()));
    //connect(settingsDialog, SIGNAL(serverUpdated()), this, SIGNAL(restartSync()));
    connect(settingsDialog, SIGNAL(requestUnbinding()), windowContext, SLOT(RequestUnbinding()));
    //connect(settingsDialog, SIGNAL(requestExportData()), this, SLOT(onExportData()));
    connect(settingsDialog, SIGNAL(FocusUnBinding()), this, SLOT(FocusUnbinding()));

    //SqlWorker *sqlWorker = windowContext->getSqlWorker();
    //connect(sqlWorker, SIGNAL(error(QString)), this, SLOT(setErrorInfo(QString)));

    CardReader *cardReader = windowContext->getCardReader();
    connect(cardReader, SIGNAL(cardDetected(uint)), this, SLOT(onCardDetected(uint)));

    BookInformation *bookInformation = windowContext->getBookInformation();
    bookInformation->InitLableInfo(ui->currentBookingCountDownLabel, ui->currentBookingTimeLabel,
                                   ui->currentBookingUserLabel, ui->moreBookingsButton);

    ReservationInformation *reservationInformation = windowContext->getReservationInformation();
    connect(reservationInformation, SIGNAL(SetCurrentStateInfo()), this, SLOT(SetCurrentStateInfo()));


    cardTiming = new QTime();
    regularTaskTimer = new QTimer(this);
    connect(regularTaskTimer, SIGNAL(timeout()), this, SLOT(doRegularTask()));
    NTG()->TurnOffLeds();

    connect(adminAction, SIGNAL(ShowWorkStatus(int)), this, SLOT(onShowWorkStatus(int)));

    connect(this, SIGNAL(qrImageUrl_update()), this, SLOT(on_qrImageUrl_update()));
}

MainWindow::~MainWindow() {
    delete cardTiming;
    delete regularTaskTimer;
    NTG()->TurnOffLeds();
    delete ui;
}

void MainWindow::on_qrImageUrl_update()
{
    //二维码地址
    static  QString url_old;


    IniParse osla(OSLA_CONFIG);
    QString url =  osla.GetString("Instrument.TerImgUrl").c_str();

    qDebug()<< "Instrument.TerImgUrl:1 : " << url_old;

    if(url.compare(url_old) == 0)
        return;

    url_old = url;

    qDebug()<< "Instrument.TerImgUrl:2 : " << url;

    DEBUG_LOG("Instrument.TerImgUrl : : " << url.toStdString());



    //imageView->setGeometry(550,0,90,90);
    //imageView->setLoadingPixmap(QPixmap("://images/btn_reset.png"));
    //imageView->setFailedPixmap(QPixmap("://images/btn_cancel.png"));

    if(!url.isEmpty()){
         imageView->setUrl(url);
    }
    imageView->show();
    qrWidget->show();

}

void MainWindow::changeTr(int language_label)
{
    QString qmFilename;
    static QTranslator* translator;
    if (translator != NULL)
    {
        qApp->removeTranslator(translator);
        delete translator;
        translator = NULL;
    }
    translator = new QTranslator;

    QString runPath = QCoreApplication::applicationDirPath();
    //获取文件运行路径

    if(language_label==1)
    {
        qmFilename = runPath + "/controller_zh_CN.qm";
    }
    else
    {
        qmFilename = runPath + "/controller_en_US.qm";
    }
    if (translator->load(qmFilename))
    {
        qApp->installTranslator(translator);
    }
        ui->retranslateUi(this);             // 重新设置界面显示
}


void MainWindow::showEvent(QShowEvent *) {
    windowContext->CheckExpiredBookings();
    AdminDialog *adminDialog = windowContext->getAdminDialog();
    adminDialog->clearCurrentUserInfo();
    BookInformation *bookInformation = windowContext->getBookInformation();
    bookInformation->UpdateCurrentBooking();

    readSettings();

    sessionManage->RestoreSession();

    /// 专享信息
    ReservationInformation *reservationInformation = windowContext->getReservationInformation();
    reservationInformation->updateReservation();

    regularTaskTimer->start(1000);

    // 启动刷卡保护定时器
    cardTiming->start();

    NTG()->SetControllerLed(true);
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    NTG()->SetNetworkStatusLed(instrumentStatus->GetNetWorkStatus());
    windowContext->InitGlobal();
}

void MainWindow::onCardDetected(uint cardSerial) {

    if (false == windowContext->isCardEnabled() || cardSerial == 0 ||
        cardTiming->elapsed() < windowContext->getCardGuard()) {
        return;
    }

    cardTiming->restart();

    /// 关闭之前的窗口，点亮屏幕
    NTG()->ClosePopupWindow();
    QWSServer::screenSaverActivate(false);

    DEBUG_LOG("card serial is :" << cardSerial);

    /// 获取用户详细信息
    UserInformation *userInformation = windowContext->getUserInformation();
    if (false == userInformation->GetNewUserInfo(cardSerial)) {
        ERROR_LOG("don't find user info for this card");
        return;
    }

    /// 管理员登录过的操作
    if (true == adminAction->doAdminActionInfo()) {
        DEBUG_LOG("admin operation over");
        return;
    }

    /// 仪器处于使用状态，用户想退出，或是其他用户想占用仪器
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    if (true == instrumentStatus->GetInUseStatus()) {
        UserLogoutCardSwiping();
        return;
    }

    IniParse ini(OSLA_CONFIG);
    long bookingModelId = ini.GetLong("Instrument.BookingModelId", 0);
    if ((bookingModelId & BookingModelType::Book) == 0) {
        ServiceEvent::GetInstance()->AddUserEvent(userInformation->GetNewUser(),
                                                  biotech::osla::USER_ACTION_START_SESSION,
                                                  biotech::osla::USER_ACTION_FAILED_BOOKINGMODELID);
        //tr("按时预约未开启")
        PROMPTINFO(this, tr("The appointment was not opened on time"), "media/TimeBookNotStart.mp3", true, 500);
        return;
    }

    /// 用户是否可以登录
    if (false == windowContext->LoginCheck()) {
        return;
    }

//don't use LooseMode
//    if (false == windowContext->isLooseMode()) {
//        BookInformation *bookInformation = windowContext->getBookInformation();
//        User &newUser = userInformation->GetNewUser();
//        if (!(true == bookInformation->IsBookHolding(newUser.timestamp, instrumentStatus) ||
//              InstrumentStatus::RESERVATION == instrumentStatus->GetWorkStatus())) {
//            //add user event,lwp,2018-1-24
//            ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_START_SESSION,
//                                                      biotech::osla::USER_ACTION_FAILED_NO_BOOK_IN_STRICT);
//            //end
//            PROMPTINFO(this, tr("请注意, 您需要提前预约"), "media/PrepareBooking.mp3", true, 500);
//            return;
//        }
//    }

    /// 预约检查
    if (false == windowContext->BookingCheck()) {
        return;
    }

    // 黑名单判断
    User &newUser = userInformation->GetNewUser();
    if (true == newUser.isInBlacklist) {
        //add user event,lwp,2018-1-24
        ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_START_SESSION,
                                                  biotech::osla::USER_ACTION_FAILED_IN_BLACK_LIST);
        //end
        PROMPTINFO(this, tr("Please note that you are blacklisted by the system"), "media/UserInBlackList.mp3", true, 500);
        return;
//        QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("注意"),
//                                                  tr("您已被系统列入黑名单, 使用仪器将产生额外费用, 是否继续？"),
//                                                  QMessageBox::Yes | QMessageBox::No, this);
//        messageBox->setDefaultButton(QMessageBox::No);
//        messageBox->setAttribute(Qt::WA_DeleteOnClose);
//
//        if (messageBox->exec() != QMessageBox::Yes) {
//            ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_NULL_OP,
//                                                      biotech::osla::USER_ACTION_NULL_RESULT);
//            return;
//        }
    }

    SessionNode node = {biotech::osla::SESSION_TYPE_NORMAL, -1, biotech::osla::USER_ACTION_START_SESSION};
    if (false == sessionManage->StartSession(node)) {
        return;
    }
}

void MainWindow::onCardDetectedFromNet(int id, int userid, uint cardId, int userrole, int op) {
    int status = 0;
    int bookId = 0;

    UserInformation *userInformation = windowContext->getUserInformation();
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    BookInformation *bookInformation = windowContext->getBookInformation();

    auto endSessionFun = [&]() {
        DEBUG_LOG(":" << "normal over session about net");
        biotech::osla::Session &ss = userInformation->GetSessionInfo();
        bookId = ss.booking_id();
        sessionManage->EndSession();
        status = 1;
        DEBUG_LOG(":" << "status is : " << status);
    };

    do {
        DEBUG_LOG("call this function [" << __FUNCTION__ << "]");
        if ((1 != op)&&(10 != op)&&(11 != op)&&(12 != op)&&(13 != op)&&(14 != op)&&(15 != op)) { /// 1关机 >10开机
            status = -1;
            break;
        }
        /// 关闭之前的窗口，点亮屏幕
        NTG()->ClosePopupWindow();
        QWSServer::screenSaverActivate(false);

        uint tempSC = (uint) cardId;
        DEBUG_LOG("card serial is :" << tempSC);

        /// 获取用户详细信息
        if (false == userInformation->GetNewUserInfo(tempSC)) {
            status = 4;/// 卡号在终端不存在
            break;
        }

        if (false == instrumentStatus->GetInUseStatus()) { /// 设备不在使用
            status = 6;  /// 仪器非试验中
            DEBUG_LOG(": Remote boot judgment!");
            /// 用户是否可以登录
            ///
//            if ( (true == userInformation->NewUserIsAdmin(userid)) || (true == userInformation->NewUserIsAdmin())){
//                DEBUG_LOG(": user is admin");
//                break;
//            }
//            else {
//                if (false == windowContext->LoginCheck()) {
//                    status = 5; /// 用户无权限操作
//                    break;
//                }
//                /// 预约检查
//                if (false == windowContext->BookingCheck()) {
//                    status = 5; /// 用户无权限操作
//                    return;
//                }

//                // 黑名单判断
//                User &newUser = userInformation->GetNewUser();
//                if (true == newUser.isInBlacklist) {

//                    ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_START_SESSION,
//                                                              biotech::osla::USER_ACTION_FAILED_IN_BLACK_LIST);

//                    PROMPTINFO(this, tr("请注意, 您已被系统列入黑名单"), "media/UserInBlackList.mp3", true, 500);
//                    status = 5; /// 用户无权限操作
//                    return;
//                }
//            }
           SessionNode node  = {biotech::osla::SESSION_TYPE_NORMAL, -1, biotech::osla::USER_ACTION_NULL_OP};
           if(op >=10){
               switch(op){
                    case 10:/// 常规状态
                         node = {biotech::osla::SESSION_TYPE_NORMAL, InstrumentStatus::NORMAL, biotech::osla::USER_ACTION_START_SESSION};
                         status = 10;
                         break;
                    case 11:///代刷
                         node = {biotech::osla::SESSION_TYPE_ASSISTANCE, InstrumentStatus::NORMAL, biotech::osla::USER_ACTION_START_ASSISTANCE};
                         status = 11;
                         break;
                    case 12:/// 培训状态
                         node = {biotech::osla::SESSION_TYPE_TRAINING, InstrumentStatus::TRAINING, biotech::osla::USER_ACTION_START_TRAINING};
                         status = 12;
                         break;
                    case 13:/// 机器维护状态
                         node = {biotech::osla::SESSION_TYPE_MAINTENANCE, InstrumentStatus::MAINTENANCE, biotech::osla::USER_ACTION_START_MAINTENANCE};
                         status = 13;
                         break;
                    case 14:/// 教学状态
                         node = {biotech::osla::SESSION_TYPE_EDUCATION, InstrumentStatus::EDUCATION, biotech::osla::USER_ACTION_START_EDUCATION};
                         status = 14;
                         break;
                    case 15:/// 委托状态
                         node = {biotech::osla::SESSION_TYPE_ENTRUST, InstrumentStatus::ENTRUST, biotech::osla::USER_ACTION_START_ENTRUST};
                         status = 15;
                         break;
                    default:
                         break;
               }
                if (false == sessionManage->StartSession(node)) {
                    DEBUG_LOG(": boot failed!");
                    status = 8;//开机失败
                    return;
                }
                DEBUG_LOG(": boot success!");
            }
            break;
        }

        if (true == instrumentStatus->GetInUseStatus()) { /// 设备在使用

            status = 7;  /// 仪器试验中    不能开机操作   关机操作判断

            bool isCurrentUser = userInformation->CurrentUserIsNewUser() || userInformation->NewUserIsAdmin(userid);
            bool isBookingHolderUser = false;

            User &newUser = userInformation->GetNewUser();
            if (true == bookInformation->IsBookHolding(newUser.timestamp, instrumentStatus) &&
                true == bookInformation->IsCurrentBookUser(newUser)) {
                isBookingHolderUser = true;
            }

            if (false == isCurrentUser && false == isBookingHolderUser) { // 刷卡卡号非当前使用者的卡号
                DEBUG_LOG("current card is : " << tempSC << ", now user card is :"
                                               << userInformation->GetCurrentUser().card.serial);
                status = 5; /// 用户无权限操作
                break;
            }
            if(1 == op){
                if (true == isCurrentUser) {
                    DEBUG_LOG(": close session is current user");
                }

                if (true == isBookingHolderUser) {
                    DEBUG_LOG(": close session is next book user");
                }
               endSessionFun();
            }
            break;
        }

    } while (false);

    emit sendDetectedFromNetResp(id, status, bookId);
}

void MainWindow::ShowCurrentUserInfo() {
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    UserInformation *userInformation = windowContext->getUserInformation();
    AdminDialog *adminDialog = windowContext->getAdminDialog();
    SettingsDialog *settingsDialog = windowContext->getSettingsDialog();

    if (true == instrumentStatus->GetInUseStatus()) {
        User &currentUser = userInformation->GetCurrentUser();
        ui->currentUserLabel->setStyleSheet("color: rgb(0, 85, 255);");
        ui->currentUserInfoLabel->setStyleSheet("color: rgb(255, 0, 0); font-size: 26px;");
        ui->currentUserInfoLabel->setText(QString("%1\n(%2)").arg(currentUser.fullName).arg(currentUser.phone));
        ui->timerLabel->setStyleSheet("color: rgb(0, 85, 255);");
        ui->timerLCD->setStyleSheet("color: rgb(255, 0, 0);");
        adminDialog->setCurrentUserInfo(currentUser);
    } else {
        ui->currentUserLabel->setStyleSheet("color: rgb(200, 200, 200);");
        ui->currentUserInfoLabel->setStyleSheet("color: rgb(200, 200, 200); font-size: 26px;");
        ui->currentUserInfoLabel->setText(tr("None"));//无
        ui->timerLabel->setStyleSheet("color: rgb(200, 200, 200);");
        ui->timerLCD->setStyleSheet("color: rgb(200, 200, 200);");
        ui->timerLCD->display("0000:00:00");
        adminDialog->clearCurrentUserInfo();
    }

    settingsDialog->setInUse(instrumentStatus->GetInUseStatus());
}

bool MainWindow::UserLogoutCardSwiping() {
    bool status = false;
    UserInformation *userInformation = windowContext->getUserInformation();
    User &newUser = userInformation->GetNewUser();

    if (false == userInformation->CurrentUserIsNewUser()) { // 刷卡卡号非当前使用者的卡号
        //仪器使用中
        PROMPTINFO(this, tr("Instrument in use"), "media/inuse.mp3", true, 500);
        ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_START_SESSION,
                                                  biotech::osla::USER_ACTION_FAILED_INSTRUMENT_INUSE);
        status = false;
    } else {
        PROMPTINFO(this, "", "media/IsEndCurrentExperiment.mp3", true, 500);
        //tr("注意"),tr("是否结束当前实验？")
        QMessageBox *messageBox = new QMessageBox(QMessageBox::Warning, tr("Notice"),
                                                  tr("whether to end the current experiment"), QMessageBox::Yes | QMessageBox::No, this);
        messageBox->setDefaultButton(QMessageBox::No);
        messageBox->setAttribute(Qt::WA_DeleteOnClose);
        if (messageBox->exec() == QMessageBox::Yes) {
            sessionManage->EndSession();
        }
        status = true;
    }

    return status;
}

void MainWindow::SetCurrentStateInfo() {
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    int cStatus = instrumentStatus->GetInstrumentStatus() << 4 | instrumentStatus->GetWorkStatus() << 1 |
                  (true == instrumentStatus->GetInUseStatus() ? 1 : 0);
    emit stateChanged(cStatus);
    UserInformation *userInformation = windowContext->getUserInformation();
    instrumentStatus->ShowWorkStatus(ui->currentStateLabel, userInformation->GetCountInTraining());
}

void MainWindow::readSettings() {
    windowContext->InitGlobal();
    windowContext->reloadFtpParams();

    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    IniParse ini(OSLA_CONFIG);
    // 仪器不在使用中则立即更新状态，在使用中则在使用结束后更新
    if (false == instrumentStatus->GetInUseStatus()) {
        instrumentStatus->SetInstrumentStatus(ini.GetLong("Instrument.Status"));
        SetCurrentStateInfo();
    }

    /// 动态更新设备名
    ui->instrumentNameLabel->setText(ini.GetString("Instrument.Name").c_str());

    /// 动态更新管理员信息
    QString adminFullName(ini.GetString("Instrument.AdminFullName").c_str());
    QString adminPhone(ini.GetString("Instrument.AdminPhone").c_str());
    //管理员
    ui->adminInfoLabel->setText(tr("Administrator: %1\n(%2)").arg(adminFullName).arg(adminPhone));

    /// 动态更新设备信息
    instrumentStatus->InitBaseInfo();
    NormalTools::GetInstance()->UpdateWorkingTime();

    windowContext->getUserInformation()->updateSettings();
}

void MainWindow::setNetworkState(bool online) {
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    instrumentStatus->SetNetWorkStatus(online);
    FtpParse *ftp = windowContext->getFtp();

    SupportFeature *supportFeature = windowContext->getSupportFeature();

    if (true == instrumentStatus->GetNetWorkStatus()) {
        NTG()->SetPilotLamp(false);
        if (true == supportFeature->isSupportFtp()) {
            ftp->SetNetStatus(true);
        }
        //tr("设备上线"));
        PROMPTINFO(this, tr("Equipment online"));
        ui->networkStateLabel->setPixmap(QPixmap(":/images/network-transmit.png"));
        int cStatus = instrumentStatus->GetInstrumentStatus() << 4 | instrumentStatus->GetWorkStatus() << 1 |
                      (true == instrumentStatus->GetInUseStatus() ? 1 : 0);
        emit stateChanged(cStatus);
        //NTG()->StartNtpService();
    } else {
        NTG()->SetPilotLamp(true);
        if (true == supportFeature->isSupportFtp()) {
            ftp->SetNetStatus(false);
        }
        //tr("设备离线"));
        PROMPTINFO(this, tr("Equipment offline"));
        ui->networkStateLabel->setPixmap(QPixmap(":/images/network-offline.png"));
    }

    NTG()->SetNetworkStatusLed(online);
}

void MainWindow::doRegularTask() {
    static int count =0;
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    if (true == instrumentStatus->GetNetWorkStatus()) {
        count ++;
    }

    if(count == 2){
       emit qrImageUrl_update();
    }

    if(count == 58)
        count = 0;

    // 刷新时钟
    NTG()->FlashCalendarInfo(ui->datetimeLabel);

    WatchDog::GetInstance()->Feed();

    QDateTime currentDateTime = QDateTime::currentDateTime();
    //QDate currentDate = currentDateTime.date();

    uint currentDateTime_t = currentDateTime.toTime_t();                     /// 到1970年的秒数
    uint currentTime_t = abs(currentDateTime.time().secsTo(QTime(0, 0, 0))); /// 到0点的秒数

    ReservationInformation *reservationInformation = windowContext->getReservationInformation();
    // 每天0点检查当天是否节假日以及是否存在专享时段
    if (currentTime_t == 0) {
        reservationInformation->updateReservation();
    }
    reservationInformation->refreshReservation(instrumentStatus);


    ///更新温度信息,
    if (currentTime_t % 20 == 0) {
        UpdateTempHumidity(currentTime_t);
    }

    if (currentTime_t % 600 == 0) {
        QProcess::startDetached("hwclock -w --utc");
    }

    if (0 == currentTime_t % 5) {
        sessionManage->SendHeartbeat();
    }

    PowerDelay *powerDelay = windowContext->getPowerDelay();
    powerDelay->RegularlyCheck();

    if (true == instrumentStatus->GetInUseStatus()) {
        // 实验中，产生的数据，属于会话数据
        int timeElapsed = sessionManage->UpdateSessionStatus();
        NTG()->SetTimerLCDText(timeElapsed, ui->timerLCD);
    }

    BookInformation *bookInformation = windowContext->getBookInformation();
    UserInformation *userInformation = windowContext->getUserInformation();
    bookInformation->RefreshShowBookInfo(userInformation, instrumentStatus);

    timeCheckNum += 1;
    if (windowContext->getServiceTime() <= timeCheckNum) {
        NTG()->StartNtpService();
        timeCheckNum = 0;
    }

    biotech::osla::Booking &book = windowContext->getNowUserBooking();
    if (book.end_time() != 0 && false == book.istips() && ((long) book.end_time() - (long) currentDateTime_t < 180)) {
        book.set_istips(true);
        DEBUG_LOG(book.end_time() << "," << currentDateTime_t);
        //请注意, 您的预约时间即将结束
        PROMPTINFO(this, tr("Please note that your reservation is coming to end"), "media/BookingTimeWillEnd.mp3", true, 500);
    }
    if (book.end_time() != 0 && false == book.isend() && ((long) currentDateTime_t > book.end_time())) {
        book.set_isend(true);
        DEBUG_LOG(book.end_time() << "," << currentDateTime_t);
        //请注意, 您的预约时间结束
        PROMPTINFO(this, tr("Please note that your reservation is  end"), "media/BookingTimeEnd.mp3", true, 500);
        book.Clear();
    }

    SqlWorker::GetSqlWorkByThread()->checkOldData();
}

void MainWindow::updateCurrentBooking() {
    BookInformation *bookInformation = windowContext->getBookInformation();
    bookInformation->UpdateCurrentBooking();
}


void MainWindow::onSettingsDialogClosed() {
    DEBUG_LOG("MainWindow: Settings panel closed");
    SettingsDialog *settingsDialog = windowContext->getSettingsDialog();
    UserInformation *userInformation = windowContext->getUserInformation();
    if (!settingsDialog->isSettingsChanged()) {
        User &newUser = userInformation->GetNewUser();
        ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_CONFIG_SYSTEM,
                                                  biotech::osla::USER_ACTION_NULL_RESULT);
    } else {
        settingsDialog->resetSettingsChanged();
    }
}

void MainWindow::doAdminAction(int dialogCode) {
    struct _TempNode_ {
        int code;
        std::function<bool()> fun;
    } arr[] = {
            {AdminDialog::START_TRAINING,    std::bind(&AdminAction::AdminActionStartTraning, adminAction)},
            {AdminDialog::START_EDUCATION,   std::bind(&AdminAction::AdminActionStartEducation, adminAction)},
            {AdminDialog::START_MAINTENANCE, std::bind(&AdminAction::AdminActionStartMaintenance, adminAction)},
            {AdminDialog::START_ASSISTANCE,  std::bind(&AdminAction::AdminActionStartAssistance, adminAction)},
            {AdminDialog::START_PROJECT,     std::bind(&AdminAction::AdminActionEntrust, adminAction)},
            {AdminDialog::END_SESSION,       std::bind(&AdminAction::AdminActionEndSession, adminAction)},
            {AdminDialog::CONFIG_SYSTEM,     std::bind(&AdminAction::AdminActionConfigSystem, adminAction)},
            {AdminDialog::VIEW_RECORDS,      std::bind(&AdminAction::AdminActionViewRecords, adminAction)},
    };
    _TempNode_ def = {0, std::bind(&AdminAction::AdminActionDefault, adminAction)};

    int index = 0;

    for (; index < (int) (sizeof(arr) / sizeof(arr[0])); ++index) {
        if (dialogCode == arr[index].code) {
            arr[index].fun();
            break;
        }
    }

    if (index == (int) (sizeof(arr) / sizeof(arr[0]))) {
        def.fun();
    }
}

void MainWindow::OnSwapCardIntervalChanged(int interval) {
    windowContext->setCardGuard(interval);
    CardReader *cardReader = windowContext->getCardReader();
    cardReader->setGuardInterval(interval);
}

void MainWindow::onInstrumentStatusUpdated(int status) {
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    UserInformation *userInformation = windowContext->getUserInformation();

    if (true == instrumentStatus->GetInUseStatus()) {
        sessionManage->EndSession();
    }

    struct _TempNode_ {
        int status;
        int code;
    } arr[] = {
            {biotech::osla::INSTRUMENT_STATUS_ACTIVE,       biotech::osla::USER_ACTION_SET_INSTRUMENT_ACTIVE},
            {biotech::osla::INSTRUMENT_STATUS_FAULTY,       biotech::osla::USER_ACTION_SET_INSTRUMENT_FAULTY},
            {biotech::osla::INSTRUMENT_STATUS_INACTIVE,     biotech::osla::USER_ACTION_SET_INSTRUMENT_INACTIVE},
            {biotech::osla::INSTRUMENT_STATUS_OUTOFSERVICE, biotech::osla::USER_ACTION_SET_INSTRUMENT_OUTOFSERVICE},
    };

    for (int i = 0; i < (int) (sizeof(arr) / sizeof(arr[0])); ++i) {
        if (status == arr[i].status) {
            User &newUser = userInformation->GetNewUser();
            ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_CONFIG_SYSTEM, arr[i].code);
            break;
        }
    }

    ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_INSTRUMENT_STATUS_CHANGED, status);
    DEBUG_LOG("MainWindow: Instrument status changed to" << status);
    instrumentStatus->SetInstrumentStatus(status);
    SetCurrentStateInfo();
}

void MainWindow::onNetworkSettingsChanged() {
    ///要考虑newUser是否有效的问题
    UserInformation *userInformation = windowContext->getUserInformation();
    User &newUser = userInformation->GetNewUser();
    ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_CONFIG_SYSTEM,
                                              biotech::osla::USER_ACTION_NETWORKSETTINGS_CHANGED);
}

void MainWindow::showBookingSchedule() {
    QStandardItemModel *model = new QStandardItemModel(this);
    BookInformation *bookInformation = windowContext->getBookInformation();
    biotech::osla::BookingList recentBookings = bookInformation->GetCurrentBookList();
    //SqlWorker *sqlWorker = windowContext->getSqlWorker();
    SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();

    biotech::osla::Booking &book = windowContext->getNowUserBooking();
    if (book.id() > 0) {
        biotech::osla::Booking *b = recentBookings.add_booking();
        b->CopyFrom(book);
    }

    for (int i = 0; i < recentBookings.booking_size(); i++) {
        biotech::osla::Booking booking = recentBookings.booking(i);
        QStandardItem *item = new QStandardItem();
        item->setData((uint) booking.start_time(), Qxt::ItemStartTimeRole);
        item->setData((uint) booking.end_time() - (uint) booking.start_time(), Qxt::ItemDurationRole);
        User user = sqlWorker->getUserInfoById((uint) booking.user_id());
        item->setData(user.fullName, Qt::DisplayRole);
        item->setData(QColor(85, 170, 255), Qt::BackgroundColorRole);
        model->appendRow(item);
    }

    ScheduleDialog *bookingSchedule = new ScheduleDialog(this);
    int minInterval = instrumentStatus->GetMinInterval();
    if (minInterval > 60) {
        minInterval = 60;
    }
    bookingSchedule->setCellInterval(minInterval);
    bookingSchedule->setDataModel(model);
    bookingSchedule->showFullScreen();
}

void MainWindow::onBindingReleased(bool result) {
    UserInformation *userInformation = windowContext->getUserInformation();
    User &newUser = userInformation->GetNewUser();

    if (true == result) {
        // 记录用户事件：解除仪器关联
        ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_CONFIG_SYSTEM,
                                                  biotech::osla::USER_ACTION_UNBIND_INSTRUMENT);
        // 记录系统事件：终端恢复出厂设置
        ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_DEVICE_FACTORY_RESET);
        // 等待5秒以尽可能完成最后的事件同步（不保证能够同步）
        msleep(5000);
        windowContext->FactoryReset();
        return;
    }
    //无法解除仪器关联！
    PROMPTINFO(this, tr("Unable to disassociate instrument"));

    // 记录用户事件：解除仪器关联失败
    ServiceEvent::GetInstance()->AddUserEvent(newUser, biotech::osla::USER_ACTION_CONFIG_SYSTEM,
                                              biotech::osla::USER_ACTION_FAILED_UNBIND_INSTRUMENT);
    windowContext->setCardEnabled(true);
}

void MainWindow::FocusUnbinding() {
    windowContext->FactoryReset();
}

void MainWindow::setErrorInfo(QString text) {
    ui->statusbarLabel->setStyleSheet("color:red");
    ui->statusbarLabel->setText(text);
    NTG()->SetControllerLed(false);
}

bool MainWindow::UpdateTempHumidity(uint currentTime_t) {
    float t;
    float h;
    bool flag = true;  

    if (temperature(&t) == 0)
    {
        flag = false;
    }
    else if (humidity(&h) == 0)
    {
        flag = false;
    }

    //if (0 == QString::compare(ui->statusbarLabel->text(), "如有任何疑问，请与仪器管理员联系")) {
    ui->statusbarLabel->setStyleSheet("color:black");
    if (flag == false)
    {   //读取温湿度失败时则不显示且不上报
        //版本
        QString info = QString(tr("Version:%1").arg(windowContext->getDeviceVersion().c_str()));
        ui->statusbarLabel->setText(info);
        return false;
    }
     //温湿度
   // QString info = QString("Version:%1  Temp/Huni:%2°C/%3%RH").arg(windowContext->getDeviceVersion().c_str()).arg(t, 0, 'f', 1).arg((int) h);
   // ui->statusbarLabel->setText(info);

    /// 记录到数据库中，添加事件
    /// 温度:1 单位 摄氏度
    /// 湿度:2
    /// 电流:3
    //InstrumentStatus* instrumentStatus = windowContext->getInstrumentStatus();
    ///如果设备在线，每隔5秒采集一次，如果设备不在线，每隔30秒采集一次，自我保护
    //if ((currentTime_t % 5 == 0 && true == instrumentStatus->GetNetWorkStatus()) || (currentTime_t % 30 == 0 && false== instrumentStatus->GetNetWorkStatus()))
    long CaptureTime = windowContext->getCaptureTime();
    if (currentTime_t % CaptureTime == 0) {
        biotech::osla::DeviceCaptureEvent deviceCaptureEvent;
        char tempt[16] = {0};
        sprintf(tempt, "%.2f", t);
        deviceCaptureEvent.set_devicename("1");
        deviceCaptureEvent.set_devicevalue(tempt);
        ServiceEvent::GetInstance()->AddDeviceCaptureEvent(deviceCaptureEvent);
        char temph[16] = {0};
        sprintf(temph, "%.2f", h);
        deviceCaptureEvent.set_devicename("2");
        deviceCaptureEvent.set_devicevalue(temph);
        ServiceEvent::GetInstance()->AddDeviceCaptureEvent(deviceCaptureEvent);

        DEBUG_LOG("update TempHumidity");
    }
    return true;
}

void MainWindow::onShowWorkStatus(int num) {
    InstrumentStatus *instrumentStatus = windowContext->getInstrumentStatus();
    instrumentStatus->ShowWorkStatus(ui->currentStateLabel, num);
}

void MainWindow::onUpgradeParamsUpdated()
{
    QTimer::singleShot(5000, windowContext->getUpgrade(), SLOT(onParamsChanged()));
}

void MainWindow::onUpgradePlanUpdated()
{
    QTimer::singleShot(5000, windowContext->getUpgradePlan(), SLOT(onPlanChanged()));
}

void MainWindow::showWaringText(QString text)
{
    ui->warningLabel->setVisible(true);
    ui->warningLabel->setText(text);
}

WindowContext *MainWindow::getWindowContext() {
    return windowContext;
}


