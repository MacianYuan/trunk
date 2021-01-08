#include <functional>
#include <memory>
#include <QtCore/QtCore>

#include <object/RegisterObject.h>
#include <tools/ConvertFrameObject.h>
#include <sqlworker/sqlworker.h>
#include <deviceinfo/version.h>
#include <tools/IniParse.h>
#include <tools/log.h>
#include <tools/PublicDefine.h>
#include <sslconnection/Tools.h>
#include <sslconnection/ClientContext.h>
#include <sslconnection/SslConnection.h>
#include <controller/Base/GlobalData.h>

#include "controller/Service/SslService.h"

//最大连接超时时间
#define CONNECT_TIMEOUT       30000

SslService::SslService(QObject *parent) : QObject(parent) {
    context = new ClientContext();

    SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
    sqlWorker->createTables();

    sqlWorker->convertUnsyncRecordStatus();

    IniParse ini(OSLA_CONFIG);
    QString sslCert(ini.GetString("Network.SslCert").c_str());
    sslConnection = new SslConnection(this);
    sslConnection->setPeerVerifyMode(QSslSocket::QueryPeer);
    sslConnection->setLocalCertificate(sslCert);
    sslConnection->setPrivateKey(sslCert);

    //connect(sqlWorker, SIGNAL(adminUpdated()), this, SIGNAL(settingsUpdated()));
    connect(&clockTimer, SIGNAL(timeout()), this, SLOT(doRegularTask()));
    connect(sslConnection, SIGNAL(connected()), this, SLOT(doConnected()));
    connect(sslConnection, SIGNAL(disconnected()), this, SLOT(doDisconnected()));
    connect(sslConnection, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(sslConnection, SIGNAL(newMessage(FrameSP)), this, SLOT(parseFrame(FrameSP)));

    connectTimer.setInterval(CONNECT_TIMEOUT);
    connectTimer.setSingleShot(true);
    connect(&connectTimer, SIGNAL(timeout()), this, SLOT(onConnectTimeout()));


    clockTimer.setInterval(1000);

    context->setInstrumentId(ini.GetLong("Instrument.ID"));
    //context->setSqlWorker(sqlWorker);
    context->setSslService(this);
}

SslService::~SslService() {
    context->setRestartEnabled(false);
    clearSendMessage();
    ERROR_LOG("SslService: Stopped");
    SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
    if (nullptr != sqlWorker) {
        sqlWorker->closeDatabase();
    }
    google::protobuf::ShutdownProtobufLibrary(); // 关闭google protobuf
    ERROR_LOG("SslService: Destructed");
}

void SslService::start() {
    //std::lock_guard<std::mutex> guard(m_ConnectionMutex);
    IniParse ini(OSLA_CONFIG);
    QString sslHost(ini.GetString("Network.SslHost").c_str());
    quint16 sslPort = ini.GetLong("Network.SslPort");
    clearSendMessage();
    sslConnection->connectToHostEncrypted(sslHost, sslPort);
    WARN_LOG("SslService: linkerr, Connecting to server" << QString("%1:%2").arg(sslHost).arg(sslPort).toLocal8Bit().data());
    context->setRestartEnabled(true);
    connectTimer.start();
}

//void SslService::stop() {
//    context->setRestartEnabled(false);
//    clearSendMessage();
//    ERROR_LOG("SslService: Stopped");
//}

void SslService::reset() {
    //std::lock_guard<std::mutex> guard(m_ConnectionMutex);
    ERROR_LOG("SslService: Reset");
    clockTimer.stop();
    sslConnection->resetConnection();
    clearSendMessage();
    if (true == context->isConnected()) {
        emit networkStateChanged(false);
    }
    context->setConnected(false);
    QTimer::singleShot(PublicDefine::getReconnectInterval() * 1000, this, SLOT(start()));
}

void SslService::doConnected() {
    /// 通知界面，网络连接上
    ERROR_LOG("SslService: linkerr, Server connected");

    if (false == context->isConnected()) {
        emit networkStateChanged(true);
    }
    context->setConnected(true);
    context->setClientState(-1);
    connectTimer.stop();
    clockTimer.start();
    offlineTime.start();
    auto ptr = RegisterObject::GetInstance().CreateObject("HeartBeat");
    ptr->setContext(dynamic_cast<BaseObject *>(context));
    auto frame = std::make_shared<Frame>();
    ptr->createRequest(nullptr, frame.get());
    if (0 > sslConnection->sendMessage(frame)) {
        ERROR_LOG("linkerr, send data to server failed");
    }
}

void SslService::doDisconnected() {
    if (false == context->getRestartEnabled()) {
        return;
    }
    ERROR_LOG("SslService: linkerr, Restarting...");
    this->reset();
    ERROR_LOG("SslService: linkerr, Reconnecting in" << PublicDefine::getReconnectInterval() << "seconds...");
}

void SslService::parseFrame(FrameSP frame) {
    struct _TempNode_ {
        int messageType;
        std::string messageParseName;
    } arr[] = {
            {biotech::osla::HEARTBEAT,                 "HeartBeat"},
            {biotech::osla::ENTRUST_LIST,              "EntrustList"},
            {biotech::osla::USERLIST,                  "UserList"},
            {biotech::osla::ACCOUNTLIST,               "AccountList"},
            {biotech::osla::USERRIGHTLIST,             "UserRightList"},
            {biotech::osla::CALENDARLIST,              "CalendarList"},
            {biotech::osla::TEMPCARDBINDINGLIST,       "TempCardBindingList"},
            {biotech::osla::INSTRUMENTDETAIL,          "InstrumentDetail"},
            {biotech::osla::BOOKINGLIST,               "BookingList"},
            {biotech::osla::RESERVATIONLIST,           "ReservationList"},
            {biotech::osla::GLOBALCONFIG,              "GlobalConfig"},
            {biotech::osla::GLOBALCONFIG,              "GlobalConfig"},
            {biotech::osla::SESSION_OPERATION_REQ_CMD, "SessionOperation"},
            {biotech::osla::UPGRADE_PARAMS,            "UpgradeParams"},
            {biotech::osla::UPGRADE_PLAN,              "UpgradePlan"},
            {biotech::osla::TINSQUACFG_DATA,           "TinsQuaList"}
    };

    offlineTime.restart();
    bool status = false;

    if (biotech::osla::COMMAND == frame->getMessageType()) {
        std::lock_guard<std::mutex> guard(m_SendMessageMutex);
        auto iter = m_SendMessage.find(frame->getSequence());
        if (iter != m_SendMessage.end()) {
            SendNode &node = iter->second;
            status = node.parse->parseRespose(frame.get(), nullptr);
            m_SendMessage.erase(iter);
        }
        DEBUG_LOG("now send wait reponse frame count num is : " << m_SendMessage.size());
    } else {
        for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i) {
            if (frame->getMessageType() != arr[i].messageType) {
                continue;
            }
            auto ptr = RegisterObject::GetInstance().CreateObject(arr[i].messageParseName);
            if (!ptr) {
                ERROR_LOG("create object failed[ID:" << frame->getMessageType()
                                                     << ", Name" << arr[i].messageParseName << "]!!!");
                break;
            }
            ptr->setContext(context);
            auto reponseFrame = std::make_shared<Frame>();
            status = ptr->parseRequset(frame.get(), reponseFrame.get());
            if (true == status && reponseFrame->getNeedSend().length() > 0) {
                sslConnection->sendMessage(reponseFrame);
            }
            RegisterObject::GetInstance().ReleaseObject(ptr);
            break;
        }
    }

    DEBUG_LOG("parse cmd[" << frame->getMessageType() << "] result is " << (true == status ? "ok" : "failed"));
}

void SslService::checkNeedResendMessage() {
    time_t now = time(nullptr);

    std::lock_guard<std::mutex> guard(m_SendMessageMutex);
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

inline void SslService::stopTimer(int timerId) {
    if (timerId > 0) {
        killTimer(timerId);
    }
}

void SslService::doRegularTask() {
    static int heartbeatTimerCount = 0;

    // 判断是否离线
    if (offlineTime.elapsed() > PublicDefine::getOfflineTimeOut() * 1000) {
        ERROR_LOG("SslService: linkerr, Connection timeout");
        heartbeatTimerCount = 0;
        reset();
        return;
    }

    // 发送心跳包
    if (++heartbeatTimerCount == Tools::HeartbeatInterval) {
        auto ptr = RegisterObject::GetInstance().CreateObject("HeartBeat");
        ptr->setContext(dynamic_cast<BaseObject *>(context));
        auto frame = std::make_shared<Frame>();
        ptr->createRequest(nullptr, frame.get());
        if (0 > sslConnection->sendMessage(frame)) {
            ERROR_LOG("linkerr,send data to server failed");
        }
        else
        {
            DEBUG_LOG("linkerr, send heartBeat");
        }
        heartbeatTimerCount = 0;
    }

    /// 检查需要重新发送的包
    checkNeedResendMessage();

    /// 发送使用记录信息
    static const char *syncServiceName[] = {"SyncFtpEvent", "SyncSession", "SyncDeviceCaptureEvent",
                                            "SyncSystemEvent", "SyncUserEvent", "SyncUserInTraining",
                                            "SyncEntrustEvent"};
    for (int j = 0; j < sizeof(syncServiceName) / sizeof(syncServiceName[0]); ++j) {
        auto ptr = RegisterObject::GetInstance().CreateObject(syncServiceName[j]);
        if (!ptr) {
            ERROR_LOG("create object[" << syncServiceName[j] << "] failed");
            continue;
        }
        ptr->setContext(dynamic_cast<BaseObject *>(context));
        auto frame = std::make_shared<Frame>();
        ptr->createRequest(nullptr, frame.get());
        if (frame->getMessageBody().length() > 0) {
            if (0 > sslConnection->sendMessage(frame)) {
                ERROR_LOG("send data to server failed");
            }
            /// 将frame存储起来，等待响应命令
            std::lock_guard<std::mutex> guard(m_SendMessageMutex);
            m_SendMessage.insert(std::make_pair(frame->getSequence(), SendNode(frame->getSequence(), ptr, frame)));
        } else {
            RegisterObject::GetInstance().ReleaseObject(ptr);
        }
    }
}

void SslService::setClientState(int state) {
    context->setClientState(state);
    ERROR_LOG("SslService: Client state " << QString::number(state, 16).toUpper().toLocal8Bit().data());
}

void SslService::sendUnbindingRequest() {
    DEBUG_LOG(__FUNCTION__);
    if (false == context->isConnected()) { //// 还需要查看是否还有记录未同步？如果有，不可以解绑
        emit bindingReleased(false);
        ERROR_LOG("SslService: Failed to release instrument binding");
        return;
    }
    DEBUG_LOG(__FUNCTION__);

    auto ptr = RegisterObject::GetInstance().CreateObject("Unbinding");
    ptr->setContext(dynamic_cast<BaseObject *>(context));
    auto frame = std::make_shared<Frame>();
    DEBUG_LOG(__FUNCTION__);
    ptr->createRequest(nullptr, frame.get());
    DEBUG_LOG(__FUNCTION__);
    if (0 > sslConnection->sendMessage(frame)) {
        DEBUG_LOG(__FUNCTION__);
        ERROR_LOG("send data to server failed");
    }
    DEBUG_LOG(__FUNCTION__);
    std::lock_guard<std::mutex> guard(m_SendMessageMutex);
    m_SendMessage.insert(std::make_pair(frame->getSequence(), SendNode(frame->getSequence(), ptr, frame)));
}

void SslService::sendDetectedFromNetResp(int id, int status, int bookid) {
    biotech::osla::SessionOpertaionRep sorep;
    sorep.set_id(id);
    sorep.set_status(status);
    sorep.set_bookingid(bookid);
    ERROR_LOG("id is " << id << ", shutdwon status is : " << status << ",booking id is:" << bookid);

    FrameSP frame = std::make_shared<Frame>();
    ConvertFrameObject::convertObject2Frame(frame.get(), sorep, biotech::osla::SESSION_OPERATION_REP_CMD);
}

void SslService::EmitBookingListUpdate() {
    emit bookingUpdated();
}

void SslService::EmitSettingsUpdated() {
    emit settingsUpdated();
}

void SslService::EmitReservationUpdated() {
    emit reservationUpdated();
}

void SslService::EmitBindingReleased(bool status) {
    DEBUG_LOG(__FUNCTION__);
    emit bindingReleased(status);
}

void SslService::EmitUpgradeParamsUpdated()
{
    emit upgradeParamsUpdated();
}

void SslService::EmitUpgradePlanUpdated()
{
    emit upgradePlanUpdated();
}

void SslService::clearSendMessage() {
    biotech::osla::Command command;
    command.set_option(biotech::osla::Command_Option::Command_Option_CLIENT_RECV_ERR);
    auto frameIn = std::make_shared<Frame>();
    ConvertFrameObject::convertObject2Frame(frameIn.get(), command, biotech::osla::COMMAND);

    std::lock_guard<std::mutex> guard(m_SendMessageMutex);
    for (auto iter = m_SendMessage.begin(); iter != m_SendMessage.end(); ++iter) {
        SendNode &node = iter->second;
        node.parse->parseRespose(frameIn.get(), nullptr);
        RegisterObject::GetInstance().ReleaseObject(node.parse);
    }
    m_SendMessage.clear();
}

void SslService::EmitCardDetectedFromNet(int id, int userid, uint cardId, int userrole, int op) {
    emit cardDetectedFromNet(id, userid, cardId, userrole, op);
}

void SslService::onSocketError(QAbstractSocket::SocketError err) {
    sslConnection->printSocketError(err);
    DEBUG_LOG("linkerr, socket error");
    reset();
}

void SslService::onConnectTimeout()
{
    DEBUG_LOG("linkerr, socket connect timeout");
    reset();
}

