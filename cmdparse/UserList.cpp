//
// Created by 何继胜 on 2017/4/12.
//

#include "cmdparse/UserList.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include <Service/SslService.h>
#include <sslconnection/Frame.h>
#include "object/RegisterOperation.h"
DynClassRegister(UserList);

UserList::UserList(std::string name) : BaseObjectExt(name) {

}

UserList::~UserList() {

}

bool UserList::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool UserList::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    auto fun = std::bind(&SqlWorker::updateUser, sqlWorker, std::placeholders::_1);
    bool status = ParseMessageType<biotech::osla::UserList>(fin->getMessageBody(), fun);
    createResponseCmd(status, fin, fout);

    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    context->getSslService()->EmitSettingsUpdated();
    return status;
}

bool UserList::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void UserList::reset() {

}
