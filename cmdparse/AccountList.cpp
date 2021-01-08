//
// Created by 何继胜 on 2017/4/12.
//

#include "cmdparse/AccountList.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include <sslconnection/Frame.h>
#include <protobuf/message.pb.h>
#include <sqlworker/sqlworker.h>
#include <object/Public.h>
#include <tools/log.h>

DynClassRegister(AccountList);

AccountList::AccountList(std::string name) : BaseObjectExt(name) {

}

AccountList::~AccountList() {

}

bool AccountList::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool AccountList::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);
    auto sqlworker = SqlWorker::GetSqlWorkByThread();
    auto fun = std::bind(&SqlWorker::updateAccount, sqlworker, std::placeholders::_1);
    bool status = ParseMessageType<biotech::osla::AccountList>(fin->getMessageBody(), fun);
    createResponseCmd(status, fin, fout);
    return status;
}

bool AccountList::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void AccountList::reset() {

}
