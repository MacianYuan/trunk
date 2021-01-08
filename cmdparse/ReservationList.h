//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_RESERVATIONLIST_H
#define OSLA_NEW_RESERVATIONLIST_H


#include "object/BaseObjectExt.h"


class ReservationList : public BaseObjectExt {
public:
    ReservationList(std::string name = "ReservationList");
    ~ReservationList();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

};


#endif //OSLA_NEW_RESERVATIONLIST_H
