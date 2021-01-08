#include <QtCore/QTime>
#include <tools/common.h>
#include <tools/log.h>
#include <build/QT/include/QtSql/QtSql>
#include <deviceinfo/version.h>
#include <tools/IniParse.h>
#include "wizard/TcpConnection.h"
#include "wizard/Public.h"

// Frame = Length + Message
static const int SizeOfLength = sizeof(int32_t); // 消息长度域字节数
static const int TimeOut = 10 * 1000; // 超时时间
static const int MaxBufferSize = 1024 * 1024; // 最大缓冲区长度

TcpConnection::TcpConnection(QObject *parent) : QTcpSocket(parent)
{
    transferTimerId = 0;
    length = 0;
    connect(this, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(this, SIGNAL(readyRead()), this, SLOT(readSocket())); // 接收数据
    connect(this, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}

qint64 TcpConnection::sendMessage(FrameSP frame) {
    const QByteArray& needSend = frame->getNeedSend();

    int len = needSend.size();
    if (len <= 0) {
        return 0;
    }
    int n = write(intToBytes(len, SizeOfLength)); // 发送消息长度
    n += write(needSend); // 发送消息
    return n;
}

void TcpConnection::resetConnection()
{
    abort(); // 直接关闭连接
    stopTimer(transferTimerId);
    length = 0; // 重置消息长度
    buffer.clear(); // 清空接收缓冲区
    DEBUG_LOG("TcpConnection: Connection is close");
}

void TcpConnection::readSocket()
{
    do {
        // 接收消息长度域
        if (length <= 0) {
            if (!hasEnoughData(SizeOfLength)) { // 等待接收到足够多的数据
                return;
            }
            buffer.clear();
            buffer = read(SizeOfLength);
            length = bytesToInt(buffer, SizeOfLength); // 提取消息长度
            if (length <= 0 || length > MaxBufferSize) { // 无效消息长度
                emit connectionError(InvalidFrameLength);
                DEBUG_LOG("TcpConnection: Invalid frame length" << length);
                length = 0;
                continue;
            }
        }
        // 接收消息域
        if (!hasEnoughData(length)) { // 等待接收到足够多的数据
            return;
        }

        buffer.clear();
        buffer = read(length);
        parseMessage(buffer);
        length = 0; // 重置消息长度
    } while (bytesAvailable() > 0); // 循环直到所有数据被接收
}

// 定时器超时事件
void TcpConnection::timerEvent(QTimerEvent *timerEvent)
{
    if (timerEvent->timerId() == transferTimerId) {
        stopTimer(transferTimerId);
        emit connectionError(TransferTimeOut); // 发送超时错误信号
		DEBUG_LOG("TcpConnection: Transfer time out");
        resetConnection(); // 重置连接
    }
}

// 检查socket缓冲区是否有足够多数据
bool TcpConnection::hasEnoughData(const int len)
{
    stopTimer(transferTimerId);
    if (bytesAvailable() < len) { // 如果没有足够多的数据则再次打开超时定时器
        transferTimerId = startTimer(TimeOut);
        return false;
    }

    return true;
}

inline void TcpConnection::stopTimer(int timerId)
{
    if (0 < timerId) {
        killTimer(timerId);
        timerId = 0;
    }
}

void TcpConnection::onConnected() {
    DEBUG_LOG("TCP connection open, send login request");
    QTime now = QTime::currentTime(); // 获取当前时间
    int currentMins = now.hour() * 60 + now.minute(); // 计算当前时间的分钟数（0~1439）
    qsrand(QTime(0, 0, 0).secsTo(now)); // 用当前时间做随机数种子
    int loginId = (qrand() % 1484092) * 1447 + currentMins; // 随机产生登录ID（不超过0x7fffffff）
    //int loginPasswd = passwd; // 读取用户输入的密码
    int loginPasswd = 12345678; // 读取用户输入的密码
    int loginToken = calcLoginToken(loginId, loginPasswd); // 根据登录ID和登录密码产生登录令牌

    // 登录请求 = 登录ID + 登录令牌
    QByteArray loginMessage = intToBytes(loginId, 4) + intToBytes(loginToken, 4);
    emit showMessage(tr("Waiting for authorization verification..."));//等待授权验证
    std::shared_ptr<Frame> frame = std::make_shared<Frame>();
    frame->setNeedSend(loginMessage);
    sendMessage(frame);
}

void TcpConnection::onDisconnected() {
    DEBUG_LOG("TCP connection closed");
}

int TcpConnection::calcLoginToken(int id, int passwd) {
    short q3 = (id >> 24) & 0xff;
    short q2 = (id >> 16) & 0xff;
    short q1 = (id >> 8) & 0xff;
    short q0 = id & 0xff;
    short m00 = (q3 & 0x03) + 2;
    short m01 = (q2 + q3) % 31 + 1;
    short m10 = (q0 + q1) % 17 + 1;
    short m11 = (q1 & 0x03) + 2;
    int a = (q0 * m01 + q2 * m11) % 126 + 1;
    a = (a << 8) + ((q1 * m01 + q3 * m11) & 0xff);
    a = (a << 8) + ((q0 * m00 + q2 * m10) & 0xff);
    a = (a << 8) + ((q1 * m00 + q3 * m10) & 0xff);
    return a ^ passwd;
}

void TcpConnection::setPasswd(int passwd) {
    TcpConnection::passwd = passwd;
}

void TcpConnection::parseMessage(const QByteArray &message) {
    emit changeRegisterState(TcpReceivedState);

    static const QByteArray AuthFailed("error");
    if (message == AuthFailed) {
        emit connectionError(AuthFailedError); // 授权校验失败
        return;
    }

    DEBUG_LOG("TCP authentication ok");

    QList<QByteArray> hostInfo = message.split(':');
    QString ntpHost = QString(hostInfo[0]);
    QString sslHost = QString(hostInfo[1]);
    long sslPort = (long)(hostInfo[2].toInt());

    QByteArray certMD5 = hostInfo[3];
    QByteArray expectedMD5 = QCryptographicHash::hash(hostInfo[4], QCryptographicHash::Md5).toHex();
    if (expectedMD5 != certMD5) {
        emit connectionError(CertChecksumError); // 主机信息解析错误
        return;
    }

    QFile certFile(SSL_CERT);
    if (!certFile.open(QFile::WriteOnly)) {
        emit connectionError(CertFileOpenError); // 文件打开错误
        return;
    }
    certFile.write(hostInfo[4]); // 写入SSL证书文件
    certFile.close();

    IniParse ini(OSLA_CONFIG);
    ini.Set("Network.SslHost", sslHost.toStdString());
    ini.Set("Network.SslPort", (long) sslPort);
    ini.Set("Network.NtpHost", ntpHost.toStdString());
    ini.Set("Network.SslCert", std::string(SSL_CERT));
    ini.SaveFile();
    QProcess::startDetached("sync");

    DEBUG_LOG("SSL settings saved");

    resetConnection(); // 断开tcp连接
    emit showMessage(tr("Establish an SSL connection..."));//建立SSL连接...
    emit changeRegisterState(SslConnectingState);
    emit connectSslHost(); // 连接ssl主机
}
