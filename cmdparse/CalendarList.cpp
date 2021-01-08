//
// Created by 何继胜 on 2017/4/12.
//

#include "cmdparse/CalendarList.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include <sslconnection/Frame.h>
#include <protobuf/message.pb.h>
#include <sqlworker/sqlworker.h>
#include <object/Public.h>

DynClassRegister(CalendarList);
CalendarList::CalendarList(std::string name) : BaseObjectExt(name) {

}

CalendarList::~CalendarList() {

}

bool CalendarList::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool CalendarList::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);
    auto sqlworker = SqlWorker::GetSqlWorkByThread();
    auto fun = std::bind(&SqlWorker::updateCalendar, sqlworker, std::placeholders::_1);
    bool status = ParseMessageType<biotech::osla::CalendarList>(fin->getMessageBody(), fun);
    createResponseCmd(status, fin, fout);
    return status;
}

bool CalendarList::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void CalendarList::reset() {

}
