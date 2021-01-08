//
// Created by 何继胜 on 2017/4/12.
//

#include <QtCore/QDateTime>
#include <QtGui/QApplication>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>

#include <deviceinfo/version.h>
#include <tools/ConvertFrameObject.h>

#include "cmdparse/Unbinding.h"
#include "protobuf/message.pb.h"
#include "tools/log.h"
#include "object/RegisterOperation.h"
#include "sslconnection/Frame.h"
#include "sslconnection/ClientContext.h"
#include "controller/Service/SslService.h"
#include "sslconnection/Tools.h"

DynClassRegister(Unbinding);

Unbinding::Unbinding(std::string name) : BaseObject(name) {

}

Unbinding::~Unbinding() {

}

bool Unbinding::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);

    biotech::osla::InstrumentBinding request;
    request.set_option(biotech::osla::InstrumentBinding_Option_RELEASE_BINDING);
    request.set_instrument_id(context->getInstrumentId());
    request.set_client_model(APP_TYPE);
    request.set_client_version(qApp->applicationVersion().toLatin1().data());

    QFile netConfigFile("/proc/cmdline");
    if (netConfigFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&netConfigFile);
        QString StringMac = stream.readAll();
        int MAC = StringMac.indexOf(QRegExp("mac="), 0);
        QString tMac = StringMac.mid(MAC + 4, 17);
        QByteArray baMac = tMac.toLatin1();
        request.set_client_mac_addr(baMac.data());
    } else {
        ERROR_LOG("Failed to open mac of cmdline");
        return false;
    }
    netConfigFile.close();

    ConvertFrameObject::convertObject2Frame(dataOut, request, biotech::osla::INSTRUMENTBINDING);

    return true;
}

bool Unbinding::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    return parseRespose(dataIn, dataOut);
}

bool Unbinding::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    DEBUG_LOG("");
    biotech::osla::Command command;
    ConvertFrameObject::convertFrame2Object(dataIn, command);

    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    SslService *sslService = context->getSslService();
    DEBUG_LOG("hello, go here, status is : " << command.option());
    if (biotech::osla::Command_Option::Command_Option_SERVER_RECV_OK == command.option()) {
        sslService->EmitBindingReleased(true);
    } else {
        sslService->EmitBindingReleased(false);
    }

    return true;
}

void Unbinding::reset() {

}
