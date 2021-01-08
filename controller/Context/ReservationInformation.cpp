//
// Created by 何继胜 on 2017/5/9.
//

#include <controller/Base/GlobalData.h>
#include <tools/log.h>
#include "ReservationInformation.h"
#include "InstrumentStatus.h"

ReservationInformation::ReservationInformation(QObject *parent) : QObject(parent) {
}

const biotech::osla::ReservationList &ReservationInformation::getReservationList() const {
    return reservationList;
}

void ReservationInformation::setReservationList(const biotech::osla::ReservationList &reservationList) {
    this->reservationList = reservationList;
}

bool ReservationInformation::isHasReservation() const {
    return hasReservation;
}

int ReservationInformation::getReservationIndex() const {
    return reservationIndex;
}

void ReservationInformation::updateReservation() {
    DEBUG_LOG("MainWindow: Check for reservations");
    QDateTime currentDateTime = QDateTime::currentDateTime();
    uint currentTime_t = abs(currentDateTime.time().secsTo(QTime(0, 0, 0)));
    QDate currentDate = currentDateTime.date();

    SqlWorker *m_SqlWork = SqlWorker::GetSqlWorkByThread();
    hasReservation = m_SqlWork->hasReservation(currentDate, reservationList);

    if (false == hasReservation) {
        DEBUG_LOG("MainWindow: No reservation");
        return;
    }

    reservationIndex = 0;

    while (reservationIndex < reservationList.reservation_size()) {
        if (currentTime_t < (uint) reservationList.reservation(reservationIndex).end_time()) {
            break;
        }

        reservationIndex++;
    }

    if (reservationIndex >= reservationList.reservation_size()) {
        hasReservation = false;
        reservationIndex = 0;
        DEBUG_LOG("MainWindow: No active reservation");
    } else {
        DEBUG_LOG("MainWindow: Active reservation found");
    }
}

void ReservationInformation::refreshReservation(InstrumentStatus *instrumentStatus) {

    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDate currentDate = currentDateTime.date();

    uint currentDateTime_t = currentDateTime.toTime_t();                     /// 到1970年的秒数
    uint currentTime_t = abs(currentDateTime.time().secsTo(QTime(0, 0, 0))); /// 到0点的秒数

    // 切换专享状态
    // 培训和维护状态优先级高于专享，不会切换
    if (InstrumentStatus::NORMAL == instrumentStatus->GetWorkStatus() && hasReservation) {
        // 判断专享时间是否开始
        if (currentTime_t >= (uint) reservationList.reservation(reservationIndex).start_time()) {
            instrumentStatus->SetWorkStatus(InstrumentStatus::RESERVATION);
            emit SetCurrentStateInfo();
        }
    } else if (InstrumentStatus::RESERVATION == instrumentStatus->GetWorkStatus()) {
        // 判断专享时间是否结束
        if (currentTime_t >= (uint) reservationList.reservation(reservationIndex).end_time()) {
            instrumentStatus->SetWorkStatus(InstrumentStatus::NORMAL);
            emit SetCurrentStateInfo();

            if (++reservationIndex >= reservationList.reservation_size()) {
                hasReservation = false;
                reservationIndex = 0;
            }
        }
    }
}

int ReservationInformation::getGroupId() const {
    if (false == hasReservation) {
        return 0;
    }
    return reservationList.reservation(reservationIndex).group_id();
}
