//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_INSTRUMENTDETAIL_H
#define OSLA_NEW_INSTRUMENTDETAIL_H

#include "object/BaseObjectExt.h"

class InstrumentDetail : public BaseObjectExt {
public:
    InstrumentDetail(std::string name = "InstrumentDetail");
    ~InstrumentDetail();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

};


#endif //OSLA_NEW_INSTRUMENTDETAIL_H
