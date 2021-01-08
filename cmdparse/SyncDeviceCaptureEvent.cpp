//
// Created by 何继胜 on 2017/4/12.
//

#include <QtCore/QDateTime>
#include <tools/ConvertFrameObject.h>
#include "cmdparse/SyncDeviceCaptureEvent.h"
#include "protobuf/message.pb.h"
#include "tools/log.h"
#include "object/RegisterOperation.h"
#include "sslconnection/Frame.h"
#include "sslconnection/ClientContext.h"
#include "sslconnection/Tools.h"

DynClassRegister(SyncDeviceCaptureEvent);

SyncDeviceCaptureEvent::SyncDeviceCaptureEvent(std::string name) : BaseObject(name) {

}

SyncDeviceCaptureEvent::~SyncDeviceCaptureEvent() {

}

bool SyncDeviceCaptureEvent::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("Checking DeviceCaptureEven...");
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    SqlWorker* sqlworker = SqlWorker::GetSqlWorkByThread();
    DEBUG_LOG(__FUNCTION__ << "sql worker is " << sqlworker);
    deviceCaptureEventList.Clear();
    deviceCaptureEventList = sqlworker->getDeviceCaptureEvent(time(NULL), 1);
    int listSize = deviceCaptureEventList.device_capture_event_size();
    if (listSize == 0) {
        DEBUG_LOG("SslService: No unsynced record");
        return true;
    }
    DEBUG_LOG(listSize << " unsynced records found");
    ConvertFrameObject::convertObject2Frame(dataOut, deviceCaptureEventList, biotech::osla::DEVICE_CAPTURE_EVENT_LIST);
    return true;
}

bool SyncDeviceCaptureEvent::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    return parseRespose(dataIn, dataOut);
}

bool SyncDeviceCaptureEvent::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    biotech::osla::Command command;
    ConvertFrameObject::convertFrame2Object(dataIn, command);
    int upToStatus = (biotech::osla::Command_Option::Command_Option_SERVER_RECV_OK == command.option() ? 2 : 0);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    for (int i = 0; i < deviceCaptureEventList.device_capture_event_size(); ++i) {
        const biotech::osla::DeviceCaptureEvent &s = deviceCaptureEventList.device_capture_event(i);
        if (2 == upToStatus) {
            sqlWorker->delSyncFlag("TDeviceCaptureEvent", s.id());
        } else {
            sqlWorker->setSyncFlag("TDeviceCaptureEvent", s.id(), upToStatus);
        }
    }

    return true;
}

void SyncDeviceCaptureEvent::reset() {

}
