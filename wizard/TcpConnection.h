#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QtNetwork/QTcpSocket>
#include <sslconnection/Frame.h>
#include "Public.h"

class TcpConnection : public QTcpSocket {
Q_OBJECT

public:
    explicit TcpConnection(QObject *parent = 0);

    void setPasswd(int passwd);
    qint64 sendMessage(FrameSP frame);
    void resetConnection();

signals:
    void connectionError(const SetupError error);
    void showMessage(const QString& info);
    void changeRegisterState(int state);
    void connectSslHost();

private slots:
    void readSocket();
    void onConnected();
    void onDisconnected();
    void parseMessage(const QByteArray &message);

protected:
    void timerEvent(QTimerEvent *timerEvent);

    int calcLoginToken(int id, int passwd);

private:
    bool hasEnoughData(const int len);

    inline void stopTimer(int timerId);

private:
    QByteArray buffer; // 接收缓冲区
    int transferTimerId; // 内部定时器Id
    int length; // 消息长度
    int passwd{0};
};

#endif // TCPCONNECTION_H
