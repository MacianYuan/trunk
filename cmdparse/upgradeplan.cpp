#include "upgradeplan.h"
#include <QtCore>
#include <QtXml>
#include "cmdparse/upgradeplan.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include <sslconnection/Frame.h>
#include <tools/IniParse.h>
#include "object/RegisterOperation.h"
#include <tools/log.h>
#include <deviceinfo/version.h>
#include <protobuf/message.pb.h>
#include <sqlworker/sqlworker.h>
#include <tools/ConvertFrameObject.h>
#include "controller/Service/SslService.h"
#include "upgrade/qupgradeplan.h"
#include "upgrade/QUpgrade.h"

DynClassRegister(UpgradePlan);

UpgradePlan::UpgradePlan(std::string name) : BaseObjectExt(name) {

}

UpgradePlan::~UpgradePlan() {

}

bool UpgradePlan::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool UpgradePlan::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);

    bool status = false;
    do {
        biotech::osla::UpgradePlan upgradePlan;
        ConvertFrameObject::convertFrame2Object(fin, upgradePlan);
        DEBUG_LOG("SslService: Received upgradePlan");
        IniParse upgradeConfig(UPGRADE_CONFIG);
        upgradeConfig.Set("upgradePlan.type", upgradePlan.type());
        QDateTime dateTime;
        dateTime = QDateTime::fromMSecsSinceEpoch(upgradePlan.start_date());
        upgradeConfig.Set("upgradePlan.start_date", dateTime.toString("yyyy-MM-dd hh:mm:ss").toStdString());
        upgradeConfig.Set("upgradePlan.interval", upgradePlan.interval());
        if (upgradePlan.type() == PLAN_TYPE_TIMER)
        {
            upgradeConfig.Set("upgrade.state", STATE_HAS_PLAN);
        }

        upgradeConfig.SaveFile();
        QProcess::execute("sync");
        SslService* service = context->getSslService();
        service->EmitUpgradePlanUpdated();

        status = true;
    } while (false);

    createResponseCmd(status, fin, fout);
    return status;
}

bool UpgradePlan::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void UpgradePlan::reset() {

}
