//
// Created by 何继胜 on 2017/4/11.
//

#include "sslconnection/Sequence.h"

int Sequence::m_Sequence = 0;

int Sequence::GetNextSeq() {
    if (++m_Sequence < 0) {
        m_Sequence = 1;
    }
    return m_Sequence;
}
