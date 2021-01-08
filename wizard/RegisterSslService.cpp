#include <functional>
#include <QtCore/QtCore>
#include <object/RegisterObject.h>
#include <memory>
#include <tools/ConvertFrameObject.h>
#include <tools/FrameOkError.h>

#include "wizard/RegisterSslService.h"
#include "wizard/Public.h"
#include "sqlworker/sqlworker.h"
#include "sslconnection/SslConnection.h"
#include "sslconnection/ClientContext.h"
#include "deviceinfo/version.h"
#include "tools/IniParse.h"
#include "tools/log.h"
#include "sslconnection/Tools.h"
#include "tools/PublicDefine.h"
#include "protobuf/message.pb.h"
#include "../controller/Context/userqualification.h"
#include "deviceinfo/enum.h"


static int TransferTimeOutNum = 30 * 1000;

#define UploadDBTemplate(Type, name, fun, titleTip, infoTip) \
     FrameSP sp = FrameOkError::createRecvError(frame); \
     stopTimer(transferTimerId);\
     do {\
        biotech::osla::Type name;\
        if (false == ConvertFrameObject::convertFrame2Object(frame.get(), name)) {\
            break;\
        }\
        SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();\
        if (!sqlWorker->fun(name)) {\
            break;\
        }\
        sp = FrameOkError::createRecvOk(frame);\
        emit downloadPageInfo(titleTip, infoTip);\
    } while (false);\
    transferTimerId = startTimer(TransferTimeOutNum);\
    return sp;


struct ParseDataNode {
    int code;
    std::function<FrameSP(FrameSP)> fun;
};

static const int MaxTryCount = 3;

SslService::SslService(QObject *parent) : QObject(parent) {
    context = new ClientContext();
    sslConnection = new SslConnection(this);

    connect(sslConnection, SIGNAL(encrypted()), this, SLOT(doConnected()));
    connect(sslConnection, SIGNAL(disconnected()), this, SLOT(doDisconnected()));
    connect(sslConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(error(QAbstractSocket::SocketError))));
    connect(sslConnection, SIGNAL(newMessage(FrameSP)), this, SLOT(parseFrame(FrameSP)));
    connect(&sslConnectTimer, SIGNAL(timeout()), this, SLOT(onSslConnectTimerTimeout()));

    context->setInstrumentId(0);
    context->setSslService(this);
}

SslService::~SslService() {
    stop();
    SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
    if (nullptr != sqlWorker) {
        sqlWorker->closeDatabase();
    }
    google::protobuf::ShutdownProtobufLibrary(); // 关闭google protobuf
    ERROR_LOG("SslService: Destructed");
}

void SslService::start(bool isGetInstrumentList) {
    if (isGetInstrumentList)
    {
        setPhase(SslConnectingState);
    }
    else
    {
        setPhase(WaitingForBindingAck);
    }
    clearSendMessage();
    IniParse ini(OSLA_CONFIG);
    std::string sslC = ini.GetString("Network.SslCert");
    DEBUG_LOG("ssl file path is : " << sslC);
    QString sslCert(sslC.c_str());

    sslConnection->setPeerVerifyMode(QSslSocket::QueryPeer);
    sslConnection->setLocalCertificate(sslCert);
    sslConnection->setPrivateKey(sslCert);
    sslConnection->connectToHostEncrypted(sslHost, sslPort);
    sslConnectTimer.start(TransferTimeOutNum);
    WARN_LOG("SslService: Connecting to server" << QString("%1:%2").arg(sslHost).arg(sslPort).toLocal8Bit().data());
    context->setRestartEnabled(true);
}

void SslService::stop() {
    context->setRestartEnabled(false);
    clearSendMessage();
    sslConnectTimer.stop();
    sslConnection->resetConnection();
    context->setConnected(false);
    ERROR_LOG("SslService: Stopped");
    phase = 0;
}

void SslService::doConnected() {
    DEBUG_LOG("");
    if (!sslConnection->isEncrypted()) { // ssl加密失败则断开连接
        DEBUG_LOG("ssl Encrypted faild!");
        emit connectionError(SslEncryptedError);
        return;
    }

    sslConnectTimer.stop();

    DEBUG_LOG("SSL connection encrypted, next send init request or bind request");

    // 发送初始化请求
    if (phase == SslConnectingState) {
        emit showMessage(tr("Fetching instrument list..."));//获取仪器列表
        phase = WaitingForInstrumentList;
        emit changeRegisterState(phase);
        sendInitRequest();
    } else if (phase == WaitingForBindingAck) {
        emit showMessage(tr("Send instrument association request..."));//发送仪器关联请求
        sendBindingRequest(biotech::osla::InstrumentBinding_Option_REQUEST_BINDING);
    }
}

void SslService::doDisconnected() {
    //DEBUG_LOG("SSL connection closed");
}


void SslService::setHostInfo(QString sslHost, int sslPort) {
    this->sslHost = sslHost;
    this->sslPort = sslPort;
}

QAbstractSocket::SocketState SslService::state() {
    return sslConnection->state();
}

void SslService::setInstrumentPage(InstrumentPage *instrumentPage) {
    SslService::instrumentPage = instrumentPage;
}

void SslService::parseFrame(FrameSP frame) {
    DEBUG_LOG("parse frame, seq is : " << frame->getSequence());
    DEBUG_LOG("message type is : " << frame->getMessageType() << ",status is " << phase);
    ParseDataNode statusParse[] = {
            {
                    WaitingForInstrumentList,
                    std::bind(&SslService::UpdateWaitingForInstrumentList, this, std::placeholders::_1)
            },
            {
                    WaitingForBindingAck,
                    std::bind(&SslService::UpdateWaitingForBindingAck, this, std::placeholders::_1)
            },
            {
                    WaitingForDataDownload,
                    std::bind(&SslService::UpdateWaitingForDataDownload, this, std::placeholders::_1)
            },
            {
                    WaitingForBindingConfirm,
                    std::bind(&SslService::UpdateWaitingForBindingConfirm, this, std::placeholders::_1)
            }
    };

    FrameSP res;
    for (int i = 0; i < sizeof(statusParse) / sizeof(statusParse[0]); ++i) {
        if (phase == statusParse[i].code) {
            res = statusParse[i].fun(frame);
            QProcess::startDetached("sync");
            break;
        }
    }

    if (res) {
        DEBUG_LOG("send process result , frame seq is : " << res->getSequence());
        sslConnection->sendMessage(res);
    }

    return;
}

void SslService::checkNeedResendMessage() {
    time_t now = time(nullptr);

    auto iter = m_SendMessage.begin();
    while (m_SendMessage.end() != iter) {
        auto pos = iter++;
        SendNode &node = pos->second;
        if ((now - node.sendTime) > PublicDefine::getSyncTimeOut()) {
            if (node.resend < 3) {
                sslConnection->sendMessage(node.sendFrame);
                node.sendTime = time(nullptr);
                node.resend += 1;
            } else {
                biotech::osla::Command command;
                command.set_option(biotech::osla::Command_Option::Command_Option_CLIENT_RECV_ERR);
                auto frameIn = std::make_shared<Frame>();
                ConvertFrameObject::convertObject2Frame(frameIn.get(), command, biotech::osla::COMMAND);
                node.parse->parseRespose(frameIn.get(), 0);
                m_SendMessage.erase(pos);
            }
        }
    }
}

void SslService::clearSendMessage() {
    biotech::osla::Command command;
    command.set_option(biotech::osla::Command_Option::Command_Option_CLIENT_RECV_ERR);
    auto frameIn = std::make_shared<Frame>();
    ConvertFrameObject::convertObject2Frame(frameIn.get(), command, biotech::osla::COMMAND);

    for (auto iter = m_SendMessage.begin(); iter != m_SendMessage.end(); ++iter) {
        SendNode &node = iter->second;
        node.parse->parseRespose(frameIn.get(), nullptr);
    }
    m_SendMessage.clear();
}

int SslService::getPhase() const {
    return phase;
}

void SslService::setPhase(int s) {
    SslService::phase = s;
}

void SslService::sendInitRequest() {
    static int count = 0;
    if (count >= MaxTryCount) {
        count = 0;
        emit connectionError(TimeOutError);
        return;
    }

    FrameSP ptr = std::make_shared<Frame>();
    biotech::osla::InitPhase initPhase;
    initPhase.set_option(biotech::osla::InitPhase_Option_CLIENT_INIT_REQ);
    initPhase.set_bind_code(orgBindCode);
    ConvertFrameObject::convertObject2Frame(ptr.get(), initPhase, biotech::osla::INIT_PHASE);
    sslConnection->sendMessage(ptr);

    DEBUG_LOG(__FUNCTION__ << "called success");
    count++;
    transferTimerId = startTimer(TransferTimeOutNum); // 发送成功开启超时定时器
}

void SslService::sendBindingRequest(biotech::osla::InstrumentBinding_Option option) {
    static int count = 0;
    if (count >= MaxTryCount) {
        count = 0;
        emit connectionError(TimeOutError);
        return;
    }

    biotech::osla::InstrumentBinding request;
    request.set_option(option);
    request.set_instrument_id(instrumentPage->selectedInstrumentId());
    request.set_client_model(241);
    request.set_client_version(qApp->applicationVersion().toLatin1().data());
    QFile cmdForMacFile("/proc/cmdline");
    if (cmdForMacFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&cmdForMacFile);
        QString StringMac = stream.readAll();
        int MAC = StringMac.indexOf(QRegExp("mac="), 0);
        QString t = StringMac.mid(MAC + 4, 17);
        QByteArray ba = t.toLatin1();
        request.set_client_mac_addr(ba.data());
    }
    cmdForMacFile.close();
    request.set_bindcode(orgBindCode);

    FrameSP ptr = std::make_shared<Frame>();
    ConvertFrameObject::convertObject2Frame(ptr.get(), request, biotech::osla::INSTRUMENTBINDING);
    sslConnection->sendMessage(ptr, true);

    count++;
    transferTimerId = startTimer(TransferTimeOutNum); // 发送成功开启超时定时器
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

FrameSP SslService::UpdateWaitingForInstrumentList(FrameSP frame) {
    FrameSP res;

    do {
        if (frame->getMessageType() == biotech::osla::INSTRUMENTLIST) { // 收到其他消息，忽略
            res = FrameOkError::createRecvError(frame);
            biotech::osla::InstrumentList instrumentList;
            if (false == ConvertFrameObject::convertFrame2Object(frame.get(), instrumentList)) {
                break;
            }

            DEBUG_LOG("Instrument list received, total " << instrumentList.instrument_size());
            stopTimer(transferTimerId);
            IniParse ini(OSLA_CONFIG);
            for (int i = 0; i < instrumentList.instrument_size(); i++) { // 解析仪器列表信息
                int id = instrumentList.instrument(i).id(); // 获取仪器ID
                QString name = QString(instrumentList.instrument(i).name().c_str()); // 获取仪器名称
                const std::string &orgCode = instrumentList.instrument(i).org_code();
                ini.Set("Global.OrgCode", orgCode);
                DEBUG_LOG("Org Code is " << orgCode);
                emit addInstrument(name, id);
            }
            ini.SaveFile();
            transferTimerId = startTimer(TransferTimeOutNum);
            DEBUG_LOG("create ok frame, org frame seq is : " << frame->getSequence());
            res = FrameOkError::createRecvOk(frame);
        } else if (frame->getMessageType() == biotech::osla::INIT_PHASE) {
            res = FrameOkError::createRecvError(frame);
            biotech::osla::InitPhase phaseM;
            if (false == ConvertFrameObject::convertFrame2Object(frame.get(), phaseM)) {
                break;
            }

            if (phaseM.option() != biotech::osla::InitPhase_Option_SERVER_DEVICE_INIT_DONE) {
                break;
            }

            stopTimer(transferTimerId);
            emit hideMessage();
            phase = WaitingForBindingAck; // 接收仪器列表正确，切换状态
            emit changeRegisterState(phase);
            emit nextPage();
            res = FrameOkError::createRecvOk(frame);
        }
    } while (false);

    return res;
}

FrameSP SslService::UpdateWaitingForBindingAck(FrameSP frame) {
    FrameSP res;
    DEBUG_LOG("cmd type is :" << frame->getMessageType());
    do {
        if (frame->getMessageType() != biotech::osla::COMMAND) { // 收到其他消息，忽略
            break;
        }

        biotech::osla::Command command;
        DEBUG_LOG("");
        const QByteArray &message = frame->getMessageBody();
        if (!command.ParseFromArray(message.data(), message.size())) { // 消息解析错误，忽略
            break;
        }

        DEBUG_LOG("command option is : " << command.option());
        if (command.option() == biotech::osla::Command_Option_SERVER_RECV_ERR) { // 绑定失败
            ERROR_LOG("bind ack result is error");
            stopTimer(transferTimerId);
            emit connectionError(BindingError);
        } else if (command.option() == biotech::osla::Command_Option_SERVER_RECV_OK) {
            INFO_LOG("bind ack result is success");
            stopTimer(transferTimerId); // 消息接收正确，关闭超时定时器
            emit hideMessage();
            emit nextPage();
            phase = WaitingForDataDownload; // 服务器正确接收，进入接收数据状态
            emit changeRegisterState(phase);
            transferTimerId = startTimer(TransferTimeOutNum); // 再次打开超时定时器
        }
    } while (false);

    return res;
}

#define BIND_SELF_FUN(FunName) std::bind(&SslService::FunName, this, std::placeholders::_1)

FrameSP SslService::UpdateWaitingForDataDownload(FrameSP frame) {
    DEBUG_LOG("UpdateWaitingForDataDownload recv message type : " << frame->getMessageType());

    ParseDataNode updateTable[] = {
            {biotech::osla::INSTRUMENTDETAIL,    BIND_SELF_FUN(UpdateInstrumentdetail)},
            {biotech::osla::USERLIST,            BIND_SELF_FUN(UpdateUserlist)},
            {biotech::osla::ACCOUNTLIST,         BIND_SELF_FUN(UpdateAccountlist)},
            {biotech::osla::USERRIGHTLIST,       BIND_SELF_FUN(UpdateUserRightList)},
            {biotech::osla::TEMPCARDBINDINGLIST, BIND_SELF_FUN(UpdateTempCardBindingList)},
            {biotech::osla::RESERVATIONLIST,     BIND_SELF_FUN(UpdateReservationList)},
            {biotech::osla::BOOKINGLIST,         BIND_SELF_FUN(UpdateBookingList)},
            {biotech::osla::ENTRUST_LIST,        BIND_SELF_FUN(UpdateEntrustList)},
            {biotech::osla::CALENDARLIST,        BIND_SELF_FUN(UpdateCalendarList)},
            {biotech::osla::SESSIONLIST,         BIND_SELF_FUN(UpdateSessionList)},
            {biotech::osla::USERINTRAININGLIST,  BIND_SELF_FUN(UpdateUserIntrainingList)},
            {biotech::osla::USEREVENTLIST,       BIND_SELF_FUN(UpdateUserEventList)},
            {biotech::osla::SYSTEMEVENTLIST,     BIND_SELF_FUN(UpdateSystemEventList)},
            {biotech::osla::GLOBALCONFIG,        BIND_SELF_FUN(UpdateGlobalConfig)},
            {biotech::osla::TINSQUACFG_DATA,     BIND_SELF_FUN(UpdateTinsQuaConfig)},
            {biotech::osla::INIT_PHASE,          BIND_SELF_FUN(UpdateCommand)},
    };

    bool isParse = false;
    FrameSP res;
    for (int i = 0; i < sizeof(updateTable) / sizeof(updateTable[0]); ++i) {
        if (frame->getMessageType() == updateTable[i].code) {
            isParse = true;
            res = updateTable[i].fun(frame);
            break;
        }
    }

    if (false == isParse) {
        ERROR_LOG("don't support this command : " << frame->getMessageType());
        if (!res) {
            res = FrameOkError::createRecvOk();
        }
    }

    return res;
}

FrameSP SslService::UpdateWaitingForBindingConfirm(FrameSP frame) {
    if (frame->getMessageType() != biotech::osla::COMMAND) {
        return nullptr;
    }

    biotech::osla::Command command;
    const QByteArray &message = frame->getMessageBody();
    if (!command.ParseFromArray(message.data(), message.size())) {
        return nullptr;
    }
    if (command.option() != biotech::osla::Command_Option_SERVER_RECV_OK) {
        return nullptr;
    }
    stopTimer(transferTimerId);

    IniParse osla(OSLA_CONFIG);
    osla.Set("Global.Setup", 1);
    osla.SaveFile();

    DEBUG_LOG("Setup completed");
    emit downloadPageInfo(tr("Binding success"), "");//
    emit downloadPageProcess(100);
    emit nextPage();

    return nullptr;
}

FrameSP SslService::UpdateInstrumentdetail(FrameSP frame) {
    FrameSP sp = FrameOkError::createRecvError(frame);

    do {
        biotech::osla::InstrumentDetail instrumentDetail;
        if (false == ConvertFrameObject::convertFrame2Object(frame.get(), instrumentDetail)) {
            break;
        }

        stopTimer(transferTimerId);
        if (instrumentPage->selectedInstrumentId() != instrumentDetail.id()) {
            break;
        }

        IniParse osla(OSLA_CONFIG);

        osla.Set("Instrument.ID", (long) instrumentDetail.id());
        osla.Set("Instrument.Name", instrumentDetail.name());
        osla.Set("Instrument.Status", (long) instrumentDetail.status());
        osla.Set("Instrument.MinInterval", (long) instrumentDetail.min_interval());
        osla.Set("Instrument.BookingHoldingTime", (long) instrumentDetail.booking_holding_time());
        osla.Set("Instrument.BookingBreakTime", (long) instrumentDetail.booking_break_time());
        osla.Set("Instrument.AlertTime", (long) instrumentDetail.alert_time());
        osla.Set("Instrument.WorkingTime", instrumentDetail.working_time());
        DEBUG_LOG("working time is : " << instrumentDetail.working_time());
        osla.Set("Instrument.BookingModelId", instrumentDetail.bookingmodelid());
        DEBUG_LOG("booking model id is : " << instrumentDetail.bookingmodelid());

        //20200826 实现二维码获取地址的转发
        DEBUG_LOG("Instrument.TerImgUrl : " << instrumentDetail.terimgurl());
        osla.Set("Instrument.TerImgUrl",instrumentDetail.terimgurl());



        if (true == instrumentDetail.has_loosemode()) {
            osla.Set("Instrument.LooseMode", instrumentDetail.loosemode());
        } else {
            osla.Set("Instrument.LooseMode", true);
        }

        if (instrumentDetail.has_off_delay_time()) {
            osla.Set("Instrument.OffDelayTime", (long) instrumentDetail.off_delay_time());
        } else {
            osla.Del("Instrument.OffDelayTime");
        }

        if (instrumentDetail.has_min_off_time()) {
            osla.Set("Instrument.MinOffTime", (long) instrumentDetail.min_off_time());
        } else {
            osla.Del("Instrument.MinOffTime");
        }

        if (instrumentDetail.has_alert_current()) {
            osla.Set("Instrument.AlertCurrent", (long) instrumentDetail.alert_current());
        } else {
            osla.Del("Instrument.AlertCurrent");
        }

        osla.SaveFile();
        QProcess::startDetached("sync");

        emit downloadPageInfo(tr("Download configuration data..."), tr("Received instrument settings"));//下载仪器配置数据  收到仪器配置数据
        sp = FrameOkError::createRecvOk(frame);
        transferTimerId = startTimer(TransferTimeOutNum);
    } while (false);

    return sp;
}

FrameSP SslService::UpdateUserlist(FrameSP frame) {
    UploadDBTemplate(UserList, userList, updateUser,
                     tr("Download user records..."), tr("Received %1 user records").arg(userList.user_size()));
}
//  tr("下载用户记录..."), tr("收到%1条用户记录").arg(userList.user_size()));
FrameSP SslService::UpdateAccountlist(FrameSP frame) {
    UploadDBTemplate(AccountList, accountList, updateAccount,
                     tr("Download account records..."), tr("Received %1 account records").arg(accountList.accout_size()))
}
// tr("下载账户记录..."), tr("收到%1条账户记录").arg(accountList.accout_size()))
FrameSP SslService::UpdateUserRightList(FrameSP frame) {
    UploadDBTemplate(UserRightList, userRightList, updateUserRight,
                     tr("Download user right records..."), tr("Received %1 user right records").arg(userRightList.user_right_size()));
}
//  tr("下载用户资格记录..."), tr("收到%1条用户资格记录").arg(userRightList.user_right_size()));
FrameSP SslService::UpdateTempCardBindingList(FrameSP frame) {
    UploadDBTemplate(TempCardBindingList, tempCardBindingList, updateTempCardBinding,
                     tr("Download temporary card records..."), tr("Received %1 temporary card records").arg(tempCardBindingList.temp_card_binding_size()));
}
// tr("下载临时卡记录..."), tr("收到%1条临时卡记录").arg(tempCardBindingList.temp_card_binding_size()));
FrameSP SslService::UpdateReservationList(FrameSP frame) {
    UploadDBTemplate(ReservationList, reservationList, updateReservation,
                     tr("Download reservation records..."), tr("Received %1 reservation records").arg(reservationList.reservation_size()));
}
//  tr("下载专享预约记录..."), tr("收到%1条专享预约记录").arg(reservationList.reservation_size()));
FrameSP SslService::UpdateBookingList(FrameSP frame) {
    UploadDBTemplate(BookingList, bookingList, updateBooking,
                     tr("Download booking records..."), tr("Received %1 booking records").arg(bookingList.booking_size()));
}
//   tr("下载预约记录..."), tr("收到%1条预约记录").arg(bookingList.booking_size()));
FrameSP SslService::UpdateEntrustList(FrameSP frame) {
    UploadDBTemplate(EntrustList, entrustList, updateEntrust,
                     tr("Download entrust records..."), tr("Received %1 entrust records").arg(entrustList.entrust_size()));
}
//  tr("下载委托记录..."), tr("收到%1条委托记录").arg(entrustList.entrust_size()));
FrameSP SslService::UpdateCalendarList(FrameSP frame) {
    UploadDBTemplate(CalendarList, calendarList, updateCalendar,
                     tr("Download calendar records..."), tr("Received %1 calendar records").arg(calendarList.calendar_size()));
}
//   tr("下载日历数据..."), tr("收到%1条日历数据").arg(calendarList.calendar_size()));
FrameSP SslService::UpdateGlobalConfig(FrameSP frame) {
    FrameSP sp = FrameOkError::createRecvError(frame);

    do {
        biotech::osla::GlobalConfig globalConfig;
        if (false == ConvertFrameObject::convertFrame2Object(frame.get(), globalConfig)) {
            break;
        }
        stopTimer(transferTimerId);
        IniParse osla(OSLA_CONFIG);
        osla.Set("Instrument.AccountDepositLimit", (long) globalConfig.account_deposit_limit());
        //osla.Set("Instrument.ValidTrainingCheckInTime", (long) globalConfig.valid_training_check_in_time());
        //osla.Set("Instrument.WorkingTime", globalConfig.working_time());
        //osla.Set("Client.UpdateUrl", globalConfig.client_software_update_url());
        //osla.Set("Client.AutoUpdate", (long) globalConfig.client_software_auto_update());
        if (true == globalConfig.has_ftp_host()) {
            osla.Set("Ftp.enable", true);
            osla.Set("Ftp.sambapath", std::string("/storage/sd0"));
            osla.Set("Ftp.maxspeed", std::string("200000"));
            osla.Set("Ftp.name", globalConfig.ftp_user_name());
            osla.Set("Ftp.passwd", globalConfig.ftp_passwd());
            osla.Set("Ftp.rootpath", globalConfig.ftp_root_url());
            osla.Set("Ftp.host", globalConfig.ftp_host());
            osla.Set("Ftp.port", globalConfig.ftp_port());
        } else {
            osla.Set("Ftp.enable", false);
        }

        if (true == globalConfig.has_capturetime()) {
            long t = globalConfig.capturetime();
            t = (0 >= t ? 30 : t);
            osla.Set("Global.CaptureTime", t);
        } else {
            osla.Set("Global.CaptureTime", (long)30);
        }

        if (true == globalConfig.has_servicetime()) {
            long t = globalConfig.servicetime();
            t = (0 >= t ? 300 : t);
            osla.Set("Global.ServiceTime", t);
        } else {
            osla.Set("Global.ServiceTime", (long)300);
        }

        osla.SaveFile();

        IniParse client(CLIENT_CONFIG);
        client.Set("CAS.ClientAddr", 0);
        client.Set("CAS.Enabled", false);
        client.Set("Energy.ScreenDim", 60);
        client.Set("Energy.ScreenOff", 300);
        client.Set("Security.AutoCloseDialog", 15);
        client.Set("Serial.BaudRate", 19200);
        client.Set("Serial.DataBits", 8);
        client.Set("Serial.Flow", 0);
        client.Set("Serial.Parity", 0);
        client.Set("Serial.StopBits", 0);
        client.Set("Voice.Enabled", true);
        client.Set("Voice.Volume", 54);
        client.Set("Reader.Guarding", 5 * 1000);
        client.Set("Reader.SeniorUserCanLogin", true);


        client.SaveFile();

        QProcess::startDetached("sync");

        sp = FrameOkError::createRecvOk(frame);
        emit downloadPageInfo(tr("Download global configuration..."), tr("Received global configuration"));// 下载全局配置数据 收到全局配置数据
        transferTimerId = startTimer(TransferTimeOutNum);
    } while (false);

    return sp;
}

FrameSP SslService::UpdateTinsQuaConfig(FrameSP frame)
{
    FrameSP sp = FrameOkError::createRecvError(frame);

    DEBUG_LOG("tinsQuaList: received");
    do {
        biotech::osla::TinsQuaList tinsQuaList;
        if (false == ConvertFrameObject::convertFrame2Object(frame.get(), tinsQuaList)) {
            break;
        }

        IniParse osla(OSLA_CONFIG);
        UserQualification userQualification;

        for (int i = 0; i < tinsQuaList.tinsquas_size(); i++)
        {
            DEBUG_LOG("tinsQuaList: user:" << tinsQuaList.tinsquas(i).quaid() \
                      << " worktime:" << tinsQuaList.tinsquas(i).worktime() << " status:" << tinsQuaList.tinsquas(i).status());
            switch (tinsQuaList.tinsquas(i).quaid())
            {
                case biotech::osla::USER_RIGHT_UNAUTH:
                    if (tinsQuaList.tinsquas(i).worktime())
                    {
                        userQualification.setUnauthorizedUserInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                    }
                    else
                    {
                        userQualification.setUnauthorizedUserNotInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                    }
                    break;
                case biotech::osla::USER_RIGHT_NORMAL:
                    if (tinsQuaList.tinsquas(i).worktime())
                    {
                        userQualification.setNormalUserInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                    }
                    else
                    {
                        userQualification.setNormalUserNotInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                    }
                    break;
                case biotech::osla::USER_RIGHT_SENIOR:
                    if (tinsQuaList.tinsquas(i).worktime())
                    {
                        userQualification.setSeniorUserInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                    }
                    else
                    {
                        userQualification.setSeniorUserNotInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                    }
                    break;
            }
        }

        userQualification.writeUserQualification(osla);

        QProcess::startDetached("sync");

        sp = FrameOkError::createRecvOk(frame);
        emit downloadPageInfo(tr("Download user right records..."), tr("Received user right records"));//
    } while (false);

    return sp;
}

FrameSP SslService::UpdateSessionList(FrameSP frame) {
    UploadDBTemplate(SessionList, sessionList, updateSession,
                     tr("Download usage record..."), tr("Received %1 usage record").arg(sessionList.session_size()));
}
//  tr("下载使用记录..."), tr("收到%1条使用记录").arg(sessionList.session_size()));
FrameSP SslService::UpdateUserIntrainingList(FrameSP frame) {
    UploadDBTemplate(UserInTrainingList, userInTrainingList, updateUserInTraining,
                     tr("Download training check-in record..."), tr("Received %1 training check-in record").arg(userInTrainingList.user_in_training_size()));
}
//tr("下载培训签到记录..."), tr("收到%1条培训签到记录").arg(userInTrainingList.user_in_training_size()));
FrameSP SslService::UpdateUserEventList(FrameSP frame) {
    UploadDBTemplate(UserEventList, userEventList, updateUserEvent,
                     tr("Download credit card record..."), tr("Received %1 credit card record").arg(userEventList.user_event_size()));
}
//  tr("下载刷卡记录..."), tr("收到%1条刷卡记录").arg(userEventList.user_event_size()));
FrameSP SslService::UpdateSystemEventList(FrameSP frame) {
    UploadDBTemplate(SystemEventList, systemEventList, updateSystemEvent,
                     tr("Download system event record..."), tr("Received %1 system event record").arg(systemEventList.system_event_size()));
}
// tr("下载系统事件记录..."), tr("收到%1条系统事件记录").arg(systemEventList.system_event_size()));


FrameSP SslService::UpdateCommand(FrameSP frame) {
    FrameSP sp = FrameOkError::createRecvError(frame);

    do {
        biotech::osla::InitPhase initPhase;
        if (false == ConvertFrameObject::convertFrame2Object(frame.get(), initPhase)) {
            break;
        }
        if (initPhase.option() != biotech::osla::InitPhase_Option_SERVER_INIT_DONE) {
            break;
        }
        stopTimer(transferTimerId);
        sp = FrameOkError::createRecvOk();
        emit downloadPageInfo(tr("Download completed"), tr("Download completed"));//下载完成
        transferTimerId = startTimer(TransferTimeOutNum);
        phase = WaitingForBindingConfirm;
        emit changeRegisterState(phase);
        DEBUG_LOG("datadownload completed");
        /// 注意，这里需要在发送完ok之后，再去发送此消息
        sendBindingRequest(biotech::osla::InstrumentBinding_Option_CONFIRM_BINDING);
        sp = FrameOkError::createRecvOk(frame);
    } while (false);

    return sp;
}

void SslService::timerEvent(QTimerEvent *timerEvent) {
    if (timerEvent->timerId() == transferTimerId) {
        stopTimer(transferTimerId);
        switch (phase) {
            case WaitingForInstrumentList:
                sendInitRequest();
                break;

            case WaitingForBindingAck:
                sendBindingRequest(biotech::osla::InstrumentBinding_Option_REQUEST_BINDING); // 重发绑定请求
                break;

            case WaitingForDataDownload:
                emit connectionError(TimeOutError); // 超时处理
                break;

            default:
                DEBUG_LOG("timeout, phase:" << phase);
                emit connectionError(TransferTimeOut); // 超时处理
                break;
        }
    }
}

void SslService::stopTimer(int timerId) {
    if (timerId > 0) {
        killTimer(timerId);
    }
}

void SslService::onSslConnectTimerTimeout()
{
    emit connectionError(TimeOutError);
}

void SslService::setSqlWorker(SqlWorker *sqlworker) {
}

const std::string &SslService::getOrgBindCode() const {
    return orgBindCode;
}

void SslService::setOrgBindCode(const std::string &orgBindCode) {
    SslService::orgBindCode = orgBindCode;
}

void SslService::onSocketError(QAbstractSocket::SocketError err) {
    sslConnection->printSocketError(err);
    stop();
    emit connectionError(TimeOutError);
}

