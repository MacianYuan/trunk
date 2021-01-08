#include <QtNetwork/QTcpSocket>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <deviceinfo/version.h>
#include <tools/log.h>

#include "controller/Context/CASConnection.h"
#include "tools/common.h"
#include "tools/IniParse.h"

CASConnection::CASConnection(QObject* parent /*= NULL*/)
{
    QProcess::startDetached("ifconfig eth0:0 192.168.33.28");
    casConnection = new QTcpSocket();
    connect(casConnection, SIGNAL(disconnected()), this, SLOT(CASReconnect()));
    connect(casConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(CASReconnect()));
    CASConnect();
}

CASConnection::~CASConnection()
{
    delete casConnection;
}

void CASConnection::SendLogin(uint cardId)
{
	IniParse ini(CLIENT_CONFIG);
    QByteArray ba;
    ba.append(0x02);
	unsigned char addr = ini.GetLong("CAS.ClientAddr") & 0XFF;
    ba.append(addr);
    ba.append(0xA0);
    ba.append(intToBytes2(cardId));
    unsigned char checksum = 0;
    for (int i = 0; i < ba.size(); i++) {
        checksum += ba[i];
        checksum &= 0xff;
    }
    checksum ^= 0xff;
    ba.append(checksum);
    ba.append(0x03);
    casConnection->write(ba);
}

void CASConnection::SendLogout(uint cardId)
{
    QByteArray ba;
	IniParse ini(CLIENT_CONFIG);
    ba.append(0x02);
	unsigned char addr = ini.GetLong("CAS.ClientAddr") & 0XFF;
    ba.append(addr);
    ba.append(0xA9);
    ba.append(intToBytes2(cardId));
    unsigned char checksum = 0;
    for (int i = 0; i < ba.size(); i++) {
        checksum += ba[i];
        checksum &= 0xff;
    }
    checksum ^= 0xff;
    ba.append(checksum);
    ba.append(0x03);
    casConnection->write(ba);
}

void CASConnection::SendHeartBeat(bool inUse)
{
    QByteArray ba;
    ba.append(0x02);
	IniParse ini(CLIENT_CONFIG);
	unsigned char add = ini.GetLong("CAS.ClientAddr") & 0XFF;
    ba.append(add);
    ba.append(0x53);
    ba.append(inUse ? 0x33 : 0x32);
    unsigned char checksum = 0;
    for (int i = 0; i < ba.size(); i++) {
        checksum += ba[i];
        checksum &= 0xff;
    }
    checksum ^= 0xff;
    ba.append(checksum);
    ba.append(0x03);
    casConnection->write(ba);
}

void CASConnection::CASConnect()
{
	IniParse ini(CLIENT_CONFIG);
    casConnection->connectToHost(ini.GetString("CAS.ServerHost").c_str(), ini.GetLong("CAS.ServerPort"));
    DEBUG_LOG( "MainWindow: Connecting to CAS server...");
}

void CASConnection::CASReconnect()
{
    casConnection->abort();
    DEBUG_LOG( "MainWindow: Reconnect in 15 seconds...");
    QTimer::singleShot(15000, this, SLOT(CASConnect()));
}
