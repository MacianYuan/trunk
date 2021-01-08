//
// Created by 何继胜 on 2017/4/12.
//

#include <functional>
#include "cmdparse/BookingList.h"
#include <sslconnection/ClientContext.h>
#include <sslconnection/Frame.h>
#include <protobuf/message.pb.h>
#include <sqlworker/sqlworker.h>
#include <controller/Service/SslService.h>
#include "object/RegisterOperation.h"

DynClassRegister(BookingList);

BookingList::BookingList(std::string name) : BaseObjectExt(name) {

}

BookingList::~BookingList() {

}

bool BookingList::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool BookingList::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);
    auto sqlworker = SqlWorker::GetSqlWorkByThread();
    auto fun = std::bind(&SqlWorker::updateBooking, sqlworker, std::placeholders::_1);
    bool status = ParseMessageType<biotech::osla::BookingList>(fin->getMessageBody(), fun);
    if (true == status) {
        SslService *service = context->getSslService();
        service->EmitBookingListUpdate();
    }
    createResponseCmd(status, fin, fout);
    return status;
}

bool BookingList::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void BookingList::reset() {

}
