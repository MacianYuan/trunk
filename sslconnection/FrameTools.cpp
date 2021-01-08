//
// Created by 何继胜 on 2017/4/11.
//
#include <QtNetwork>
#include <tools/IniParse.h>
#include <deviceinfo/version.h>
#include <sslconnection/FrameTools.h>

FrameTools::FrameTools() {
    SizeOfHeader = sizeof(int32_t);
    SizeOfSeq = sizeof(int32_t);
    SizeOfLength = sizeof(int32_t);
    SizeOfType = sizeof(int32_t);
    SizeOfChecksum = sizeof(int32_t);
    MinFrameLength = SizeOfType + SizeOfChecksum;
    MaxBufferSize = 1024 * 1024;
    TransferTimeOut = 10 * 1000;

    IniParse osla(OSLA_CONFIG);
    int version = osla.GetLong("Global.Version");
    if (2 == version) {
        Header = "~:~:";
    } else if (3 == version) {
        Header = "~:~4";
    }
}

FrameTools::~FrameTools() {

}

int FrameTools::getSizeOfHeader() const {
    return SizeOfHeader;
}

int FrameTools::getSizeOfLength() const {
    return SizeOfLength;
}

int FrameTools::getSizeOfType() const {
    return SizeOfType;
}

int FrameTools::getSizeOfChecksum() const {
    return SizeOfChecksum;
}

int FrameTools::getMinFrameLength() const {
    return MinFrameLength;
}

int FrameTools::getMaxBufferSize() const {
    return MaxBufferSize;
}

int FrameTools::getTransferTimeOut() const {
    return TransferTimeOut;
}

const std::string &FrameTools::getHeader() const {
    return Header;
}

int FrameTools::getSizeOfSeq() const {
    return SizeOfSeq;
}

FrameTools *FrameTools::GetInstance() {
    static FrameTools tools;
    return &tools;
}

void FrameTools::Destory() {

}

