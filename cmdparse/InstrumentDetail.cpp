//
// Created by 何继胜 on 2017/4/24.
//

#include "cmdparse/InstrumentDetail.h"
#include <functional>
#include <sslconnection/ClientContext.h>
#include "controller/Service/SslService.h"
#include <sslconnection/Frame.h>
#include <tools/IniParse.h>
#include "object/RegisterOperation.h"
#include <tools/log.h>
#include <deviceinfo/version.h>

DynClassRegister(InstrumentDetail);

InstrumentDetail::InstrumentDetail(std::string name) : BaseObjectExt(name) {

}

InstrumentDetail::~InstrumentDetail() {

}

bool InstrumentDetail::createRequest(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

bool InstrumentDetail::parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
    ClientContext *context = dynamic_cast<ClientContext *>(m_Context);
    Frame *fin = dynamic_cast<Frame *>(dataIn);
    Frame *fout = dynamic_cast<Frame *>(dataOut);

    biotech::osla::InstrumentDetail instrumentDetail;
    QByteArray data = fin->getMessageBody();
    bool status = false;
    do {
        if (!instrumentDetail.ParseFromArray(data.data(), data.size())) {
            break;
        }

        IniParse ini(OSLA_CONFIG);
        if ((uint) instrumentDetail.id() != ini.GetLong("Instrument.ID")) {
            break;
        }
        DEBUG_LOG("SslService: Received instrument settings");

        ini.Set("Instrument.ID", (long) instrumentDetail.id());
        DEBUG_LOG("Instrument.Name is [" << instrumentDetail.name().c_str() << "]");
        ini.Set("Instrument.Name", instrumentDetail.name());
        ini.Set("Instrument.Status", (long) instrumentDetail.status());
        ini.Set("Instrument.MinInterval", (long) instrumentDetail.min_interval());
        ini.Set("Instrument.BookingHoldingTime", (long) instrumentDetail.booking_holding_time());
        ini.Set("Instrument.BookingBreakTime", (long) instrumentDetail.booking_break_time());
        ini.Set("Instrument.AlertTime", (long) instrumentDetail.alert_time());
        ini.Set("Instrument.BookingBreakTime", (long) instrumentDetail.booking_break_time());
        ini.Set("Instrument.WorkingTime", instrumentDetail.working_time());
        ini.Set("Instrument.BookingModelId", instrumentDetail.bookingmodelid());
        DEBUG_LOG("booking model id is : " << instrumentDetail.bookingmodelid());

        DEBUG_LOG("Instrument.TerImgUrl : : " << instrumentDetail.terimgurl());
        ini.Set("Instrument.TerImgUrl", instrumentDetail.terimgurl());

        if (true == instrumentDetail.has_loosemode()) {
            ini.Set("Instrument.LooseMode", instrumentDetail.loosemode());
        } else {
            ini.Set("Instrument.LooseMode", true);
        }

        if (instrumentDetail.has_off_delay_time()) {
            ini.Set("Instrument.OffDelayTime", (long) instrumentDetail.off_delay_time());
        } else {
            ini.Del("Instrument.OffDelayTime");
        }

        if (instrumentDetail.has_min_off_time()) {
            ini.Set("Instrument.MinOffTime", (long) instrumentDetail.min_off_time());
        } else {
            ini.Del("Instrument.MinOffTime");
        }

        if (instrumentDetail.has_alert_current()) {
            ini.Set("Instrument.AlertCurrent", (long) instrumentDetail.alert_current());
        } else {
            ini.Del("Instrument.AlertCurrent");
        }

        ini.SaveFile();

        SslService* service = context->getSslService();
        if (nullptr != service) {
            service->EmitSettingsUpdated();
        } else {

        }

        status = true;
    } while (false);


    createResponseCmd(status, fin, fout);
    return status;
}

bool InstrumentDetail::parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
    return false;
}

void InstrumentDetail::reset() {

}
