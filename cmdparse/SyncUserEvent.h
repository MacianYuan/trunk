//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_SYNCUSEREVENT_H
#define OSLA_NEW_SYNCUSEREVENT_H

#include <protobuf/message.pb.h>
#include "object/BaseObjectExt.h"

class SyncUserEvent : public BaseObjectExt {
public:
    SyncUserEvent(std::string name = "SyncUserEvent");
    ~SyncUserEvent();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

private:
    biotech::osla::UserEventList userEventList;
};




#endif //OSLA_NEW_SYNCUSEREVENT_H
