//
// Created by 何继胜 on 2017/4/12.
//

#include <QtCore/QDateTime>
#include <tools/ConvertFrameObject.h>
#include "cmdparse/SyncSystemEvent.h"
#include "protobuf/message.pb.h"
#include "tools/log.h"
#include "object/RegisterOperation.h"
#include "sslconnection/Frame.h"
#include "sslconnection/ClientContext.h"
#include "sslconnection/Tools.h"

DynClassRegister(SyncSystemEvent);

SyncSystemEvent::SyncSystemEvent(std::string name) : BaseObjectExt(name) {

}

SyncSystemEvent::~SyncSystemEvent() {

}

bool SyncSystemEvent::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("Checking TSystemEvent...");
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    systemEventList = sqlWorker->getSystemEvent(time(NULL), 1);

    if (0 == systemEventList.system_event_size()) {
        return true;
    }

    DEBUG_LOG(systemEventList.system_event_size() << " unsynced records found");
    ConvertFrameObject::convertObject2Frame(dataOut, systemEventList, biotech::osla::SYSTEMEVENTLIST);

    return true;
}

bool SyncSystemEvent::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    return true;
}

bool SyncSystemEvent::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    biotech::osla::Command command;
    ConvertFrameObject::convertFrame2Object(dataIn, command);
    int upToStatus = (biotech::osla::Command_Option::Command_Option_SERVER_RECV_OK == command.option() ? 2 : 0);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    for (int i = 0; i < systemEventList.system_event_size(); ++i) {
        const biotech::osla::SystemEvent &s = systemEventList.system_event(i);
        sqlWorker->setSyncFlag("TSystemEvent", s.id(), upToStatus);
    }

    return true;
}

void SyncSystemEvent::reset() {

}
