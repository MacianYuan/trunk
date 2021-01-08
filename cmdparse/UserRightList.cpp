//
// Created by 何继胜 on 2017/4/12.
//

#include "cmdparse/UserRightList.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include <sslconnection/Frame.h>
#include <Service/SslService.h>
#include <object/Public.h>

DynClassRegister(UserRightList);

UserRightList::UserRightList(std::string name) : BaseObjectExt(name) {

}

UserRightList::~UserRightList() {

}

bool UserRightList::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool UserRightList::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    auto fun = std::bind(&SqlWorker::updateUserRight, sqlWorker, std::placeholders::_1);
    bool status = ParseMessageType<biotech::osla::UserRightList>(fin->getMessageBody(), fun);
    createResponseCmd(status, fin, fout);
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    context->getSslService()->EmitSettingsUpdated();
    return status;
}

bool UserRightList::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void UserRightList::reset() {

}
