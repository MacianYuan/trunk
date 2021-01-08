#include <QtCore>
#include <QtXml>
#include "cmdparse/upgradeparams.h"
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
#include "upgrade/QUpgrade.h"

DynClassRegister(UpgradeParams);

UpgradeParams::UpgradeParams(std::string name) : BaseObjectExt(name) {

}

UpgradeParams::~UpgradeParams() {

}

bool UpgradeParams::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool UpgradeParams::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);

    bool status = false;
    do {
        biotech::osla::UpgradeParams upgradeParams;
        ConvertFrameObject::convertFrame2Object(fin, upgradeParams);
        DEBUG_LOG("SslService: Received upgrade params");
        IniParse upgradeConfig(UPGRADE_CONFIG);
        upgradeConfig.Set("upgradeParams.enable", upgradeParams.enable());
        upgradeConfig.Set("upgradeParams.forceUpgrade", upgradeParams.force_upgrade());
        upgradeConfig.Set("upgradeParams.newVer", upgradeParams.new_ver());
        upgradeConfig.Set("upgradeParams.fromVer", upgradeParams.from_ver());
        //将size和adler32值转换成字符串存入conf文件
        QString intstr;
        intstr = QString("%1").arg((unsigned int)upgradeParams.size());
        upgradeConfig.Set("upgradeParams.size", intstr.toStdString());
        intstr = QString("%1").arg((unsigned int)upgradeParams.adler32());
        upgradeConfig.Set("upgradeParams.adler32", intstr.toStdString());
        upgradeConfig.Set("upgradeParams.url", upgradeParams.url());
        upgradeConfig.Set("upgradeParams.upgradeId", upgradeParams.reversed1());

        upgradeConfig.SaveFile();
        QProcess::execute("sync");
        SslService* service = context->getSslService();
        service->EmitUpgradeParamsUpdated();

        status = true;
    } while (false);


    createResponseCmd(status, fin, fout);
    return status;
}

bool UpgradeParams::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void UpgradeParams::reset() {

}
