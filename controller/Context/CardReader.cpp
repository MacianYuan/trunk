#include <QtCore/QTimerEvent>
#include <deviceinfo/rc522.h>
#include <deviceinfo/tq2440.h>
#include "controller/Context/CardReader.h"

static const uint Interval = 100; // 寻卡间隔（毫秒）
static const uint StableCount = 2;

//CardReader::CardReader(QObject *parent) : QObject(parent), serialEndian(QSysInfo::BigEndian) {
CardReader::CardReader(QObject *parent) : QObject(parent), serialEndian(QSysInfo::LittleEndian) {
    ResetRC522();
    PcdInit();
}

void CardReader::timerEvent(QTimerEvent *event) {
    if (event->timerId() == timerId) {
        seekCard();
    }
}

void CardReader::setGuardInterval(uint msecs) {
    guardCount = msecs / Interval;
}

void CardReader::setSerialEndian(QSysInfo::Endian endian) {
    serialEndian = endian;
}

void CardReader::open() {
    timerId = startTimer(Interval);
}

void CardReader::close() {
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    }
}

void CardReader::seekCard() {
    if (count < StableCount && count != StableCount) {
        for (int i = 0; i < 4; i++) {
            serialBytes[i] = 0;
        }
        if (PcdSeek(serialBytes) != MI_OK) {
            count = 0;
            return;
        }
        uint serialTemp = 0;
        for (int i = 0; i < 4; i++) {
            if (serialEndian == QSysInfo::BigEndian) {
                serialTemp += (serialBytes[3 - i] << (8 * i));
            } else if (serialEndian == QSysInfo::LittleEndian) {
                serialTemp += (serialBytes[i] << (8 * i));
            }
        }
        if (serialTemp == 0) {
            count = 0;
            return;
        }
        if (count == 0) {
            serial = serialTemp;
            count++;
        } else {
            if (serial == serialTemp) {
                count++;
            } else {
                count = 0;
                return;
            }
        }

        /// 如果两次读取出来的卡是一样的，说明是有效卡
        if (count == StableCount) {
            beep(2600, 250);
            emit cardDetected(serial);
        }
    } else if (count >= guardCount) {
        count = 0;
    } else {
        count++;
    }
}
