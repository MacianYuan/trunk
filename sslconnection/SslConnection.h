#ifndef SSLCONNECTION_H
#define SSLCONNECTION_H

#include <QtNetwork/QSslSocket>

#include <3rdparty/quazip/quaadler32.h>
#include "sslconnection/Frame.h"

class SslConnection : public QSslSocket
{
    Q_OBJECT
public:
    explicit SslConnection(QObject *parent = 0);
    qint64 sendMessage(FrameSP frame, bool needResend = true);
    void resetConnection();

    enum ConnectionError {
        TransferTimeOut,
        InvalidFrameLength,
        FrameChecksumError
    };
    void printSocketError(QAbstractSocket::SocketError error);

signals:
    void connectionError(const SslConnection::ConnectionError error);
    void newMessage(FrameSP message);
    
private slots:
    void readSocket();
    void onDisconnected();

protected:
    void timerEvent(QTimerEvent *timerEvent);

private:
    bool hasEnoughData(const int length);
    inline void stopTimer(int timerId);
    void checkOverFlow();
    bool recvHeader();
    bool recvLength();
    bool checkLengthIsLegal();

private:
    struct ResendNode {
        int seq{0};   /// 帧的序号
        int time{0};   /// 发送的时间点
        FrameSP sp;  /// 需要重发的帧
        int num{0}; /// 重发次数
    };
    QByteArray recvBuffer;
    std::map<int, ResendNode> needResendFrame;
    //CompleteFrame completeFrame;
    //QByteArray message;
    bool newFrame;
    int length;

    int transferTimerId;
};

#endif // SSLCONNECTION_H
