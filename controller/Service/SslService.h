#ifndef SSLSERVICE_H
#define SSLSERVICE_H

#include <mutex>
#include <QtCore/QObject>
#include <QtCore/QTime>
#include <QtCore/QVector>
#include <QtNetwork/QAbstractSocket>

#include "protobuf/message.pb.h"
#include "sslconnection/Frame.h"

class QTimer;
class SqlWorker;
class SslConnection;
class ClientContext;

class SslService : public QObject {
    Q_OBJECT

public:
    explicit SslService(QObject *parent = 0);
    ~SslService();

    enum SslServiceError {
        DatabaseOpenError,
        TableCreateError,
        InvalidFrameLength,
        FrameChecksumError
    };

    void EmitBookingListUpdate();
    void EmitSettingsUpdated();
    void EmitReservationUpdated();
    void EmitCardDetectedFromNet(int id, int userid, uint cardId, int userrole, int op);
    void EmitBindingReleased(bool);
    void EmitUpgradeParamsUpdated();
    void EmitUpgradePlanUpdated();

signals:
    /// 设置更新
    void settingsUpdated();
    /// 用户更新
    void userUpdated();
    /// 预约更新
    void bookingUpdated();
    /// 专项更新
    void reservationUpdated();
    /// 网络状态发生变化
    void networkStateChanged(bool);
    /// 让界面开始解绑设备
    void bindingReleased(bool);
    /// 远程关机信息
    void cardDetectedFromNet(int, int, uint, int, int);
    /// 网络出差
    void error(SslServiceError error);
    //升级参数下发
    void upgradeParamsUpdated();
    //升级计划下发
    void upgradePlanUpdated();

public slots:
    void start();
    //void stop();
    void setClientState(int state);
    void sendUnbindingRequest();
    void sendDetectedFromNetResp(int id, int status, int bookid);
    void doDisconnected();
    void doConnected();
    void doRegularTask();
    void parseFrame(FrameSP frame);
    void onSocketError(QAbstractSocket::SocketError err);
    void onConnectTimeout(void);


protected:
    void checkNeedResendMessage();

private:
    void reset();
    void clearSendMessage();
    inline void stopTimer(int timerId);

private:
    struct SendNode {
        int seq;
        std::shared_ptr<BaseObject> parse;
        std::shared_ptr<Frame> sendFrame;
        time_t sendTime;
        int resend;

        SendNode(int seq, std::shared_ptr<BaseObject> parse, std::shared_ptr<Frame> sendFrame) {
            this->seq = seq;
            this->parse = parse;
            this->sendFrame = sendFrame;
            sendTime = time(nullptr);
            resend = 0;
        }
    };

    std::mutex m_ConnectionMutex;
    std::mutex m_SendMessageMutex;
    std::map<int, SendNode> m_SendMessage;  /// 只有需要确认重发的消息，才放到此队列中

    SslConnection *sslConnection;          /// ssl会话连接
    QTimer clockTimer;                     /// 定时任务定时器
    QTime offlineTime;                     /// 离线时间计数器
    QTimer connectTimer;                   /// 连接超时定时器
    ClientContext* context;        /// 客户端上下文
};

#endif // SSLSERVICE_H
