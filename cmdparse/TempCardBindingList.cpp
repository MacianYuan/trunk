//
// Created by 何继胜 on 2017/4/12.
//

#include "cmdparse/TempCardBindingList.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include <sslconnection/Frame.h>
#include <object/Public.h>

DynClassRegister(TempCardBindingList);
TempCardBindingList::TempCardBindingList(std::string name) : BaseObjectExt(name) {

}

TempCardBindingList::~TempCardBindingList() {

}

bool TempCardBindingList::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool TempCardBindingList::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);
    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    auto fun = std::bind(&SqlWorker::updateTempCardBinding, sqlWorker, std::placeholders::_1);
    bool status = ParseMessageType<biotech::osla::TempCardBindingList>(fin->getMessageBody(), fun);
    createResponseCmd(status, fin, fout);
    return status;
}

bool TempCardBindingList::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void TempCardBindingList::reset() {

}
