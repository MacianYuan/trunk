//
// Created by 何继胜 on 2017/5/4.
//

#ifndef OSLA_NEW_FRAMEOKERROR_H
#define OSLA_NEW_FRAMEOKERROR_H


#include <sslconnection/Frame.h>

class FrameOkError {
public:
    static FrameSP createRecvOk();
    static FrameSP createRecvOk(FrameSP org);
    static FrameSP createRecvError();
    static FrameSP createRecvError(FrameSP org);
    static FrameSP createParseOK();
    static FrameSP createParseError();

private:
    static FrameSP create(int cmd);
    static FrameSP create(int cmd, FrameSP org);
};


#endif //OSLA_NEW_FRAMEOKERROR_H
