//
// Created by 何继胜 on 2017/4/12.
//

#include <tools/log.h>
#include "object/BaseObjectExt.h"
#include "protobuf/message.pb.h"
#include "sslconnection/Frame.h"

BaseObjectExt::BaseObjectExt(std::string name) : BaseObject(name) {

}

BaseObjectExt::~BaseObjectExt() {

}

void BaseObjectExt::createResponseCmd(bool status, BaseObject* org, BaseObject *in) {
    biotech::osla::Command command;
    if (true == status) {
        command.set_option(biotech::osla::Command_Option_CLIENT_RECV_OK);
    } else {
        command.set_option(biotech::osla::Command_Option_CLIENT_RECV_ERR);
    }
    int size = command.ByteSize();

    QByteArray message(size, '0');
    command.SerializeToArray(message.data(), size);
    Frame *inFrame = dynamic_cast<Frame *>(in);
    Frame * frame = dynamic_cast<Frame *>(org);
    DEBUG_LOG("replay seq is : " << frame->getSequence());
    inFrame->EncodeFrame(biotech::osla::COMMAND, message, frame->getSequence());
}