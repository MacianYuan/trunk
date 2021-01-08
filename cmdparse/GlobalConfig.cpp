//
// Created by 何继胜 on 2017/4/24.
//
#include <QtCore>
#include "cmdparse/GlobalConfig.h"
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
#include "controller/Context/userqualification.h"
#include "tools/IniParse.h"

DynClassRegister(GlobalConfig);

GlobalConfig::GlobalConfig(std::string name) : BaseObjectExt(name) {

}

GlobalConfig::~GlobalConfig() {

}

bool GlobalConfig::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool GlobalConfig::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);

    bool status = false;
    do {
        biotech::osla::GlobalConfig globalConfig;
        ConvertFrameObject::convertFrame2Object(fin, globalConfig);
        DEBUG_LOG("SslService: Received global configuration");
        IniParse osla(OSLA_CONFIG);
        osla.Set("Instrument.AccountDepositLimit", (long) globalConfig.account_deposit_limit());
        DEBUG_LOG("Instrument.AccountDepositLimit : " << globalConfig.account_deposit_limit());
        if (globalConfig.has_payment_account_enable())
        {
            osla.Set("Global.PaymentAccountEnable", globalConfig.payment_account_enable());
        }
        else
        {
            osla.Set("Global.PaymentAccountEnable", true);
        }
        //osla.Set("Instrument.ValidTrainingCheckInTime", (long) globalConfig.valid_training_check_in_time());
        //osla.Set("Instrument.WorkingTime", globalConfig.working_time());
        //osla.Set("Client.UpdateUrl", globalConfig.client_software_update_url());
        //osla.Set("Client.AutoUpdate", (long) globalConfig.client_software_auto_update());

        if (true == globalConfig.has_ftp_host()) {
            osla.Set("Ftp.enable", true);
            osla.Set("Ftp.sambapath", std::string("/storage/sd0"));
            osla.Set("Ftp.maxspeed", (long) 200000);
            osla.Set("Ftp.name", globalConfig.ftp_user_name());
            osla.Set("Ftp.passwd", globalConfig.ftp_passwd());
            osla.Set("Ftp.rootpath", globalConfig.ftp_root_url());
            osla.Set("Ftp.host", globalConfig.ftp_host());
            osla.Set("Ftp.port", globalConfig.ftp_port());
        } else {
            osla.Set("Ftp.enable", false);
        }

        if (true == globalConfig.has_capturetime()) {
            long t = globalConfig.capturetime();
            t = (0 >= t ? 30 : t);
            osla.Set("Global.CaptureTime", t);
        }

        if (true == globalConfig.has_servicetime()) {
            long t = globalConfig.servicetime();
            t = (0 >= t ? 300 : t);
            osla.Set("Global.ServiceTime", t);
        }

        if (globalConfig.has_working_time())
        {
            parseWorkingTime(QString::fromStdString(globalConfig.working_time()), osla);
        }

        osla.SaveFile();
        SslService* service = context->getSslService();
        service->EmitSettingsUpdated();

        status = true;
    } while (false);


    createResponseCmd(status,fin,  fout);
    return status;
}

bool GlobalConfig::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void GlobalConfig::reset() {

}

void GlobalConfig::parseWorkingTime(QString workingTime, IniParse &osla)
{
    QStringList params = workingTime.split(";");
    if (params.count() > 0)
    {
        foreach (QString param, params) {
            QStringList paramValue = param.split("=");
            if (paramValue.count() == 2)
            {
                if (paramValue[0] == "enableDepositUser")
                {
                    if (paramValue[1] == "1")
                    {
                        osla.Set("Global.enableDepositUser", true);
                    }
                    else
                    {
                        osla.Set("Global.enableDepositUser", false);
                    }
                }
            }
        }
    }
}
