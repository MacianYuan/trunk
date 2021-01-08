//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_GLOBALCONFIG_H
#define OSLA_NEW_GLOBALCONFIG_H

#include "object/BaseObjectExt.h"
#include "IniParse.h"

class GlobalConfig : public BaseObjectExt {
public:
    GlobalConfig(std::string name = "GlobalConfig");
    ~GlobalConfig();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

private:
    void parseWorkingTime(QString workingTime, IniParse &osla);
};


#endif //OSLA_NEW_GLOBALCONFIG_H
