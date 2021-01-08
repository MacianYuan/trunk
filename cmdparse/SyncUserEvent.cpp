//
// Created by 何继胜 on 2017/4/12.
//

#include <QtCore/QDateTime>
#include <tools/ConvertFrameObject.h>
#include "cmdparse/SyncUserEvent.h"
#include "protobuf/message.pb.h"
#include "tools/log.h"
#include "object/RegisterOperation.h"
#include "sslconnection/Frame.h"
#include "sslconnection/ClientContext.h"
#include "sslconnection/Tools.h"
#include <QDebug>

DynClassRegister(SyncUserEvent);

SyncUserEvent::SyncUserEvent(std::string name) : BaseObjectExt(name) {

}

SyncUserEvent::~SyncUserEvent() {

}

bool SyncUserEvent::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("Checking TUserEvent...");
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    userEventList = sqlWorker->getUserEvent(time(NULL), 1);

    if (0 == userEventList.user_event_size()) {
        return true;
    }

    DEBUG_LOG(userEventList.user_event_size() << " unsynced records found");
    ConvertFrameObject::convertObject2Frame(dataOut, userEventList, biotech::osla::USEREVENTLIST);

    return true;
}

bool SyncUserEvent::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    return true;
}

bool SyncUserEvent::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("");
    biotech::osla::Command command;
    ConvertFrameObject::convertFrame2Object(dataIn, command);
    int upToStatus = (biotech::osla::Command_Option::Command_Option_SERVER_RECV_OK == command.option() ? 2 : 0);
    DEBUG_LOG("up To Status is : " << upToStatus);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    for (int i = 0; i < userEventList.user_event_size(); ++i) {
        const biotech::osla::UserEvent &s = userEventList.user_event(i);
        sqlWorker->setSyncFlag("TUserEvent", s.id(), upToStatus);
    }

    return true;
}

void SyncUserEvent::reset() {

}
