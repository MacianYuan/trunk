//
// Created by 何继胜 on 2017/5/9.
//

#ifndef OSLA_NEW_RESERVATIONINFORMATION_H
#define OSLA_NEW_RESERVATIONINFORMATION_H

#include <QtCore/QObject>
#include <protobuf/message.pb.h>
#include <sqlworker/sqlworker.h>

class InstrumentStatus;

class ReservationInformation : public QObject {
Q_OBJECT

public:
    explicit ReservationInformation(QObject *parent = nullptr);
    void updateReservation();
    void refreshReservation(InstrumentStatus* instrumentStatus);

    const biotech::osla::ReservationList &getReservationList() const;

    void setReservationList(const biotech::osla::ReservationList &reservationList);

    bool isHasReservation() const;

    int getReservationIndex() const;

    int getGroupId() const;

signals:
    void SetCurrentStateInfo();

private:
    bool hasReservation {false};
    int reservationIndex {0};
    biotech::osla::ReservationList reservationList;     /// 专享信息
};


#endif //OSLA_NEW_RESERVATIONINFORMATION_H
