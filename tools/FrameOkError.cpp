//
// Created by 何继胜 on 2017/5/4.
//

#include <protobuf/message.pb.h>
#include <QtCore/QByteArray>
#include "FrameOkError.h"
#include "ConvertFrameObject.h"

FrameSP FrameOkError::createRecvOk() {
    return create(biotech::osla::Command_Option_CLIENT_RECV_OK);
}

FrameSP FrameOkError::createRecvOk(FrameSP org) {
    return create(biotech::osla::Command_Option_CLIENT_RECV_OK, org);
}

FrameSP FrameOkError::createRecvError() {
    return create(biotech::osla::Command_Option_CLIENT_RECV_ERR);
}

FrameSP FrameOkError::createRecvError(FrameSP org) {
    return create(biotech::osla::Command_Option_CLIENT_RECV_ERR, org);
}

FrameSP FrameOkError::createParseOK() {
    return create(biotech::osla::Command_Option_CLIENT_PARSE_OK);
}

FrameSP FrameOkError::createParseError() {
    return create(biotech::osla::Command_Option_CLIENT_PARSE_ERR);
}

FrameSP FrameOkError::create(int cmd) {
    FrameSP sp = std::make_shared<Frame>();
    biotech::osla::Command command;
    command.set_option((biotech::osla::Command_Option) cmd);
    ConvertFrameObject::convertObject2Frame(sp.get(), command, biotech::osla::COMMAND);
    return sp;
}

FrameSP FrameOkError::create(int cmd, FrameSP org) {
    FrameSP sp = std::make_shared<Frame>();
    DEBUG_LOG("--org seq is : " << org->getSequence());
    sp->setSequence(org->getSequence());
    biotech::osla::Command command;
    command.set_option((biotech::osla::Command_Option) cmd);
    ConvertFrameObject::convertObject2Frame(sp.get(), command, biotech::osla::COMMAND);
    return sp;
}
