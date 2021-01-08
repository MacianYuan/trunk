//
// Created by 何继胜 on 2017/4/12.
//

#include "cmdparse/SessionOperation.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include <controller/Service/SslService.h>
#include <sslconnection/Frame.h>
#include <tools/log.h>
#include "object/RegisterOperation.h"

DynClassRegister(SessionOperation);

SessionOperation::SessionOperation(std::string name) : BaseObjectExt(name) {

}

SessionOperation::~SessionOperation() {

}

bool SessionOperation::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool SessionOperation::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);

    DEBUG_LOG("user use net operation the machine");
    biotech::osla::SessionOpertaionReq soreq;
    QByteArray data = fin->getMessageBody();
    bool status = false;

    do {
        if (!soreq.ParseFromArray(data.data(), data.size())) {
            break;
        }
        SslService *service = context->getSslService();

        //远程关机以用户ID来判断
        auto sqlWorker = SqlWorker::GetSqlWorkByThread();
        User user = sqlWorker->getUserInfoById(soreq.user_id());
        QString cardStr = QString::fromStdString(user.card.serial);
        bool ok;
        uint card = cardStr.toUInt(&ok);
        if (!ok)
        {
            card = 0;
        }
        //sscanf(soreq.card_serial().c_str(), "%ud", &card);

        service->EmitCardDetectedFromNet(soreq.id(), soreq.user_id(), card,
                                         soreq.user_role(), soreq.operation_type());
        status = true;
    } while (false);
    createResponseCmd(status, fin, fout);

    return true;
}

bool SessionOperation::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void SessionOperation::reset() {

}
