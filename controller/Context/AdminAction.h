
#ifndef OSLA_NEW_ADMINACTION_H
#define OSLA_NEW_ADMINACTION_H

#include <QtCore/QObject>
#include "controller/Context/WindowContext.h"
#include "controller/Context/SessionManage.h"

class AdminAction : public QObject {
    Q_OBJECT
public:
    explicit AdminAction(QObject* parent = nullptr);
    void Init(WindowContext* c, SessionManage* s);

    /////////////////////////////////////
    /// true:admin处理了登录信息，不用其他的处理了
    bool doAdminActionInfo();

    /////////////////////////////////////
    bool AdminCardSwiping();
    bool TrainingCardSwiping();
    bool EducationCardSwiping();
    bool MaintenanceCardSwiping();

    //////////////////////////////////////
    bool AdminActionStartEducation();
    bool AdminActionStartTraning();
    bool AdminActionStartMaintenance();
    bool AdminActionStartAssistance();
    bool AdminActionEndSession();
    bool AdminActionConfigSystem();
    bool AdminActionViewRecords();
    bool AdminActionEntrust();
    bool AdminActionDefault();
    //////////////////////////////////////
    bool IsContinueUseInstrument();
    bool TypeCardSwiping(int type);

signals:
    void ShowWorkStatus(int num);

private:
    WindowContext* windowContext;
    SessionManage* sessionManage;
};


#endif //OSLA_NEW_ADMINACTION_H
