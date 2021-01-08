#ifndef SSLSERVICE_H
#define SSLSERVICE_H

#include <QtCore/QObject>
#include <QtCore/QTime>
#include <QtCore/QVector>
#include <build/QT/include/QtNetwork/QAbstractSocket>

#include "protobuf/message.pb.h"
#include "sslconnection/Frame.h"
#include "Public.h"
#include "InstrumentPage.h"

class QTimer;
class SqlWorker;
class SslConnection;
class ClientContext;

class SslService : public QObject {
    Q_OBJECT

public:
    explicit SslService(QObject *parent = 0);
    ~SslService();

    void setSqlWorker(SqlWorker* sqlworker);
    void setInstrumentPage(InstrumentPage *instrumentPage);
    void setHostInfo(QString sslHost, int sslPort);
    int getPhase() const;
    void setPhase(int state);
    QAbstractSocket::SocketState state();

signals:
    void changeRegisterState(int s);
    void showMessage(const QString& info);
    void hideMessage();
    void connectionError(const SetupError error);
    void downloadPageInfo(const QString& label, const QString& ap);
    void downloadPageProcess(int num);
    void nextPage();
    void addInstrument(const QString& name, int id);

public slots:
    void start(bool isGetInstrumentList = true);
    void stop();
    void doDisconnected();
    void doConnected();
    void parseFrame(FrameSP frame);
    void onSocketError(QAbstractSocket::SocketError err);

public:
    void checkNeedResendMessage();
    void sendInitRequest();
    void sendBindingRequest(biotech::osla::InstrumentBinding_Option option);
    // 等待仪器列表状态
    FrameSP UpdateWaitingForInstrumentList(FrameSP frame);
    // 等待仪器绑定请求确认状态
    FrameSP UpdateWaitingForBindingAck(FrameSP frame);
    // 接收数据状态
    FrameSP UpdateWaitingForDataDownload(FrameSP frame);
    FrameSP UpdateWaitingForBindingConfirm(FrameSP frame);

    FrameSP UpdateInstrumentdetail(FrameSP frame);
    FrameSP UpdateUserlist(FrameSP frame);
    FrameSP UpdateAccountlist(FrameSP frame);
    FrameSP UpdateUserRightList(FrameSP frame);
    FrameSP UpdateTempCardBindingList(FrameSP frame);
    FrameSP UpdateReservationList(FrameSP frame);
    FrameSP UpdateBookingList(FrameSP frame);
    FrameSP UpdateEntrustList(FrameSP frame);
    FrameSP UpdateCalendarList(FrameSP frame);
    FrameSP UpdateGlobalConfig(FrameSP frame);
    FrameSP UpdateTinsQuaConfig(FrameSP frame);
    FrameSP UpdateSessionList(FrameSP frame);
    FrameSP UpdateUserIntrainingList(FrameSP frame);
    FrameSP UpdateUserEventList(FrameSP frame);
    FrameSP UpdateSystemEventList(FrameSP frame);
    FrameSP UpdateCommand(FrameSP frame);

    const std::string &getOrgBindCode() const;
    void setOrgBindCode(const std::string &orgBindCode);

private:
    void clearSendMessage();
    void timerEvent(QTimerEvent* timerEvent);
    void stopTimer(int timerId);
private slots:
    void onSslConnectTimerTimeout(void);
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

    std::map<int, SendNode> m_SendMessage;  /// 只有需要确认重发的消息，才放到此队列中
    SslConnection *sslConnection;
    QTimer sslConnectTimer;             //ssl connect timer
    QTime offlineTime;
    ClientContext* context;        /// 客户端上下文
    int phase {0};

    QString sslHost;
    int sslPort{0};
    InstrumentPage* instrumentPage;
    int transferTimerId;
    std::string orgBindCode;
};

#endif // SSLSERVICE_H
