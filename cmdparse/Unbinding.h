//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_UNBINDING_H
#define OSLA_NEW_UNBINDING_H

#include "object/BaseObject.h"
#include "protobuf/message.pb.h"

class Unbinding : public BaseObject {
public:
    Unbinding(std::string name = "Unbinding");
    ~Unbinding();

    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject* dataIn, BaseObject* dataOut);
    bool parseRespose(BaseObject* dataIn, BaseObject* dataOut);
    void reset();

};



#endif //OSLA_NEW_UNBINDING_H
