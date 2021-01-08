//
// Created by 何继胜 on 2017/4/12.
//

#include "cmdparse/Command.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include <sslconnection/Frame.h>
#include <protobuf/message.pb.h>
#include <sqlworker/sqlworker.h>
#include <object/Public.h>
#include "controller/Service/SslService.h"

DynClassRegister(Command);

Command::Command(std::string name) : BaseObjectExt(name) {

}

Command::~Command() {

}

bool Command::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool Command::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    return parseRespose(dataIn, dataOut);
}

bool Command::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    SslService* sslService = context->getSslService();
    if(nullptr != dataIn) {
        //Frame* frame = dynamic_cast<Frame*>(dataIn);
        //sslService->parseFrame(frame);
    }
    return true;
}

void Command::reset() {

}
