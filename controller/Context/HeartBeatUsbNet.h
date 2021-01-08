#ifndef HEARTBEATUSBNET_H
#define HEARTBEATUSBNET_H

#include <QtCore/QObject>
#include <QtNetwork/QUdpSocket>
#include <protobuf/message.pb.h>
class HeartBeatUsbNet : public QObject
{
    Q_OBJECT
public:
    explicit HeartBeatUsbNet(QObject* parent = NULL);
    ~HeartBeatUsbNet();
    void SendLoginRequest(uint serial);
    void SendLogoutRequest(uint serial);
    void SendHeartbeatIdle();
    void SendHeartbeatInUse();
    void SendToWin_FTP();
private:
    void Send(QByteArray content);
    void SendRefuseAck(uint serial);

    void SendToWin(QByteArray content);
private slots:
    void NetOnReadyRead();

private:
    QUdpSocket* m_HeartBeatUsbNet;
    QHostAddress m_IpHost;
    int m_IpPort;
    biotech::osla::UserEventList userEventList;
};

#endif //HEARTBEATUSBNET_H
