
#include "controller/Context/HeartBeatUsbNet.h"
#include "tools/common.h"
#include "sqlworker/sqlworker.h"

static const unsigned char FTPFrameHeader = 0xFF;
static const unsigned char FTPFrameEnd = 0xAA;
static const unsigned char NetFrameHeader = 0x02;
static const unsigned char NetFrameClientReqLogin = 0xA0;
static const unsigned char NetFrameClientReqLogout = 0xA9;
static const unsigned char NetFrameServerAck = 0xA5;
static const unsigned char NetFrameServerLogin = 0x59;
static const unsigned char NetFrameServerLogout = 0x50;
static const unsigned char NetFrameServerRefuse = 0x57;
static const unsigned char NetFrameClientHeartbeat = 0x53;
static const unsigned char NetFrameClientIdle = 0x32;
static const unsigned char NetFrameClientInUse = 0x33;
static const unsigned char NetFrameEnd = 0x03;

HeartBeatUsbNet::HeartBeatUsbNet(QObject* parent /*= NULL*/) : QObject(parent)
{
    //m_IpHost = QHostAddress(tr("169.254.126.100"));
    m_IpHost.setAddress(tr("169.254.126.88"));
    m_IpPort = 10240;
    m_HeartBeatUsbNet = new QUdpSocket(this);
    connect(m_HeartBeatUsbNet, SIGNAL(readyRead()), this, NULL);
}

HeartBeatUsbNet::~HeartBeatUsbNet()
{
    delete m_HeartBeatUsbNet;
}

void HeartBeatUsbNet::SendLoginRequest(uint serial)
{
    QByteArray ba;
    ba.append(NetFrameClientReqLogin);
    ba.append(intToBytes2(serial));
    Send(ba);
}

void HeartBeatUsbNet::SendLogoutRequest(uint serial)
{
    QByteArray ba;
    ba.append(NetFrameClientReqLogout);
    ba.append(intToBytes2(serial));
    Send(ba);
}

void HeartBeatUsbNet::SendHeartbeatIdle()
{
    QByteArray ba;
    ba.append(NetFrameClientHeartbeat);
    ba.append(NetFrameClientIdle);
    Send(ba);
}

void HeartBeatUsbNet::SendHeartbeatInUse()
{
    QByteArray ba;
    ba.append(NetFrameClientHeartbeat);
    ba.append(NetFrameClientInUse);
    Send(ba);
}
void HeartBeatUsbNet::SendToWin_FTP()
{
    QByteArray ba;

    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    userEventList = sqlWorker->getUserEvent(time(NULL), 1);
    if(userEventList.user_event_size() > 0 ) {
       // for (int i = 0; i < userEventList.user_event_size(1); ++i) {
            const biotech::osla::UserEvent &s = userEventList.user_event(1);
            ba.append(s.id());
            ba.append("$");
            ba.append(s.user_id());
            ba.append("$");
            ba.append(s.group_id());
            ba.append("$");
            ba.append(s.card_serial().data());
            ba.append("$");
            ba.append(s.card_type());
            ba.append("$");
            ba.append(s.action_type());
            ba.append("$");
            ba.append(s.action_result());
            ba.append("$");
            ba.append(s.create_time());
            ba.append("$");
            ba.append(s.instrument_id());

            SendToWin(ba);
       // }
    }
}

void HeartBeatUsbNet::SendToWin(QByteArray content)
{
    QByteArray ba;
    ba.append(FTPFrameHeader); //02
    int len = 1 + 1 + content.length() + 1 + 1; //len
    ba.append((unsigned char) len & 0xff); /// serailΪ0
    ba.append(content);
    unsigned char sum = 0;
    for (int i = 0; i < ba.size(); i++) {
        sum += ba[i];
        sum &= 0xff;
    }
    unsigned char checksum = sum ^ 0xff;
    ba.append(checksum);
    ba.append(FTPFrameEnd);
    m_HeartBeatUsbNet->writeDatagram(ba, m_IpHost, m_IpPort);
}

void HeartBeatUsbNet::Send(QByteArray content)
{
    QByteArray ba;
    ba.append(NetFrameHeader); //02
    int len = 1 + 1 + content.length() + 1 + 1; //len
    ba.append((unsigned char) len & 0xff); /// serailΪ0
    ba.append(content);
    unsigned char sum = 0;
    for (int i = 0; i < ba.size(); i++) {
        sum += ba[i];
        sum &= 0xff;
    }
    unsigned char checksum = sum ^ 0xff;
    ba.append(checksum);
    ba.append(NetFrameEnd);
    m_HeartBeatUsbNet->writeDatagram(ba, m_IpHost, m_IpPort);
}

void HeartBeatUsbNet::SendRefuseAck(uint serial)
{
    QByteArray ba;
    //ba.append(NetFrameHeader);
    //ba.append(address);
    ba.append(NetFrameServerAck);
    ba.append(NetFrameServerRefuse);
    ba.append(intToBytes2(serial));
    //ba.append(NetFrameEnd);
    Send(ba);
}

void HeartBeatUsbNet::NetOnReadyRead()
{
    qDebug() << "call function " << __FUNCTION__;
    while (m_HeartBeatUsbNet->hasPendingDatagrams()) {
        qDebug() << "call function " << __FUNCTION__ << 1;
        QByteArray buffer;
        buffer.resize(m_HeartBeatUsbNet->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        qDebug() << "call function " << __FUNCTION__ << 2;
        m_HeartBeatUsbNet->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        qDebug() << "call function " << __FUNCTION__ << 3;
        if (buffer.startsWith(NetFrameHeader) && 
            buffer.endsWith(NetFrameEnd) && 
            buffer.size() >= 6) {
            qDebug() << "call function " << __FUNCTION__ << 4;
            int sum = 0;
            for (int i = 0; i < buffer.size() - 1; i++) {
                sum += buffer[i];
            }

            if ((sum & 0xff) == 0xff) {
                qDebug() << "call function " << __FUNCTION__ << 5;
                int len = buffer[1];
                unsigned char cmd = buffer[2];
                if (cmd == NetFrameClientReqLogin) {
                    qDebug() << "call function " << __FUNCTION__ << 6;
                    uint serial = bytesToInt2(buffer.mid(3, 4));
                    qDebug() << "SerialPort: Card" << QByteArray::number(serial, 16).toUpper() << "detected from Net";
                    SendRefuseAck(serial);
                }
            }
        }
    }
}
