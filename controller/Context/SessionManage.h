//
// Created by 何继胜 on 2017/5/2.
//

#ifndef OSLA_NEW_SESSIONMANAGE_H
#define OSLA_NEW_SESSIONMANAGE_H

#include <build/QT/include/QtCore/QObject>
#include "WindowContext.h"

struct SessionNode {
    int session_type;
    int instruction_type;
    int action_type;
};

class SessionManage : public QObject {
    Q_OBJECT
public:
    SessionManage(QObject* parent);
    void Init(WindowContext* context);
    bool RestoreSession();
    bool StartSession(SessionNode &node);
    void EndSession();
    void SendLogin();
    void SendLogout();
    void SendHeartbeat();
    int UpdateSessionStatus();

signals:
    void ShowCurrentUserInfo();
    void SetCurrentStateInfo();

private:
    void PromptStartSession();
    void PromptEndSession();

private:
    WindowContext* windowContext;
};


#endif //OSLA_NEW_SESSIONMANAGE_H
