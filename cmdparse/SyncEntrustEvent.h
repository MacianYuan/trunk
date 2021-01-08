//
// Created by 何继胜 on 2017/5/15.
//

#ifndef OSLA_NEW_SYNCENTRUSTEVENT_H
#define OSLA_NEW_SYNCENTRUSTEVENT_H

#include <protobuf/message.pb.h>
#include "object/BaseObjectExt.h"

class SyncEntrustEvent : public BaseObjectExt {
public:
    SyncEntrustEvent(std::string name = "SyncEntrustEvent");
    ~SyncEntrustEvent();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

private:
    biotech::osla::EntrustEventList entrustEventList;
};


#endif //OSLA_NEW_SYNCENTRUSTEVENT_H
