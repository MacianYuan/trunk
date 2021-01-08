//
// Created by 何继胜 on 2017/4/12.
//

#include <QtCore/QDateTime>
#include <tools/ConvertFrameObject.h>
#include "cmdparse/SyncSession.h"
#include "protobuf/message.pb.h"
#include "tools/log.h"
#include "object/RegisterOperation.h"
#include "sslconnection/Frame.h"
#include "sslconnection/ClientContext.h"
#include "sslconnection/Tools.h"

DynClassRegister(SyncSession);

SyncSession::SyncSession(std::string name) : BaseObjectExt(name) {

}

SyncSession::~SyncSession() {

}

bool SyncSession::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    ERROR_LOG("Checking TSession...");
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    sessionList = sqlWorker->getSession(time(NULL), 1);

    if (0 == sessionList.session_size()) {
        return true;
    }
    DEBUG_LOG(sessionList.session_size() << " unsynced records found");
    ConvertFrameObject::convertObject2Frame(dataOut, sessionList, biotech::osla::SESSIONLIST);

    return true;
}

bool SyncSession::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    return true;
}

bool SyncSession::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    biotech::osla::Command command;
    ConvertFrameObject::convertFrame2Object(dataIn, command);
    int upToStatus = (biotech::osla::Command_Option::Command_Option_SERVER_RECV_OK == command.option() ? 2 : 0);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    for (int i = 0; i < sessionList.session_size(); ++i) {
        const biotech::osla::Session &s = sessionList.session(i);
        sqlWorker->setSyncFlag("TSession", s.id(), upToStatus);
    }

    return true;
}

void SyncSession::reset() {

}
