#include <functional>
#include "tinsqualist.h"
#include "tools/IniParse.h"
#include <sslconnection/ClientContext.h>
#include <sslconnection/Frame.h>
#include <protobuf/message.pb.h>
#include <controller/Service/SslService.h>
#include "object/RegisterOperation.h"
#include "deviceinfo/enum.h"
#include "deviceinfo/version.h"
#include "tools/log.h"

DynClassRegister(TinsQuaList);

TinsQuaList::TinsQuaList(std::string name) : BaseObjectExt(name) {
    IniParse osla(OSLA_CONFIG);
    userQualification.readUserQualification(osla);
}

TinsQuaList::~TinsQuaList() {

}

bool TinsQuaList::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool TinsQuaList::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);

    DEBUG_LOG("TinsQuaList: received");
    auto fun = std::bind(&TinsQuaList::updateQuaList, this, std::placeholders::_1);
    bool status = ParseMessageType<biotech::osla::TinsQuaList>(fin->getMessageBody(), fun);
    if (true == status) {
        IniParse osla(OSLA_CONFIG);
        userQualification.writeUserQualification(osla);
        osla.SaveFile();
        QProcess::execute("sync");
        SslService *service = context->getSslService();
        service->EmitSettingsUpdated();
    }
    createResponseCmd(status, fin, fout);
    return status;
}

bool TinsQuaList::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void TinsQuaList::reset() {

}

bool TinsQuaList::updateQuaList(const biotech::osla::TinsQuaList &tinsQuaList)
{
    DEBUG_LOG("TinsQuaList: tinsquas_size():" << tinsQuaList.tinsquas_size());

    if (tinsQuaList.tinsquas_size() == 0)
    {
        return false;
    }

    for (int i = 0; i < tinsQuaList.tinsquas_size(); i++)
    {
        DEBUG_LOG("TinsQuaList: quaid," << tinsQuaList.tinsquas(i).quaid() << \
                  ", workTime:" << tinsQuaList.tinsquas(i).worktime() << ", status:"\
                  << tinsQuaList.tinsquas(i).status());
        switch (tinsQuaList.tinsquas(i).quaid())
        {
            case biotech::osla::USER_RIGHT_UNAUTH:
                if (tinsQuaList.tinsquas(i).worktime())
                {
                    userQualification.setUnauthorizedUserInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                }
                else
                {
                    userQualification.setUnauthorizedUserNotInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                }
                break;
            case biotech::osla::USER_RIGHT_NORMAL:
                if (tinsQuaList.tinsquas(i).worktime())
                {
                    userQualification.setNormalUserInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                }
                else
                {
                    userQualification.setNormalUserNotInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                }
                break;
            case biotech::osla::USER_RIGHT_SENIOR:
                if (tinsQuaList.tinsquas(i).worktime())
                {
                    userQualification.setSeniorUserInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                }
                else
                {
                    userQualification.setSeniorUserNotInWorkTime(tinsQuaList.tinsquas(i).status() == 1); //1:直接刷卡,2:需要预约
                }
                break;
        }
    }

    return true;
}

