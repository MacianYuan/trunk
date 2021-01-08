//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_SYNCFTPEVENT_H
#define OSLA_NEW_SYNCFTPEVENT_H

#include "object/BaseObject.h"
#include "protobuf/message.pb.h"

class SyncFtpEvent : public BaseObject {
public:
    SyncFtpEvent(std::string name = "SyncFtpEvent");
    ~SyncFtpEvent();

    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject* dataIn, BaseObject* dataOut);
    bool parseRespose(BaseObject* dataIn, BaseObject* dataOut);
    void reset();

private:
    biotech::osla::FtpEventList ftpEventList;
};

#endif //OSLA_NEW_SYNCFTPEVENT_H
