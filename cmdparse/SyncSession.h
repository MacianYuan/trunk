//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_SYNCSESSION_H
#define OSLA_NEW_SYNCSESSION_H


#include <protobuf/message.pb.h>
#include "object/BaseObjectExt.h"

class SyncSession : public BaseObjectExt {
public:
    SyncSession(std::string name = "SyncSession");
    ~SyncSession();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

private:
    biotech::osla::SessionList sessionList;
};


#endif //OSLA_NEW_SYNCSESSION_H
