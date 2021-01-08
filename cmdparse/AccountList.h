//
// Created by 何继胜 on 2017/4/12.
//

#ifndef OSLA_NEW_ACCOUNTLIST_H
#define OSLA_NEW_ACCOUNTLIST_H

#include "object/BaseObjectExt.h"

class AccountList : public BaseObjectExt {
public:
    AccountList(std::string name = "AccountList");
    ~AccountList();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();
};


#endif //OSLA_NEW_ACCOUNTLIST_H
