//
// Created by 何继胜 on 2017/5/14.
//

#ifndef OSLA_NEW_ENTRUSTLIST_H
#define OSLA_NEW_ENTRUSTLIST_H

#include "object/BaseObjectExt.h"

class EntrustList : public BaseObjectExt {
public:
    EntrustList(std::string name = "EntrustList");
    ~EntrustList();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

};



#endif //OSLA_NEW_ENTRUSTLIST_H
