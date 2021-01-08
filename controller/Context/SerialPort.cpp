#include <deviceinfo/tq2440.h>
#include "Context/SerialPort.h"
#include "tools/common.h"
#include "tools/IniParse.h"

static const unsigned char FrameHeader = 0x02;
static const unsigned char FrameClientReqLogin = 0xA0;
static const unsigned char FrameClientReqLogout = 0xA9;
static const unsigned char FrameServerAck = 0xA5;
static const unsigned char FrameServerLogin = 0x59;
static const unsigned char FrameServerLogout = 0x50;
static const unsigned char FrameServerRefuse = 0x57;
static const unsigned char FrameClientHeartbeat = 0x53;
static const unsigned char FrameClientIdle = 0x32;
static const unsigned char FrameClientInUse = 0x33;
static const unsigned char FrameEnd = 0x03;

SerialPort::SerialPort(QString portName, QObject *parent) : QextSerialPort(portName, QextSerialPort::EventDriven, parent)
{
	IniParse client("/storage/conf/controller.conf");
    setBaudRate((BaudRateType)(client.GetLong("Serial.BaudRate")));
    setDataBits((DataBitsType)(client.GetLong("Serial.DataBits")));
    setParity((ParityType)(client.GetLong("Serial.Parity")));
    setStopBits((StopBitsType)(client.GetLong("Serial.StopBits")));
    setFlowControl(FLOW_OFF);
    connect(this, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

unsigned char SerialPort::portAddress() const
{
	return address;
}

void SerialPort::setPortAddress(unsigned char addr)
{
    address = addr;
}

void SerialPort::sendLoginRequest(uint serial)
{
    QByteArray ba;
    ba.append(FrameClientReqLogin);
    ba.append(intToBytes2(serial));
    send(ba);
}

void SerialPort::sendLogoutRequest(uint serial)
{
    QByteArray ba;
    ba.append(FrameClientReqLogout);
    ba.append(intToBytes2(serial));
    send(ba);
}

void SerialPort::sendHeartbeatIdle()
{
    QByteArray ba;
    ba.append(FrameClientHeartbeat);
    ba.append(FrameClientIdle);
    send(ba);
}

void SerialPort::sendHeartbeatInUse()
{
    QByteArray ba;
    ba.append(FrameClientHeartbeat);
    ba.append(FrameClientInUse);
    send(ba);
}

void SerialPort::setRTS(bool on)
{
    if (portName() == TQ2440_SERIAL1) {
        com1_rts((int)on);
    }
 //   else if (portName() == TQ2440_SERIAL2)
   //     com2_rts((int)on);
}

void SerialPort::write(QByteArray data)
{
    setRTS(true);
    msleep(10);
    QextSerialPort::write(data);
    msleep(200);
    setRTS(false);
}

void SerialPort::send(QByteArray content)
{
    QByteArray ba;
    ba.append(FrameHeader);
    ba.append(address);
    ba.append(content);
    unsigned char sum = 0;
    for (int i = 0; i < ba.size(); i++) {
        sum += ba[i];
        sum &= 0xff;
    }
    unsigned char checksum = sum ^ 0xff;
    ba.append(checksum);
    ba.append(FrameEnd);
    write(ba);
}

void SerialPort::sendRefuseAck(uint serial)
{
    QByteArray ba;
    ba.append(FrameHeader);
    ba.append(address);
    ba.append(FrameServerAck);
    ba.append(FrameServerRefuse);
    ba.append(intToBytes2(serial));
    ba.append(FrameEnd);
    write(ba);
}


void SerialPort::onReadyRead()
{
    if (!bytesAvailable()) {
        return;
    }
    QByteArray buffer = readAll();

    if (buffer.startsWith(FrameHeader) && buffer.endsWith(FrameEnd) && buffer.size() >= 6) {
        int sum = 0;
        for (int i = 0; i < buffer.size() - 1; i++) {
            sum += buffer[i];
        }
        if ((sum & 0xff) == 0xff) {
            address = buffer[1];
            unsigned char cmd = buffer[2];
            if (cmd == FrameClientReqLogin) {
                uint serial = bytesToInt2(buffer.mid(3, 4));
                emit cardDetected(serial);
                sendRefuseAck(serial);
            }
        }
    }
}
