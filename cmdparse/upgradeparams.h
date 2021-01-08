#ifndef UPGRADEPARAMS_H
#define UPGRADEPARAMS_H

#include "object/BaseObjectExt.h"

class UpgradeParams : public BaseObjectExt {
public:
    UpgradeParams(std::string name = "UpgradeParams");
    ~UpgradeParams();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();
};

#endif // UPGRADEPARAMS_H
