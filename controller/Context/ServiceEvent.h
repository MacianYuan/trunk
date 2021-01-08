
#ifndef SERVICEEVENT_H
#define SERVICEEVENT_H

#include <mutex>
#include <QtCore/QObject>
#include <deviceinfo/typedef.h>
#include <sqlworker/sqlworker.h>
#include "protobuf/message.pb.h"


#define SE_G  ServiceEvent::GetInstance()

class ServiceEvent : public QObject
{
    Q_OBJECT
public:
    static ServiceEvent* GetInstance();
    static void Destory();

    /// <summary>
    /// 添加用户事件.
    /// </summary>
    /// <param name="user">用户信息.</param>
    /// <param name="actionType">用户动作类型.</param>
    /// <param name="actionResult">操作结果.</param>
    void AddUserEvent(User& user, uint actionType, uint actionResult);

    /// <summary>
    /// 添加系统事件.
    /// </summary>
    /// <param name="eventType">事件类型.</param>
    /// <param name="eventContent">事件内容.</param>
    void AddSystemEvent(uint eventType, quint64 eventContent = 0);

    /// <summary>
    /// 添加会话信息.
    /// </summary>
    /// <param name="session">会话信息.</param>
    /// <param name="isCompleted">会话是否完成.</param>
    void AddSessionEvent(biotech::osla::Session& session, bool isCompleted);

    void AddFtpEvent(biotech::osla::FtpEvent& ftp);

    void AddDeviceCaptureEvent(biotech::osla::DeviceCaptureEvent& deviceCapture);

    void AddEntrustEvent(biotech::osla::EntrustEvent& entrustEvent);

    /// <summary>
    /// 添加培训信息.
    /// </summary>
    /// <param name="id">会话ID信息.</param>
    void AddTraningEvent(quint64 id);

protected:
    explicit ServiceEvent(QObject* parent);
    ~ServiceEvent();

private:
    static ServiceEvent* m_Instance;
    static std::mutex m_Mutex;
};

#endif //SERVICEEVENT_H