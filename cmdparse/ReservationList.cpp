//
// Created by 何继胜 on 2017/4/12.
//

#include "cmdparse/ReservationList.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include <controller/Service/SslService.h>
#include <sslconnection/Frame.h>
#include "object/RegisterOperation.h"
DynClassRegister(ReservationList);

ReservationList::ReservationList(std::string name) : BaseObjectExt(name) {

}

ReservationList::~ReservationList() {

}

bool ReservationList::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool ReservationList::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);
    auto sqlworker = SqlWorker::GetSqlWorkByThread();
    auto fun = std::bind(&SqlWorker::updateReservation, sqlworker, std::placeholders::_1);
    bool status = ParseMessageType<biotech::osla::ReservationList>(fin->getMessageBody(), fun);
    if (true == status) {
        ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
        SslService* service = context->getSslService();
        service->EmitReservationUpdated();
    }
    createResponseCmd(status, fin, fout);
    return status;
}

bool ReservationList::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void ReservationList::reset() {

}
