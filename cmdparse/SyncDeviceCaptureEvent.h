//
// Created by 何继胜 on 2017/5/11.
//

#ifndef OSLA_NEW_SYNCDEVICECAPTUREEVENT_H
#define OSLA_NEW_SYNCDEVICECAPTUREEVENT_H

#include "object/BaseObject.h"
#include "protobuf/message.pb.h"

class SyncDeviceCaptureEvent : public BaseObject {
public:
    SyncDeviceCaptureEvent(std::string name = "SyncDeviceCaptureEvent");
    ~SyncDeviceCaptureEvent();

    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject* dataIn, BaseObject* dataOut);
    bool parseRespose(BaseObject* dataIn, BaseObject* dataOut);
    void reset();

private:
    biotech::osla::DeviceCaptureEventList deviceCaptureEventList;
};


#endif //OSLA_NEW_SYNCDEVICECAPTUREEVENT_H
