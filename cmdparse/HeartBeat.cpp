//
// Created by 何继胜 on 2017/4/12.
//

#include <QtCore/QDateTime>
#include <deviceinfo/version.h>
#include <tools/IniParse.h>
#include "cmdparse/HeartBeat.h"
#include "protobuf/message.pb.h"
#include "tools/log.h"
#include "object/RegisterOperation.h"
#include "sslconnection/Frame.h"
#include "sslconnection/ClientContext.h"
#include "sslconnection/Tools.h"

DynClassRegister(HeartBeat);

HeartBeat::HeartBeat(std::string name) : BaseObject(name) {

}

HeartBeat::~HeartBeat() {

}

bool HeartBeat::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("send time heart beat : " << QDateTime::currentDateTime().toString().toLocal8Bit().data());
    Frame *frame = dynamic_cast<Frame *>(dataOut);
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);

    static std::string orgCode = "";
    if (orgCode.length() <= 0) {
        IniParse ini(OSLA_CONFIG);
        orgCode = ini.GetString("Global.OrgCode", "");
        DEBUG_LOG("Org Code is " << orgCode);
    }

    heartbeat.set_instrument_id(context->getInstrumentId());
    heartbeat.set_client_timestamp(QDateTime::currentDateTime().toTime_t());
    heartbeat.set_server_timestamp(0);
    DEBUG_LOG("getClientState : " << context->getClientState());
    heartbeat.set_client_state(context->getClientState());
    heartbeat.set_org_code(orgCode);

    QByteArray heartbeatMessage;
    heartbeatMessage.resize(heartbeat.ByteSize());
    heartbeat.SerializeToArray(heartbeatMessage.data(), heartbeat.ByteSize());
    frame->EncodeFrame(biotech::osla::HEARTBEAT, heartbeatMessage);

    return true;
}

bool HeartBeat::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    return parseRespose(dataIn, dataOut);
}

bool HeartBeat::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    Frame *frameIn = dynamic_cast<Frame *>(dataIn);
    const QByteArray &messageBody = frameIn->getMessageBody();
    if (!heartbeat.ParseFromArray(messageBody.data(), messageBody.size())) {
        DEBUG_LOG("linkerr, recv heartBeat err");
        return false;
    }

    if (!(heartbeat.has_client_timestamp() && heartbeat.has_server_timestamp())) {
        DEBUG_LOG("linkerr, recv heartBeat err");
        return false;
    }

    uint lastTimestamp = (uint) heartbeat.client_timestamp();
    uint currentTimestamp = QDateTime::currentDateTime().toTime_t();
    int rtt = currentTimestamp - lastTimestamp;
    if (rtt > Tools::MaxRoundTripTime || rtt < 0) {
        context->setNetworkStable(false);
        int n1 = heartbeat.server_timestamp() - heartbeat.client_timestamp();
        int n2 = currentTimestamp - heartbeat.server_timestamp();
        DEBUG_LOG("Connection unreliable client to server: " << n1 << "seconds");
        DEBUG_LOG("Connection unreliable server to client: " << n2 << "seconds");
        DEBUG_LOG("Connection unreliable: RTT " << rtt << "seconds");
        WARN_LOG("Connection stop sync");
    } else {
        context->setNetworkStable(true);
    }

    DEBUG_LOG("linkerr, recv heartBeat ok");
    return true;
}

void HeartBeat::reset() {

}
