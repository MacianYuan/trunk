//
// Created by 何继胜 on 2017/4/12.
//

#ifndef OSLA_NEW_HEARTBEAT_H
#define OSLA_NEW_HEARTBEAT_H

#include "object/BaseObject.h"
#include "protobuf/message.pb.h"

class HeartBeat : public BaseObject {
public:
    HeartBeat(std::string name = "HeartBeat");
    ~HeartBeat();

    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject* dataIn, BaseObject* dataOut);
    bool parseRespose(BaseObject* dataIn, BaseObject* dataOut);
    void reset();

private:
    biotech::osla::Heartbeat heartbeat;
};


#endif //OSLA_NEW_HEARTBEAT_H
