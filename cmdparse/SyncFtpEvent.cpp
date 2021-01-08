//
// Created by 何继胜 on 2017/4/12.
//

#include <QtCore/QDateTime>
#include <tools/ConvertFrameObject.h>
#include "cmdparse/SyncFtpEvent.h"
#include "protobuf/message.pb.h"
#include "tools/log.h"
#include "object/RegisterOperation.h"
#include "sslconnection/Frame.h"
#include "sslconnection/ClientContext.h"
#include "sslconnection/Tools.h"

DynClassRegister(SyncFtpEvent);

SyncFtpEvent::SyncFtpEvent(std::string name) : BaseObject(name) {

}

SyncFtpEvent::~SyncFtpEvent() {

}

bool SyncFtpEvent::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("Checking TFtpEvent...");
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    ftpEventList.Clear();
    ftpEventList = sqlWorker->getFtpEvent(time(NULL), 1);
    int listSize = ftpEventList.ftp_event_size();
    if (listSize == 0) {
        return true;
    }
    DEBUG_LOG(listSize << " unsynced records found");
    ConvertFrameObject::convertObject2Frame(dataOut, ftpEventList, biotech::osla::FTPEVENTLIST);
    return true;
}

bool SyncFtpEvent::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    return parseRespose(dataIn, dataOut);
}

bool SyncFtpEvent::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("Update TFtpEvent...");
    biotech::osla::Command command;
    ConvertFrameObject::convertFrame2Object(dataIn, command);
    int upToStatus = (biotech::osla::Command_Option::Command_Option_SERVER_RECV_OK == command.option() ? 2 : 0);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    for (int i = 0; i < ftpEventList.ftp_event_size(); ++i) {
        const biotech::osla::FtpEvent &s = ftpEventList.ftp_event(i);
        for (int j = 0; j < s.file_size(); ++j) {
            const biotech::osla::FtpFileUpload& u = s.file(j);
            sqlWorker->setSyncFlag("TFtpEvent", u.localid(), upToStatus);
        }
    }

    return true;
}

void SyncFtpEvent::reset() {

}
