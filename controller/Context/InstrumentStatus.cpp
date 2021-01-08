#include <QtGui/QLabel>
#include <deviceinfo/version.h>
#include <deviceinfo/enum.h>
#include <controller/Base/NormalTools.h>

#include "controller/Context/InstrumentStatus.h"
#include "controller/Context/ServiceEvent.h"
#include "tools/IniParse.h"

InstrumentStatus::InstrumentStatus(QObject *parent /*= NULL*/) : QObject(parent) {
    InitBaseInfo();
}

void InstrumentStatus::InitBaseInfo() {
    m_InstrumentDetail.Clear();
    IniParse ini(OSLA_CONFIG);
    m_InstrumentDetail.set_id(ini.GetLong("Instrument.ID"));
    m_InstrumentDetail.set_name(ini.GetString("Instrument.Name"));
    m_InstrumentDetail.set_min_interval(ini.GetLong("Instrument.MinInterval"));
    m_InstrumentDetail.set_booking_holding_time(ini.GetLong("Instrument.BookingHoldingTime"));
    m_InstrumentDetail.set_booking_break_time(ini.GetLong("Instrument.BookingBreakTime"));
    m_InstrumentDetail.set_alert_time(ini.GetLong("Instrument.AlertTime"));
    m_InstrumentDetail.set_off_delay_time(ini.GetLong("Instrument.OffDelayTime"));
    m_InstrumentDetail.set_min_off_time(ini.GetLong("Instrument.MinOffTime"));
    m_InstrumentDetail.set_alert_current(ini.GetLong("Instrument.AlertCurrent"));
    m_InstrumentDetail.set_status(ini.GetLong("Instrument.Status"));
}

InstrumentStatus::~InstrumentStatus() {

}

void InstrumentStatus::SetInUseStatus(bool inUsed) {
    m_InUsed = inUsed;
}

bool InstrumentStatus::GetInUseStatus() {
    return m_InUsed;
}

void InstrumentStatus::SetWorkStatus(WorkingState st) {
    m_WorkingStatus = st;
}

void InstrumentStatus::SetWorkStatusBySessionType(int sessionType) {
    switch (sessionType) {
        case biotech::osla::SESSION_TYPE_NORMAL:
        case biotech::osla::SESSION_TYPE_ASSISTANCE:
            m_WorkingStatus = NORMAL;
            break;
        case biotech::osla::SESSION_TYPE_TRAINING:
            m_WorkingStatus = TRAINING;
            break;
        case biotech::osla::SESSION_TYPE_MAINTENANCE:
            m_WorkingStatus = MAINTENANCE;
            break;
        case biotech::osla::SESSION_TYPE_ENTRUST:
            m_WorkingStatus = ENTRUST;
            break;
        case biotech::osla::SESSION_TYPE_EDUCATION:
            m_WorkingStatus = EDUCATION;
            break;
    }

    // 强制设置仪器状态为启用，忽略从配置文件里获取的值
    m_InstrumentDetail.set_status(biotech::osla::INSTRUMENT_STATUS_ACTIVE);
}

InstrumentStatus::WorkingState InstrumentStatus::GetWorkStatus() {
    return m_WorkingStatus;
}

void InstrumentStatus::ShowWorkStatus(QObject *object, int cuit) {
    QLabel *currentStateLabel = (QLabel *) object;

    switch (m_InstrumentDetail.status()) {
        case biotech::osla::INSTRUMENT_STATUS_ACTIVE: // 启用
            switch (m_WorkingStatus) {
                case NORMAL: // 一般情况
                    currentStateLabel->setText(true == m_InUsed ? tr("Experiment") : tr("Idle"));//tr("实验中") : tr("空闲")
                    break;
                case RESERVATION: // 专享时段
                    currentStateLabel->setText(true == m_InUsed ? tr("Experiment (Exclusive)") : tr("Idle (Exclusive)"));
                    break;
                case TRAINING: // 培训中
                    currentStateLabel->setText(tr("Training(Sign in %1)").arg(cuit));//Training
                    break;
                case EDUCATION: // 教学
                    currentStateLabel->setText(tr("Education(Sign in %1)").arg(cuit));
                    break;
                case MAINTENANCE: // 维护中
                    currentStateLabel->setText(tr("Maintenance"));
                    break;
                case ENTRUST://项目委托实验中
                    currentStateLabel->setText(tr("Entrust"));
                    break;
                default: // 其他
                    currentStateLabel->setText(tr("Unknown"));
                    break;
            }
            break;

        case biotech::osla::INSTRUMENT_STATUS_FAULTY: // 故障
            if (m_WorkingStatus == MAINTENANCE) {
                currentStateLabel->setText(tr("Error(Maintenance)"));
            } else {
                currentStateLabel->setText(tr("Error"));
            }
            break;

        case biotech::osla::INSTRUMENT_STATUS_INACTIVE: // 未启用
            currentStateLabel->setText(tr("Inactive"));
            break;

        case biotech::osla::INSTRUMENT_STATUS_OUTOFSERVICE: // 停用
            currentStateLabel->setText(tr("Out of service"));
            break;

        default: // 其他
            currentStateLabel->setText(tr("Unknow"));
            break;
    }

}

void InstrumentStatus::SetInstrumentStatus(int status) {
    m_InstrumentDetail.set_status(status);
}

int InstrumentStatus::GetInstrumentStatus() {
    return m_InstrumentDetail.status();
}

void InstrumentStatus::SetNetWorkStatus(bool status) {
    m_NetWorkStatus = status;
}

bool InstrumentStatus::GetNetWorkStatus() {
    return m_NetWorkStatus;
}

int InstrumentStatus::GetId() {
    return m_InstrumentDetail.id();
}

int InstrumentStatus::GetBookingHoldingTime() {
    return m_InstrumentDetail.booking_holding_time();
}

int InstrumentStatus::GetBookingBreakTime() {
    return m_InstrumentDetail.booking_break_time();
}

int InstrumentStatus::GetAlertTime() {
    return m_InstrumentDetail.alert_time();
}

int InstrumentStatus::GetMinInterval() {
    return m_InstrumentDetail.min_interval();
}

bool InstrumentStatus::InstrumentStatusCheck(User &user) {
    ConditionNode arr[] = {
            {(m_InstrumentDetail.status() == biotech::osla::INSTRUMENT_STATUS_FAULTY),
                    tr("Instrument faulty"),  "media/faulty.mp3",       biotech::osla::USER_ACTION_FAILED_INSTRUMENT_FAULTY},//仪器故障
            {(m_InstrumentDetail.status() == biotech::osla::INSTRUMENT_STATUS_INACTIVE),
                    tr("Instrument inactive"), "media/inactive.mp3",     biotech::osla::USER_ACTION_FAILED_INSTRUMENT_INACTIVE},//仪器未启用
            {(m_InstrumentDetail.status() == biotech::osla::INSTRUMENT_STATUS_OUTOFSERVICE),
                    tr("Instrument out of service"), "media/outofservice.mp3", biotech::osla::USER_ACTION_FAILED_INSTRUMENT_OUTOFSERVICE}
    };

    for (int i = 0; i < (int) (sizeof(arr) / sizeof(arr[0])); ++i) {
        if (true == arr[i].status) {
            PROMPTINFO(parent(), arr[i].info, arr[i].soundPath, true, 500);
            ServiceEvent::GetInstance()->AddUserEvent(user, biotech::osla::USER_ACTION_START_SESSION, arr[i].errorCode);
            return false;
        }
    }

    return true;
}

bool InstrumentStatus::InstrumentStatusCheckExpectFaulty(User &user) {
    ConditionNode arr[] = {
            {(m_InstrumentDetail.status() == biotech::osla::INSTRUMENT_STATUS_INACTIVE),
                    tr("Instrument inactive"), "media/inactive.mp3",     biotech::osla::USER_ACTION_FAILED_INSTRUMENT_INACTIVE},
            {(m_InstrumentDetail.status() == biotech::osla::INSTRUMENT_STATUS_OUTOFSERVICE),
                    tr("Instrument out of service"), "media/outofservice.mp3", biotech::osla::USER_ACTION_FAILED_INSTRUMENT_OUTOFSERVICE}
    };

    for (int i = 0; i < (int) (sizeof(arr) / sizeof(arr[0])); ++i) {
        if (true == arr[i].status) {
            PROMPTINFO(parent(), arr[i].info, arr[i].soundPath, true, 500);
            ServiceEvent::GetInstance()->AddUserEvent(user, biotech::osla::USER_ACTION_START_SESSION, arr[i].errorCode);
            return false;
        }
    }

    return true;
}
