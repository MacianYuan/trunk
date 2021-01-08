#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "qextserialport/src/qextserialport.h"

class SerialPort : public QextSerialPort
{
    Q_OBJECT
public:
    explicit SerialPort(QString portName, QObject *parent = 0);
    unsigned char portAddress() const;
    void setPortAddress(unsigned char addr);
    void sendLoginRequest(uint serial);
    void sendLogoutRequest(uint serial);
    void sendHeartbeatIdle();
    void sendHeartbeatInUse();

private:
    void setRTS(bool on);
    void write(QByteArray data);
    void send(QByteArray content);
    void sendRefuseAck(uint serial);

signals:
    void cardDetected(uint serial);
    void heartbeatDetected();

private slots:
    void onReadyRead();

private:
    unsigned char address;
};

#endif // SERIALPORT_H
