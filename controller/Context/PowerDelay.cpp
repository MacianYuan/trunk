
#include <controller/Base/NormalTools.h>
#include <deviceinfo/tq2440.h>
#include <deviceinfo/version.h>
#include "tools/IniParse.h"
#include "controller/Context/PowerDelay.h"
#include "controller/Base/GlobalData.h"
PowerDelay::PowerDelay(QObject* parent /*= NULL*/) : QObject(parent)
{
}

PowerDelay::~PowerDelay()
{
}

void PowerDelay::StartUpParse()
{
    bool working;
    if (true == CheckInstrumentWorking(working) && false == working) {
        SetInstrumentEnabled(false);
    }
}

bool PowerDelay::StartSesssion()
{
    /// powerOnDelay大于0，说明还处于上电保护期间
    if (powerOnDelay > 0) {//"仪器上电延时保护,请等待%1秒后再试"
        if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0){
            PROMPTINFO((QWidget*)parent(), tr("仪器上电延时保护,请等待%1秒后再试").arg(powerOnDelay), "", true, 500);
        }else{
            PROMPTINFO((QWidget*)parent(), tr("Please wait for %1 second before trying again").arg(powerOnDelay), "", true, 500);
        }
        return false;
    }

    powerOnDelay = 0;
    powerOffDelay = 0;
    needCheckStopPower = false;

    return true;
}

bool PowerDelay::EndSession()
{
    powerOnDelay = 0;
    powerOffDelay = GetOffDelayTime();
    powerOffDelay = (powerOffDelay == 0 ? 1 : powerOffDelay);
    needCheckStopPower = false;

    return true;
}

bool PowerDelay::CheckInstrumentWorking(bool& working)
{
    int ret = checkworking();
    if (ret < 0) {
        return false;
    }
    working = (ret == 0 ? true : false);

    return true;
}

bool PowerDelay::SetInstrumentEnabled(bool enabled)
{
    return relay(true == enabled ? 0 : 1);
}

bool PowerDelay::RegularlyCheck()
{
    /// 上电保护
    if (powerOnDelay > 0) {
        powerOnDelay -= 1;
		if (powerOnDelay == 0) {
			NTG()->SetDeviceWorkStatusLed(false);
		}
    }

    // 延时关闭仪器电源
    if (powerOffDelay > 0  && --powerOffDelay == 0) {
        needCheckStopPower = true;
    }

    // 只有过了延时关闭的时间，才去检测到外接设备是否关闭了，再去断开电源
    bool working = false;
    if (true == needCheckStopPower && true == CheckInstrumentWorking(working) && false == working) {
        powerOffDelay = 0;
        SetInstrumentEnabled(false);
        powerOnDelay = GetMinOffTime(); /// 只有真的断电了，才需要上电延迟
        powerOnDelay = (powerOnDelay == 0 ? 1 : powerOnDelay);
        needCheckStopPower = false;
    }

    return true;
}

int PowerDelay::GetMinOffTime()
{
	IniParse osla(OSLA_CONFIG);
    return osla.GetLong("Instrument.MinOffTime");
}

int PowerDelay::GetOffDelayTime()
{
	IniParse osla(OSLA_CONFIG);
    return osla.GetLong("Instrument.OffDelayTime");
}
