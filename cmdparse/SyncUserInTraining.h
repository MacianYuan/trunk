//
// Created by 何继胜 on 2017/4/24.
//

#ifndef OSLA_NEW_SYNCUSERINTRAINING_H
#define OSLA_NEW_SYNCUSERINTRAINING_H

#include <protobuf/message.pb.h>
#include "object/BaseObjectExt.h"

class SyncUserInTraining : public BaseObjectExt {
public:
    SyncUserInTraining(std::string name = "SyncUserInTraining");
    ~SyncUserInTraining();
    bool createRequest(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRequset(BaseObject *dataIn, BaseObject *dataOut);
    bool parseRespose(BaseObject *dataIn, BaseObject *dataOut);
    void reset();

private:
    biotech::osla::UserInTrainingList userInTrainingList;
};




#endif //OSLA_NEW_SYNCUSERINTRAINING_H
