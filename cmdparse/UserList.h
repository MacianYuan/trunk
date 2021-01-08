//
// Created by 何继胜 on 2017/4/12.
//

#ifndef OSLA_NEW_USERLIST_H
#define OSLA_NEW_USERLIST_H

#include "object/BaseObjectExt.h"


class UserList : public BaseObjectExt {
public:
    UserList(std::string name = "UserList");
    ~UserList();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

};


#endif //OSLA_NEW_USERLIST_H
