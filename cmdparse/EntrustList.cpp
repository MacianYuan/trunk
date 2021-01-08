//
// Created by 何继胜 on 2017/4/12.
//

#include "cmdparse/EntrustList.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include <controller/Service/SslService.h>
#include <sslconnection/Frame.h>
#include <tools/log.h>
#include "object/RegisterOperation.h"
DynClassRegister(EntrustList);

EntrustList::EntrustList(std::string name) : BaseObjectExt(name) {

}

EntrustList::~EntrustList() {

}

bool EntrustList::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool EntrustList::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("");
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);
    auto sqlworker = SqlWorker::GetSqlWorkByThread();
    auto fun = std::bind(&SqlWorker::updateEntrust, sqlworker, std::placeholders::_1);
    bool status = ParseMessageType<biotech::osla::EntrustList>(fin->getMessageBody(), fun);
    createResponseCmd(status, fin, fout);
    DEBUG_LOG("status is : " << (status ? "true" : "false"));
    return status;
}

bool EntrustList::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void EntrustList::reset() {

}
