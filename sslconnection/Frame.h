//
// Created by 何继胜 on 2017/4/11.
//

#ifndef OSLA_NEW_FRAME_H
#define OSLA_NEW_FRAME_H

#include <QtCore>
#include "object/BaseObject.h"

// Frame = Header + Seq + Length + Type + Message + Checksum
// length = type.length + message.length + checksum.length

class Frame : public BaseObject {
public:
    Frame(std::string name = "Frame");
    bool DecodeFrame(QByteArray &message);
    bool EncodeFrame(const int messageType, const QByteArray &message, int newseq = 0);
    const QByteArray &getNeedSend() const;

    void setNeedSend(const QByteArray &needSend);

    const QByteArray &getHaveRecv() const;
    int getSequence() const;
    int getMessageType() const;
    const QByteArray &getMessageBody() const;
    int getSequence();
    void setSequence(int seq);

private:
    QByteArray needSend;
    QByteArray haveRecv;
    int sequence{0};
    int messageType{0};
    QByteArray messageBody;
};

using FrameSP = std::shared_ptr<Frame>;

#endif //OSLA_NEW_FRAME_H
