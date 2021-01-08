//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_BOOKLIST_H
#define OSLA_NEW_BOOKLIST_H


#include "object/BaseObjectExt.h"


class BookingList : public BaseObjectExt {
public:
    BookingList(std::string name = "BookingList");
    ~BookingList();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

};


#endif //OSLA_NEW_BOOKLIST_H
