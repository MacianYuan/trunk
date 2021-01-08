//
// Created by 何继胜 on 2017/4/11.
//

#include <tools/common.h>
#include <quazip/quaadler32.h>
#include <build/QT/include/QtCore/QByteArray>
#include <build/QT/include/QtCore/QString>
#include <tools/log.h>
#include "sslconnection/Frame.h"
#include "sslconnection/Sequence.h"
#include "sslconnection/FrameTools.h"

Frame::Frame(std::string name) : BaseObject(name) {
}

bool Frame::EncodeFrame(const int messageType, const QByteArray &message, int newseq) {
    needSend.clear();

    DEBUG_LOG("in new seq is : " << newseq);
    if (0 == newseq) {
        this->sequence = Sequence::GetNextSeq();
    } else {
        this->sequence = newseq;
    }
    DEBUG_LOG("out new seq is : " << sequence);
    this->messageType = messageType;

    FrameTools *tools = FrameTools::GetInstance();
    QByteArray type = intToBytes(this->messageType, tools->getSizeOfType());
    QByteArray seq = intToBytes(this->sequence, tools->getSizeOfSeq());


    QuaAdler32 adler32;
    adler32.reset();
    adler32.update(type);
    adler32.update(seq);
    adler32.update(message);
    messageBody = message;
    QByteArray checksum = intToBytes(adler32.value(), tools->getSizeOfChecksum());

    QByteArray length = intToBytes(tools->getSizeOfType()
                                   + tools->getSizeOfSeq()
                                   + message.size()
                                   + tools->getSizeOfChecksum(),
                                   tools->getSizeOfLength());
    QByteArray Header = QString::fromStdString(tools->getHeader()).toAscii();
    needSend = Header + length + type + seq + message + checksum;

    std::string ss;
    for (int i = 0; i < needSend.size(); ++i) {
        char temp[32] = {0};
        sprintf(temp, "%x", (int) needSend[i]);
        ss += " " + std::string(temp);
    }
    //DEBUG_LOG("send Head buffer is : " << ss.c_str() << ", (message.size() + SizeOfType + SizeOfChecksum) len is :" << (message.size() + SizeOfType + SizeOfChecksum));

    return true;
}

bool Frame::DecodeFrame(QByteArray &message) {
    FrameTools *tools = FrameTools::GetInstance();

    /// 帧头
    int pos = 0;

    /// 帧长度 数据长度 len = type + seq + all + index + body + crc
    pos += tools->getSizeOfHeader();
    int len = bytesToInt(message.mid(tools->getSizeOfHeader(), tools->getSizeOfLength()), tools->getSizeOfLength());

    /// 消息类型
    pos += tools->getSizeOfLength();
    QByteArray mtArr = message.mid(pos, tools->getSizeOfType());
    messageType = bytesToInt(mtArr, tools->getSizeOfType());

    /// 序列号
    pos += tools->getSizeOfType();
    QByteArray seqArr = message.mid(pos, tools->getSizeOfSeq());
    sequence = bytesToInt(seqArr, tools->getSizeOfSeq());
    DEBUG_LOG("decode frame, seq is : "<< sequence);

    /// 数据
    pos += tools->getSizeOfSeq();
    QByteArray data = message.mid(pos, len - tools->getSizeOfType() - tools->getSizeOfSeq() - tools->getSizeOfChecksum());
    messageBody = data;

    /// crc值
    quint32 checksum = bytesToInt(message.right(tools->getSizeOfChecksum()), tools->getSizeOfChecksum());

    QuaAdler32 adler32;
    adler32.reset();
    QByteArray ss = message.mid(tools->getSizeOfHeader() + tools->getSizeOfLength(), len - tools->getSizeOfChecksum());
    adler32.update(ss);
    if (checksum != adler32.value()) {
        return false;
    }

    return true;
}

const QByteArray &Frame::getNeedSend() const {
    return needSend;
}

const QByteArray &Frame::getHaveRecv() const {
    return haveRecv;
}

int Frame::getSequence() const {
    return sequence;
}

int Frame::getMessageType() const {
    return messageType;
}

const QByteArray &Frame::getMessageBody() const {
    return messageBody;
}

void Frame::setNeedSend(const QByteArray &needSend) {
    Frame::needSend = needSend;
}

int Frame::getSequence() {
    return sequence;
}

void Frame::setSequence(int seq) {
    sequence = seq;
}



