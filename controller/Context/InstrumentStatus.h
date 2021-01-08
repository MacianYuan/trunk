
#ifndef INSTRUMENTSTATUS_H
#define INSTRUMENTSTATUS_H

#include <QtCore/QObject>
#include <deviceinfo/typedef.h>
#include "protobuf/message.pb.h"

class InstrumentStatus : public QObject
{
    Q_OBJECT
public:
	enum WorkingState {
		NORMAL = 0,         /// 常规状态
		RESERVATION = 1,    /// 专享状态
		TRAINING = 2,       /// 培训状态
		MAINTENANCE = 3,    /// 机器维护状态
		EDUCATION = 4,      /// 教学状态
		ENTRUST = 5,        /// 委托状态
        ASSISTANCE= 6,      /// 代刷
    };

    explicit InstrumentStatus(QObject* parent = NULL);
	void InitBaseInfo();
    ~InstrumentStatus();

    void SetInUseStatus(bool inUsed);
    bool GetInUseStatus();

    void SetWorkStatus(WorkingState st);
    void SetWorkStatusBySessionType(int sessionType); /// 主要是重启的时候使用，回复之前的状态
    WorkingState GetWorkStatus();
    void ShowWorkStatus(QObject* object, int cuit);

    void SetNetWorkStatus(bool status);
    bool GetNetWorkStatus();

    void SetInstrumentStatus(int status);
    int GetInstrumentStatus();
    int GetId();
    int GetBookingHoldingTime();
	int GetBookingBreakTime();
    int GetAlertTime();
    int GetMinInterval();

    /// 检测设备是否可以使用
    bool InstrumentStatusCheck(User& user);
    bool InstrumentStatusCheckExpectFaulty(User& user);

private:
    bool m_InUsed{ false };  /// 是否有用户在使用，true：有用户在使用，false：无用户在使用
    WorkingState m_WorkingStatus{ NORMAL };  /// 仪器工作状态
    bool m_NetWorkStatus{false}; /// 是否连接到服务器，true：连接到服务器，false：未连接到服务器
    biotech::osla::InstrumentDetail m_InstrumentDetail;         /// 设备信息
};

#endif //INSTRUMENTSTATUS_H
