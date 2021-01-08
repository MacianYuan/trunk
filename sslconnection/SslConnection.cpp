#include <QtCore/QTimerEvent>
#include <tools/FrameOkError.h>

#include "tools/common.h"
#include "deviceinfo/version.h"
#include "sslconnection/SslConnection.h"
#include "tools/IniParse.h"
#include "sslconnection/FrameTools.h"
#include "sslconnection/Frame.h"
#include "tools/log.h"

static int TransferTimeOutNum = 10 * 1000;

SslConnection::SslConnection(QObject *parent) : QSslSocket(parent) {
    transferTimerId = 0;
    newFrame = false;
    length = 0;
    connect(this, SIGNAL(readyRead()), this, SLOT(readSocket()));
}

void SslConnection::timerEvent(QTimerEvent *timerEvent) {
    if (timerEvent->timerId() == transferTimerId) {
        emit connectionError(TransferTimeOut);
        DEBUG_LOG("SslConnection: Transfer time out");
        resetConnection();
    }
}

void SslConnection::checkOverFlow() {
    FrameTools *tools = FrameTools::GetInstance();
    if ((tools->getMaxBufferSize() - recvBuffer.size()) <= 0) {
        recvBuffer.clear();
        DEBUG_LOG("SslConnection: Receiver buffer overflow");
    }
}

bool SslConnection::recvHeader() {
    FrameTools *tools = FrameTools::GetInstance();

    /// 头部数据长度是否够
    if (recvBuffer.size() < tools->getSizeOfHeader()) {
        return false;
    }

    /// 检查是否有头部数据
    QByteArray Header = QString::fromStdString(tools->getHeader()).toAscii();
    int headerIndex = recvBuffer.indexOf(Header);
    if (-1 == headerIndex) { // 未找到帧头
        recvBuffer.remove(0, recvBuffer.size() - tools->getSizeOfHeader() + 1);
        return false;
    }

    /// 去除头部数据的多余数据
    recvBuffer.remove(0, headerIndex);

    /// 找到了头部指针，开始读取完整的帧
    newFrame = true;

    return true;
}

bool SslConnection::recvLength() {
    FrameTools *tools = FrameTools::GetInstance();
    if (recvBuffer.size() < (tools->getSizeOfLength() + tools->getSizeOfHeader())) { // 未找到帧长
        return false;
    }

    length = bytesToInt(recvBuffer.mid(tools->getSizeOfHeader(), tools->getSizeOfLength()), tools->getSizeOfLength());
    DEBUG_LOG("length is : " << length);
    return true;
}

bool SslConnection::checkLengthIsLegal() {
    FrameTools *tools = FrameTools::GetInstance();

    if (length > tools->getMaxBufferSize() || length < tools->getMinFrameLength()) { // 无效帧长
        emit connectionError(InvalidFrameLength);
        DEBUG_LOG("SslConnection: Invalid frame length" << length);
        recvBuffer.clear();
        newFrame = false;
        length = 0;
        return false;
    }
    return true;
}

void SslConnection::readSocket() {
    FrameTools *tools = FrameTools::GetInstance();

    /// 开始读取数据
    checkOverFlow();
    recvBuffer.append(read(tools->getMaxBufferSize() - recvBuffer.size()));

    do {
        if (false == newFrame) {
            /// 接受帧头
            if (false == recvHeader()) {
                return;
            }
        }

        if (0 == length) {
            /// 接受帧的长度
            if (false == recvLength()) {
                return;
            }

            /// 检查长度是否合法
            if (false == checkLengthIsLegal()) {
                continue;
            }
        }

        /// 数据长度是否够
        if (recvBuffer.size() < (length + tools->getSizeOfHeader() + tools->getSizeOfLength())) {
            return;
        }

        FrameSP recv = std::make_shared<Frame>();
        if (false == recv->DecodeFrame(recvBuffer)) { // 校验失败
            emit connectionError(FrameChecksumError);
            DEBUG_LOG("SslConnection: Frame checksum error");
        } else {
            std::string ss;
            for (int i = 0; i < recvBuffer.size(); ++i) {
                char temp[32] = {0};
                sprintf(temp, " %02x", (int) recvBuffer[i]);
                ss += std::string(temp);
            }
            DEBUG_LOG("Recv Message is : " << ss);

            //FrameSP recvOk = FrameOkError::createRecvOk();
            //sendMessage(recvOk);
            emit newMessage(recv);
        }

        recvBuffer.remove(0, length + tools->getSizeOfHeader() + tools->getSizeOfLength());
        newFrame = false;
        length = 0;
    } while (!recvBuffer.isEmpty());
}

void SslConnection::printSocketError(QAbstractSocket::SocketError error) {
    static std::map<int, std::string> errorInfo = {
            {ConnectionRefusedError,           "ConnectionRefusedError"},
            {RemoteHostClosedError,            "RemoteHostClosedError"},
            {HostNotFoundError,                "HostNotFoundError"},
            {SocketAccessError,                "SocketAccessError"},
            {SocketResourceError,              "SocketResourceError"},
            {SocketTimeoutError,               "SocketTimeoutError"},
            {DatagramTooLargeError,            "DatagramTooLargeError"},
            {NetworkError,                     "NetworkError"},
            {AddressInUseError,                "AddressInUseError"},
            {SocketAddressNotAvailableError,   "SocketAddressNotAvailableError"},
            {UnsupportedSocketOperationError,  "UnsupportedSocketOperationError"},
            {UnfinishedSocketOperationError,   "UnfinishedSocketOperationError"},
            {ProxyAuthenticationRequiredError, "ProxyAuthenticationRequiredError"},
            {SslHandshakeFailedError,          "SslHandshakeFailedError"},
            {ProxyConnectionRefusedError,      "ProxyConnectionRefusedError"},
            {ProxyConnectionClosedError,       "ProxyConnectionClosedError"},
            {ProxyConnectionTimeoutError,      "ProxyConnectionTimeoutError"},
            {ProxyNotFoundError,               "ProxyNotFoundError"},
            {ProxyProtocolError,               "ProxyProtocolError"},
            {UnknownSocketError,               "UnknownSocketError"},
    };
    auto iter = errorInfo.find(error);
    ERROR_LOG("SslConnection: Socket error (" << error << "," << iter->second.c_str() << ")");
}

void SslConnection::onDisconnected() {
    DEBUG_LOG("SslConnection: Server disconnected");
}

void SslConnection::resetConnection() {
    abort(); // 直接关闭连接
    stopTimer(transferTimerId); // 关闭超时定时器
    recvBuffer.clear();
    newFrame = false;
    length = 0;
    DEBUG_LOG("SslConnection: Connection is reset");
}

bool SslConnection::hasEnoughData(const int len) {
    stopTimer(transferTimerId);

    if (recvBuffer.size() < len) { // 如果没有足够多的数据则再次打开超时定时器
        transferTimerId = startTimer(TransferTimeOutNum);
        return false;
    }

    return true;
}

inline void SslConnection::stopTimer(int timerId) {
    if (timerId) {
        killTimer(timerId);
        timerId = 0;
    }
}

qint64 SslConnection::sendMessage(FrameSP frame, bool needResend) {
    if (!frame) {
        return 0;
    }
    QByteArray needSend = frame->getNeedSend();
    if (0 >= needSend.size()) {
        return 0;
    }
//    if (true == needResend) {
//        ResendNode node;
//        node.seq = frame->getSequence();
//        node.time = time(nullptr);
//        node.sp = frame;
//        needResendFrame.insert(std::make_pair(node.seq, node));
//    }
    std::string info;
    for (int i = 0; i < needSend.size(); ++i) {
        char temp[32] = {0};
        sprintf(temp, " %02x", (unsigned char) needSend[i]);
        info += std::string(temp);
    }
    DEBUG_LOG("Send Message is : " << info);
    int n = write(needSend);
    return n;
}
