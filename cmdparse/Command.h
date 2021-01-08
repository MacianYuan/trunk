//
// Created by 何继胜 on 2017/4/25.
//

#ifndef OSLA_NEW_COMMAND_H
#define OSLA_NEW_COMMAND_H

#include "object/BaseObjectExt.h"

class Command : public BaseObjectExt {
public:
    Command(std::string name = "Command");
    ~Command();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();
};
#endif //OSLA_NEW_COMMAND_H
