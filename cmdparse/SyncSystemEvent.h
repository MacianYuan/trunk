//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_SYNCSYSTEMEVENT_H
#define OSLA_NEW_SYNCSYSTEMEVENT_H

#include <protobuf/message.pb.h>
#include "object/BaseObjectExt.h"

class SyncSystemEvent : public BaseObjectExt {
public:
    SyncSystemEvent(std::string name = "SyncSystemEvent");
    ~SyncSystemEvent();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

private:
    biotech::osla::SystemEventList systemEventList;
};


#endif //OSLA_NEW_SYNCSYSTEMEVENT_H
