//
// Created by 何继胜 on 2017/4/11.
//

#ifndef OSLA_NEW_FRAMETOOLS_H
#define OSLA_NEW_FRAMETOOLS_H

#include <string>

class FrameTools {
public:
    static void Init();

    static void Destory();

    static FrameTools *GetInstance();

    int getSizeOfHeader() const;

    int getSizeOfLength() const;

    int getSizeOfType() const;

    int getSizeOfChecksum() const;

    int getMinFrameLength() const;

    int getMaxBufferSize() const;

    int getTransferTimeOut() const;

    const std::string &getHeader() const;

    int getSizeOfSeq() const;

private:
    FrameTools();

    ~FrameTools();

private:
    int SizeOfHeader;
    int SizeOfSeq;
    int SizeOfLength;
    int SizeOfType;
    int SizeOfChecksum;
    int MinFrameLength;
    int MaxBufferSize;
    int TransferTimeOut;
    std::string Header;
};


#endif //OSLA_NEW_FRAMETOOLS_H
