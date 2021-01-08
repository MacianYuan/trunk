
#include <controller/Base/GlobalData.h>
#include <tools/log.h>
#include "ServiceEvent.h"


ServiceEvent *ServiceEvent::m_Instance = nullptr;
std::mutex ServiceEvent::m_Mutex;

ServiceEvent::ServiceEvent(QObject *parent) {
}

ServiceEvent::~ServiceEvent() {

}

ServiceEvent *ServiceEvent::GetInstance() {
    if (nullptr == m_Instance) {
        std::lock_guard<std::mutex> guard(m_Mutex);
        if (nullptr == m_Instance) {
            m_Instance = new ServiceEvent(nullptr);
        }
    }
    return m_Instance;
}

void ServiceEvent::Destory() {
    if (nullptr != m_Instance) {
        std::lock_guard<std::mutex> guard(m_Mutex);
        if (nullptr != m_Instance) {
            delete m_Instance;
            m_Instance = nullptr;
        }
    }
}

void ServiceEvent::AddUserEvent(User &user, uint actionType, uint actionResult) {
    user.timestamp = time(nullptr);
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    DEBUG_LOG(__FUNCTION__ << "sql worker is " << m_SqlWork);
    m_SqlWork->addUserEvent(user, actionType, actionResult);
}

void ServiceEvent::AddSystemEvent(uint eventType, quint64 eventContent) {
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    DEBUG_LOG(__FUNCTION__ << "sql worker is " << m_SqlWork);
    m_SqlWork->addSystemEvent(eventType, eventContent);
}

void ServiceEvent::AddSessionEvent(biotech::osla::Session &session, bool isCompleted) {
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    DEBUG_LOG(__FUNCTION__ << "sql worker is " << m_SqlWork);
    m_SqlWork->addSession(session, isCompleted);
}

void ServiceEvent::AddFtpEvent(biotech::osla::FtpEvent &ftp) {
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    DEBUG_LOG(__FUNCTION__ << "sql worker is " << m_SqlWork);
    m_SqlWork->addFtpEvent(ftp);
}

void ServiceEvent::AddTraningEvent(quint64 id) {
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    DEBUG_LOG(__FUNCTION__ << "sql worker is " << m_SqlWork);
    m_SqlWork->endUserInTraining(id);
}

void ServiceEvent::AddDeviceCaptureEvent(biotech::osla::DeviceCaptureEvent &deviceCapture) {
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    DEBUG_LOG(__FUNCTION__ << "sql worker is " << m_SqlWork);
    m_SqlWork->addDeviceCaptureEvent(deviceCapture);
}

void ServiceEvent::AddEntrustEvent(biotech::osla::EntrustEvent &entrustEvent) {
    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    DEBUG_LOG(__FUNCTION__ << "sql worker is " << m_SqlWork);
    m_SqlWork->addEntrustEvent(entrustEvent);
}
