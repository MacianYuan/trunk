#ifndef UPGRADEPLAN_H
#define UPGRADEPLAN_H

#include "object/BaseObjectExt.h"

class UpgradePlan : public BaseObjectExt {
public:
    UpgradePlan(std::string name = "UpgradePlan");
    ~UpgradePlan();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();
};

#endif // UPGRADEPLAN_H
