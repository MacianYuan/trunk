//
// Created by 何继胜 on 2017/4/12.
//

#include <QtCore/QDateTime>
#include <tools/ConvertFrameObject.h>
#include "cmdparse/SyncEntrustEvent.h"
#include "protobuf/message.pb.h"
#include "tools/log.h"
#include "object/RegisterOperation.h"
#include "sslconnection/Frame.h"
#include "sslconnection/ClientContext.h"
#include "sslconnection/Tools.h"

DynClassRegister(SyncEntrustEvent);

SyncEntrustEvent::SyncEntrustEvent(std::string name) : BaseObjectExt(name) {

}

SyncEntrustEvent::~SyncEntrustEvent() {

}

bool SyncEntrustEvent::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("Checking EntrustEvent...");
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    entrustEventList = sqlWorker->getEntrustEvent(time(NULL), 1);
    if (0 == entrustEventList.entrust_event_size()) {
        return true;
    }
    DEBUG_LOG("SslService:" << __FUNCTION__ << ":" << entrustEventList.entrust_event_size() << " unsynced records found");
    ConvertFrameObject::convertObject2Frame(dataOut, entrustEventList, biotech::osla::ENTRUST_EVENT_LIST);
    return true;
}

bool SyncEntrustEvent::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    return true;
}

bool SyncEntrustEvent::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    biotech::osla::Command command;
    ConvertFrameObject::convertFrame2Object(dataIn, command);
    int upToStatus = (biotech::osla::Command_Option::Command_Option_SERVER_RECV_OK == command.option() ? 2 : 0);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    for (int i = 0; i < entrustEventList.entrust_event_size(); ++i) {
        const biotech::osla::EntrustEvent &s = entrustEventList.entrust_event(i);
        sqlWorker->setSyncFlag("TBookingEntrustEvent", s.id(), upToStatus);
    }

    return true;
}

void SyncEntrustEvent::reset() {

}
