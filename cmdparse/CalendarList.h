//
// Created by 何继胜 on 2017/4/12.
//

#ifndef OSLA_NEW_CALENDARLIST_H
#define OSLA_NEW_CALENDARLIST_H
#include "object/BaseObjectExt.h"

class CalendarList : public BaseObjectExt {
public:
    CalendarList(std::string name = "CalendarList");
    ~CalendarList();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();
};


#endif //OSLA_NEW_CALENDARLIST_H
