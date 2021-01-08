
#include <tools/common.h>
#include "controller/Context/UsbNetConnection.h"
#include "sqlworker/sqlworker.h"
#include <QDebug>
#include "controller/Base/GlobalData.h"

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

UsbNetConnection::UsbNetConnection(QObject* parent /*= NULL*/) : QObject(parent)
{
    //m_IpHost.setAddress(tr("169.254.126.255"));
    m_IpHost.setAddress(tr("169.254.255.255"));
    m_IpPort = 10240;
    m_HeartBeatUsbNet = new QUdpSocket(this);
    //connect(m_HeartBeatUsbNet, SIGNAL(readyRead()), this, NULL);
}

UsbNetConnection::~UsbNetConnection()
{
}

void UsbNetConnection::SendLoginRequest(uint serial)
{
    QByteArray ba;
    ba.append(NetFrameClientReqLogin);
    ba.append(intToBytes2(serial));
    Send(ba);
}

void UsbNetConnection::SendLogoutRequest(uint serial)
{
    QByteArray ba;
    ba.append(NetFrameClientReqLogout);
    ba.append(intToBytes2(serial));
    Send(ba);
}

void UsbNetConnection::SendHeartbeatIdle()
{
    QByteArray ba;
    ba.append(NetFrameClientHeartbeat);
    ba.append(NetFrameClientIdle);
    Send(ba);
}

void UsbNetConnection::SendHeartbeatInUse()
{
    QByteArray ba;
    ba.append(NetFrameClientHeartbeat);
    ba.append(NetFrameClientInUse);
    Send(ba);
}

void UsbNetConnection::SendToWin_FTP()
{
    QByteArray ba;

    auto sqlWorker = SqlWorker::GetSqlWorkByThread();
    sqlWorker->getUserEvent_data();


    QString id = QString::number(GlobalData::id,10);
    ba.append(", id:");
    ba.append(id);

    QString user_id = QString::number(GlobalData::user_id,10);
    ba.append(", user_id:");
    ba.append(user_id);

    QString group_id = QString::number(GlobalData::group_id,10);
    ba.append(", group_id:");
    ba.append(group_id);


    ba.append(", card_serial:");
    ba.append(GlobalData::card_serial);

    QString card_type = QString::number(GlobalData::card_type,10);
    ba.append(", card_type:");
    ba.append(card_type);

    QString action_type = QString::number(GlobalData::action_type,10);
    ba.append(", action_type:");
    ba.append(action_type);

    QString action_result = QString::number(GlobalData::action_result,10);
    ba.append(", action_result:");
    ba.append(action_result);

    QString create_time = QString::number(GlobalData::create_time,10);
    ba.append(", create_time:");
    ba.append(create_time);

    QString instrument_id = QString::number(GlobalData::instrument_id,10);
    ba.append(", instrument_id:");
    ba.append(instrument_id);


    auto sqlWorker1 = SqlWorker::GetSqlWorkByThread();
    user = sqlWorker1->getUserInfoById(GlobalData::user_id);

    QString account_id = QString::number(user.account.id,10);
    ba.append(", account_id:");
    ba.append(account_id);

    qDebug()<<ba.data();
    SendToWin(ba);

}
void UsbNetConnection::SendToWin(QByteArray content)
{
    QByteArray ba;
    ba.append("$"); //02
    ba.append(",");
    int len = 1 + 1 + content.length() + 1 + 1; //len
    QString SendToWin_len = QString::number(len,10);
    ba.append(SendToWin_len); /// serailΪ0
    ba.append(content);
    ba.append("$");
    m_HeartBeatUsbNet->writeDatagram(ba, m_IpHost, m_IpPort);
}
void UsbNetConnection::Send(QByteArray content)
{
    QByteArray ba;
    ba.append(NetFrameHeader);
    int len = 1 + 1 + content.length() + 1 + 1;
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

void UsbNetConnection::SendRefuseAck(uint serial)
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

void UsbNetConnection::NetOnReadyRead()
{
    while (m_HeartBeatUsbNet->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(m_HeartBeatUsbNet->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        m_HeartBeatUsbNet->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        if (buffer.startsWith(NetFrameHeader) && 
            buffer.endsWith(NetFrameEnd) && 
            buffer.size() >= 6) {
            int sum = 0;
            for (int i = 0; i < buffer.size() - 1; i++) {
                sum += buffer[i];
            }

            if ((sum & 0xff) == 0xff) {
               // int len = buffer[1];
                unsigned char cmd = buffer[2];
                if (cmd == NetFrameClientReqLogin) {
                    uint serial = bytesToInt2(buffer.mid(3, 4));
                    SendRefuseAck(serial);
                }
            }
        }
    }
}
