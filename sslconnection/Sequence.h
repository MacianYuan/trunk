//
// Created by 何继胜 on 2017/4/11.
//

#ifndef OSLA_NEW_SEQUENCE_H
#define OSLA_NEW_SEQUENCE_H


class Sequence {
public:
    static int GetNextSeq();

private:
    static int m_Sequence;
};


#endif //OSLA_NEW_SEQUENCE_H
