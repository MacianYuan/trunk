//
// Created by 何继胜 on 2017/4/12.
//

#include <QtCore/QDateTime>
#include <tools/ConvertFrameObject.h>
#include "cmdparse/SyncUserInTraining.h"
#include "protobuf/message.pb.h"
#include "tools/log.h"
#include "object/RegisterOperation.h"
#include "sslconnection/Frame.h"
#include "sslconnection/ClientContext.h"
#include "sslconnection/Tools.h"

DynClassRegister(SyncUserInTraining);

SyncUserInTraining::SyncUserInTraining(std::string name) : BaseObjectExt(name) {

}

SyncUserInTraining::~SyncUserInTraining() {

}

bool SyncUserInTraining::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("Checking TUserInTraining...");
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    userInTrainingList = sqlWorker->getUserInTraining(time(NULL), 1);
    if (0 == userInTrainingList.user_in_training_size()) {
        return true;
    }
    DEBUG_LOG(userInTrainingList.user_in_training_size() << " unsynced records found");
    ConvertFrameObject::convertObject2Frame(dataOut, userInTrainingList, biotech::osla::USERINTRAININGLIST);

    return true;
}

bool SyncUserInTraining::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    return true;
}

bool SyncUserInTraining::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    biotech::osla::Command command;
    ConvertFrameObject::convertFrame2Object(dataIn, command);
    int upToStatus = (biotech::osla::Command_Option::Command_Option_SERVER_RECV_OK == command.option() ? 2 : 0);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();

    for (int i = 0; i < userInTrainingList.user_in_training_size(); ++i) {
        const biotech::osla::UserInTraining &s = userInTrainingList.user_in_training(i);
        uint card = 0;
        sscanf(s.card_serial().c_str(), "%u", &card);
        sqlWorker->setSyncFlag((long long)s.session_id(), card, upToStatus);
    }

    return true;
}

void SyncUserInTraining::reset() {

}
