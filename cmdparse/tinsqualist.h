#ifndef TINSQUALIST_H
#define TINSQUALIST_H

#include "object/BaseObjectExt.h"
#include <protobuf/message.pb.h>
#include "../controller/Context/userqualification.h"

class TinsQuaList : public BaseObjectExt {
public:
    TinsQuaList(std::string name = "TinsQuaList");
    ~TinsQuaList();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();
private:
    bool updateQuaList(const biotech::osla::TinsQuaList &tinsQuaList);
private:
    UserQualification userQualification;
};


#endif // TINSQUALIST_H
