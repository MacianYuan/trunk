#include <QtCore/QDateTime>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>

#include <controller/Base/GlobalData.h>
#include <controller/Base/SmallDefine.h>
#include <controller/Base/NormalTools.h>

#include <tools/log.h>
#include <deviceinfo/enum.h>
#include "controller/Context/BookInformation.h"
#include "controller/Context/UserInformation.h"
#include "controller/Context/InstrumentStatus.h"
#include "controller/Context/ServiceEvent.h"
#include "controller/Context/WindowContext.h"
#include "sqlworker/sqlworker.h"

enum Current_book_status {
    BEFORE_A = 0x01,
    A_TO_C = 0x01 << 1,
    ARRIAVAL_A = 0x01 << 2,
    A_TO_B = 0x01 << 3,
    ARRIAVAL_B = 0x01 << 4,
    B_TO_C = 0x01 << 5,
    C_TO_D = 0x01 << 6,
    E_TO_B = 0x01 << 7
};

BookInformation::BookInformation(QObject *parent /*= nullptr*/) : QObject(parent) {
    currentBooking.Clear();
    currentBookStatus = 0;
}

BookInformation::~BookInformation() {

}

void BookInformation::InitLableInfo(QObject *status, QObject *period, QObject *user, QObject *more) {
    ZY_ASSERT(nullptr != status, "status label is null");
    ZY_ASSERT(nullptr != period, "period label is null");
    ZY_ASSERT(nullptr != user, "user label is null");
    ZY_ASSERT(nullptr != more, "more booking label is null");
    m_StatusLabel = (QLabel *) status;
    m_PeriodLable = (QLabel *) period;
    m_UserLable = (QLabel *) user;
    m_MoreLable = (QToolButton *) more;
}

bool BookInformation::IsHaveBook() {
    return (currentBooking.id() != 0);
}

biotech::osla::Booking BookInformation::GetCurrentBook() {
    return currentBooking;
}

biotech::osla::BookingList BookInformation::GetCurrentBookList() {
    return recentBookings;
}

biotech::osla::BookingList BookInformation::GetRecentBookList() {
    biotech::osla::BookingList l;
    for (int i = 0; i < recentBookings.booking_size(); ++i) {
        biotech::osla::Booking * b = l.add_booking();
        const biotech::osla::Booking & bb = recentBookings.booking(i);
        b->CopyFrom(bb);

    }
    if (currentBooking.id() != 0) {
        biotech::osla::Booking * b = l.add_booking();
        b->CopyFrom(currentBooking);
    }

    return l;
}


#define BookBindFun(fun) std::bind(&BookInformation::fun, this, std::placeholders::_1, std::placeholders::_2)

bool BookInformation::RefreshShowBookInfo(UserInformation *ui, InstrumentStatus *is) {
    if (false == IsHaveBook()) {
        return false;
    }

    QDateTime currentDateTime = QDateTime::currentDateTime();
    currentDateTime_t = currentDateTime.toTime_t();
    uint currentTime_t = abs(currentDateTime.time().secsTo(QTime(0, 0, 0)));

    struct _TempNode_ {
        bool status;
        std::function<bool(UserInformation *ui, InstrumentStatus *is)> fun;
    };
    /*
     *    BEFORE_A = 0x01,
     *    A_TO_C = 0x01 << 1,
     *    ARRIAVAL_A = 0x01 << 2,
     *    A_TO_B = 0x01 << 3,
     *    ARRIAVAL_B = 0x01 << 4,
     *    B_TO_C = 0x01 << 5,
     *    C_TO_D = 0x01 << 6
     *
     *                                          预约开始                                       预约结束
     *                                             |                                            |
     *                  提示用户，即将挤占他人预约     |   预约真实开始                               |
     *                                   |   60s   |   60s  |                                   |
     *                                   |    |    |    |   |                                   |
     *                                   v/-------\v/------\v                                   v
     * ----------------------A-----------E---------B--------D-----------C----------------------KO----------->
     *                        \-------------------/|\-------------------/
     *                                 |           |        |
     *                             预约保护时间  开始时间   违约时间段
     *
     * */

    int bookingHolding = is->GetBookingHoldingTime();
    int bookingBreak = is->GetBookingBreakTime();
    if (bookingBreak == 0) {
        bookingBreak = (currentBooking.end_time() - currentBooking.start_time()) / 60;
    } else if (bookingBreak < 0) {
        int i = currentBooking.end_time() - currentBooking.start_time();
        if (i <= abs(bookingBreak)) {
            //2018-6-1,郑总要求:违约小于整个预约时间时，则违约时间按整个预约时间算
            bookingBreak = (currentBooking.end_time() - currentBooking.start_time()) / 60;
            //bookingBreak = bookingHolding;
        } else {
            bookingBreak = (i + bookingBreak * 60) / 60;
        }
    } else if (bookingBreak > 0) {
        int i = (currentBooking.end_time() - currentBooking.start_time()) / 60;
        if (bookingBreak > i) {
            bookingBreak = i;
        }
    }


    /// 需要考虑时间跳转问题
    _TempNode_ arr[] = {
            // 刷新预约倒计时
            {
                    (currentDateTime_t < (uint) (currentBooking.start_time() - bookingHolding * 60)) &&
                    ((BEFORE_A & currentBookStatus) == 0),
                    BookBindFun(RefreshStatusLableBeforeA)
            },
            // 从显示预约倒计时改为显示预约锁定
            {
                    (true == IsBookHolding(currentDateTime_t, is)) &&
                    ((A_TO_C & currentBookStatus) == 0),
                    BookBindFun(RefreshStatusLableAToC)
            },
            // 预约锁定开始时刻
            {
                    (5 >= abs(currentBooking.start_time() - bookingHolding * 60 - currentDateTime_t)) &&
                    ((ARRIAVAL_A & currentBookStatus) == 0),
                    BookBindFun(RefreshStatusLableArrivalA)
            },
            // 预约保护开始时间 至 预约开始时间，需要运行这个时间点，说明有重启操作
            {
                    ((currentBooking.start_time() - bookingHolding * 60) <= currentDateTime_t) &&
                    ((long) currentBooking.start_time() > (long) currentDateTime_t) &&
                    ((A_TO_B & currentBookStatus) == 0),
                    BookBindFun(RefreshStatusLableArrivalA)
            },

            // 用户正在使用，提示用户即将占用他人预约
            {
                    (120 >= currentBooking.start_time() - currentDateTime_t) &&
                    ((E_TO_B & currentBookStatus) == 0),
                    BookBindFun(RefreshStatusLableDToB)
            },
            // 预约开始时刻
            {
                    (currentBooking.start_time() <= currentDateTime_t) &&
                    ((ARRIAVAL_B & currentBookStatus) == 0),
                    BookBindFun(RefreshStatusLableArrivalB)
            },
            // 从预约开始到预约保护结束，如果需要运行这个时间点，说明有重启操作，导致没有经历预约开始这个时间点
            {
                    (currentBooking.start_time() <= currentDateTime_t) &&
                    ((currentBooking.start_time() + bookingBreak * 60) > currentDateTime_t) &&
                    ((B_TO_C & currentBookStatus) == 0),
                    BookBindFun(RefreshStatusLableArrivalB)
            },

            // 预约违约结束时刻判断
            {
                    (currentDateTime_t >= (uint) (currentBooking.start_time() + bookingBreak * 60)) &&
                    ((C_TO_D & currentBookStatus) == 0),
                    BookBindFun(RefreshStatusLableCToD)
            }
    };

    for (int i = 0; i < (int) (sizeof(arr) / sizeof(arr[0])); ++i) {
        if (true == arr[i].status) {
            DEBUG_LOG("do booking check " << i);
            arr[i].fun(ui, is);
            break;
        }
    }

    return true;
}

bool BookInformation::UpdateBookInfo(UserInformation *ui, InstrumentStatus *is, biotech::osla::Booking &book) {
    User &currentUser = ui->GetCurrentUser();

    if (false == IsBookHolding(currentUser.timestamp, is)) {
        DEBUG_LOG(__FUNCTION__ << ":" << __LINE__);
        return false;
    }

    SqlWorker* m_SqlWork = SqlWorker::GetSqlWorkByThread();
    biotech::osla::Session &session = ui->GetSessionInfo();
    User currentBookingUser = m_SqlWork->getUserInfoById((uint) currentBooking.user_id());

    if (currentUser.id != currentBookingUser.id && currentUser.timestamp >= (uint) currentBooking.start_time()) {
        DEBUG_LOG("MainWindow: User" << currentUser.id << "penalized for occupying booking"
                                     << (uint) currentBooking.id());
        PROMPTINFO(parent(), tr("You have a reservation with someone else"));//您已占用他人预约
        biotech::osla::Session &session = ui->GetSessionInfo();
        session.set_penalty_flag(true); // 占用他人预约，设置使用记录的惩罚标志
        currentBooking.set_status(biotech::osla::BOOKING_STATUS_OCCUPIED);
        m_SqlWork->addSession(session, false);
        m_SqlWork->updateBookingStatus(currentBooking, false); // 更新数据库预约状态
        return false;
    }

    if (currentUser.id == currentBookingUser.id && currentBooking.type() == session.type()) {
        DEBUG_LOG("MainWindow: Booking" << (uint) currentBooking.id() << "completed");
        PROMPTINFO(parent(), tr("To make an appointment to complete"));//预约完成
        session.set_booking_id(currentBooking.id()); // 关联使用记录和预约记录
        currentBooking.set_status(biotech::osla::BOOKING_STATUS_COMPLETED); // 预约完成
        m_SqlWork->addSession(session, false);
        m_SqlWork->updateBookingStatus(currentBooking, true); // 更新数据库预约状态，并结束该预约

        //book.CopyFrom(currentBooking);
        windowContext->UpdateNowUserBooking(currentBooking.id());

        ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_BOOKING_COMPLETED, currentBooking.id());
        windowContext->UpdateNowUserBooking(currentBooking.id());
        currentBooking.Clear();
        UpdateCurrentBooking();

        return true;
    }

    return true;
}

bool BookInformation::IsBookHolding(uint timestamp, InstrumentStatus *is) {
    if (currentBooking.id() == 0) {
        return false;
    }

    int bookingBreak = is->GetBookingBreakTime();
    int startTime = (uint) (currentBooking.start_time() - is->GetBookingHoldingTime() * 60);
    int endTime = 0;

    if (bookingBreak > 0) {
        endTime = (uint) (currentBooking.start_time() + bookingBreak * 60);
        if (endTime > currentBooking.end_time()) {
            endTime = currentBooking.end_time();
        }
    } else if (bookingBreak == 0) {
        endTime = currentBooking.end_time();
    } else if (bookingBreak < 0) {
        int i = currentBooking.end_time() - currentBooking.start_time();
        if (i <= abs(bookingBreak)) {
            endTime = currentBooking.start_time() + is->GetBookingHoldingTime() * 60;
        } else {
            endTime = currentBooking.end_time() + is->GetBookingBreakTime() * 60;
        }
    }

    return timestamp >= startTime && timestamp < endTime;
}

bool BookInformation::IsCurrentBookUser(User &user) {
    if (currentBooking.id() == 0) {
        return false;
    }

    SqlWorker* m_SqlWork = SqlWorker::GetSqlWorkByThread();
    User currentBookingUser = m_SqlWork->getUserInfoById((uint) currentBooking.user_id());
    if (currentBookingUser.id != user.id) {
        return false;
    }
    return true;
}

bool BookInformation::UpdateCurrentBooking() {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    currentBooking.Clear();
    SqlWorker* m_SqlWork = SqlWorker::GetSqlWorkByThread();
    currentBooking = m_SqlWork->getCurrentBooking(currentDateTime);

    if (0 != currentBooking.id()) {//时段
        QString currentBookingTime(tr("Time frame: %1 to %2")
                                           .arg(QDateTime::fromTime_t((uint) currentBooking.start_time()).toString(
                                                   "M/d hh:mm"))
                                           .arg(QDateTime::fromTime_t((uint) currentBooking.end_time()).toString(
                                                   "M/d hh:mm")));
        m_PeriodLable->setText(currentBookingTime);
        User currentBookingUser = m_SqlWork->getUserInfoById((uint) currentBooking.user_id());
        //用户            未知
        QString currentBookingUserInfo = tr("User: %1(%2)").
                arg((currentBookingUser.id == 0) ? tr("Unknown") : currentBookingUser.fullName).
                arg(currentBookingUser.phone);
        m_UserLable->setText(currentBookingUserInfo);
    } else {
        m_StatusLabel->setText("");
        m_PeriodLable->setText("");
        m_UserLable->setText("");
    }

    // 查询最近一周内的预约。如果最近一周内的预约数大于1，则允许显示日历
    recentBookings.Clear();
    recentBookings = m_SqlWork->getRecentBookingsInDays(currentDateTime, 7);
//    if (recentBookings.booking_size() > 0) {
//        m_MoreLable->setEnabled(true);
//    } else {
//        m_MoreLable->setEnabled(false);
//    }

    currentBookStatus = 0;

    return true;
}

bool BookInformation::RefreshStatusLableBeforeA(UserInformation *ui, InstrumentStatus *is) {
    // 刷新预约倒计时
    (void *) ui, (void *) is;
    int countDownSecs = (uint) currentBooking.start_time() - currentDateTime_t;
    //预约将在 %1 后开始
    m_StatusLabel->setText(tr("Appointments will begin after %1").arg(NTG()->FormatTimeString(countDownSecs)));
    SqlWorker* m_SqlWork = SqlWorker::GetSqlWorkByThread();
    User currentBookingUser = m_SqlWork->getUserInfoById((uint) currentBooking.user_id());
    QString currentBookingUserInfo = tr("User: %1(%2)").
            arg((currentBookingUser.id == 0) ? tr("Unknown") : currentBookingUser.fullName).//未知
            arg(currentBookingUser.phone);
    m_UserLable->setText(currentBookingUserInfo);
    currentBookStatus = 0;
    return true;
}

bool BookInformation::RefreshStatusLableAToC(UserInformation *ui, InstrumentStatus *is) {
    (void *) ui;
    currentBookStatus |= A_TO_C;
    m_StatusLabel->setText(tr("Reservation locked in"));//

    SqlWorker* m_SqlWork = SqlWorker::GetSqlWorkByThread();
    User currentBookingUser = m_SqlWork->getUserInfoById((uint) currentBooking.user_id());
    QString currentBookingUserInfo = tr("User: %1(%2)").
            arg((currentBookingUser.id == 0) ? tr("Unknown") : currentBookingUser.fullName).
            arg(currentBookingUser.phone);
    m_UserLable->setText(currentBookingUserInfo);

    if (false == is->GetInUseStatus()) {
        // 若当前无人使用, 产生系统事件，通知预约用户
        ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_BOOKING_LOCKED, currentBooking.id());
    }
    return true;
}

bool BookInformation::RefreshStatusLableArrivalA(UserInformation *ui, InstrumentStatus *is) {
    currentBookStatus |= ARRIAVAL_A | A_TO_B;

    if (true == is->GetInUseStatus()) {
        User &currentUser = ui->GetCurrentUser();
        SqlWorker* m_SqlWork = SqlWorker::GetSqlWorkByThread();
        User currentBookingUser = m_SqlWork->getUserInfoById((uint) currentBooking.user_id());
        biotech::osla::Session &session = ui->GetSessionInfo();

        if (currentUser.id != currentBookingUser.id) {
            DEBUG_LOG("MainWindow: Booking" << currentBooking.id() << "starting in" << is->GetBookingHoldingTime()
                                            << "minutes");
            //请注意, 最近预约将在 %1 分钟后开始
            PROMPTINFO(parent(), tr("Please note that the most recent appointment will begin in %1 minutes").arg(is->GetBookingHoldingTime()),
                       "media/bookingstarting.mp3");
            ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_BOOKING_LOCKED_IN_USED,
                                                        currentBooking.id());
        } else if (currentBooking.type() != session.type()) { // 若当前使用者为预约用户，预约类型与使用类型不一致，也提示
            //请注意, 最近预约将在 %1 分钟后开始
            PROMPTINFO(parent(), tr("Please note that the most recent appointment will begin in %1 minutes").arg(is->GetBookingHoldingTime()),
                       "media/bookingstarting.mp3");
            ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_BOOKING_LOCKED_IN_USED_SELF,
                                                        currentBooking.id());
        }
    }

    return true;
}

bool BookInformation::RefreshStatusLableDToB(UserInformation *ui, InstrumentStatus *is) {
    currentBookStatus |= E_TO_B;
    SqlWorker* m_SqlWork = SqlWorker::GetSqlWorkByThread();
    do {
        // 仪器空闲
        if (false == is->GetInUseStatus()) {
            // 检查仪器是否启用，否则标记该预约状态为终止，待预约锁定结束时再确认
            if (biotech::osla::INSTRUMENT_STATUS_ACTIVE != is->GetInstrumentStatus()) {
                DEBUG_LOG("MainWindow: Booking" << (uint) currentBooking.id()
                                                << "will be terminated for instrument status"
                                                << is->GetInstrumentStatus());
                currentBooking.set_status(biotech::osla::BOOKING_STATUS_TERMINATED);
                m_SqlWork->updateBookingStatus(currentBooking, false); // 更新数据库预约状态
            }
            break;
        }

        // 仪器使用中
        User &currentUser = ui->GetCurrentUser();
        User currentBookingUser = m_SqlWork->getUserInfoById((uint) currentBooking.user_id());
        if (currentUser.id != currentBookingUser.id) { // 若当前使用者非预约用户，给出惩罚提醒
            DEBUG_LOG("MainWindow: User" << currentUser.id << "near booking, maybe occupying booking"
                                         << (uint) currentBooking.id());
            //您即将占用他人预约, 请尽快结束实验
            PROMPTINFO(parent(), tr("You are about to occupy someone else appointment,\n please finish the experiment as soon as possible"), "media/TipUserAboutBooking.mp3", true, 500);
        }
    } while (false);

    return true;
}

bool BookInformation::RefreshStatusLableArrivalB(UserInformation *ui, InstrumentStatus *is) {
    currentBookStatus |= ARRIAVAL_B | B_TO_C;
    SqlWorker* m_SqlWork = SqlWorker::GetSqlWorkByThread();
    do {
        // 仪器空闲
        if (false == is->GetInUseStatus()) {
            // 检查仪器是否启用，否则标记该预约状态为终止，待预约锁定结束时再确认
            if (biotech::osla::INSTRUMENT_STATUS_ACTIVE != is->GetInstrumentStatus()) {
                DEBUG_LOG("MainWindow: Booking" << (uint) currentBooking.id()
                                                << "will be terminated for instrument status"
                                                << is->GetInstrumentStatus());
                currentBooking.set_status(biotech::osla::BOOKING_STATUS_TERMINATED);
                m_SqlWork->updateBookingStatus(currentBooking, false); // 更新数据库预约状态
                m_SqlWork->endExpiredBookings(QDateTime::currentDateTime());
            }
            break;
        }

        // 仪器使用中
        User &currentUser = ui->GetCurrentUser();
        User currentBookingUser = m_SqlWork->getUserInfoById((uint) currentBooking.user_id());
        biotech::osla::Session &session = ui->GetSessionInfo();
        DEBUG_LOG("current booking user is : " << currentBooking.user_id());

        if (currentUser.id != currentBookingUser.id) { // 若当前使用者非预约用户，给出惩罚提醒
            DEBUG_LOG("MainWindow: User" << currentUser.id << "penalized for occupying booking "
                                         << (uint) currentBooking.id());
            //您已占用他人预约, 本次使用将受惩罚
            PROMPTINFO(parent(), tr("You have already reserved someone else appointment,\n this time will be punished"), "media/bookingoccupied.mp3");
            session.set_penalty_flag(true); // 占用他人预约，设置使用记录的惩罚标志
            m_SqlWork->addSession(session, false);
            currentBooking.set_status(biotech::osla::BOOKING_STATUS_OCCUPIED);
            m_SqlWork->updateBookingStatus(currentBooking, false);
        } else if (currentUser.id == currentBookingUser.id &&
                   currentBooking.type() == session.type()) { // 若当前使用者为预约用户,预约类型与使用类型一致，则预约完成
            DEBUG_LOG("MainWindow: Booking" << (uint) currentBooking.id() << "completed");
            PROMPTINFO(parent(), tr("To make an appointment to complete"));//预约完成
            session.set_booking_id(currentBooking.id()); // 关联使用记录和预约记录
            m_SqlWork->addSession(session, false);
            currentBooking.set_status(biotech::osla::BOOKING_STATUS_COMPLETED);
            m_SqlWork->updateBookingStatus(currentBooking, true);
            ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_BOOKING_COMPLETED,
                                                        currentBooking.id());
            windowContext->UpdateNowUserBooking(currentBooking.id());
            currentBooking.Clear();
            UpdateCurrentBooking();
        }
    } while (false);

    return true;
}

bool BookInformation::RefreshStatusLableCToD(UserInformation *ui, InstrumentStatus *is) {
    currentBookStatus |= C_TO_D;

    SqlWorker* m_SqlWork = SqlWorker::GetSqlWorkByThread();

    if ((uint) currentBooking.status() == biotech::osla::BOOKING_STATUS_OCCUPIED) {

        DEBUG_LOG("MainWindow: Booking " << (uint) currentBooking.id() << " occupied by user "
                                         << ui->GetCurrentUser().id);
        PROMPTINFO(parent(), tr("Appointments are crowded out"));//预约被挤占
        m_SqlWork->updateBookingStatus(currentBooking, true);
        ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_BOOKING_OCCUPIED, currentBooking.id());

    } else if ((uint) currentBooking.status() == biotech::osla::BOOKING_STATUS_TERMINATED) {

        DEBUG_LOG("MainWindow: Booking " << (uint) currentBooking.id() << " terminated for instrument status "
                                         << is->GetInstrumentStatus());
        PROMPTINFO(parent(), tr("To make an appointment to terminate"));//预约终止
        m_SqlWork->updateBookingStatus(currentBooking, true);
        ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_BOOKING_TERMINATED,
                                                    currentBooking.id());

    } else {

        DEBUG_LOG("MainWindow: Booking " << (uint) currentBooking.id() << " absent");
        PROMPTINFO(parent(), tr("To make an appointment date"));//预约过期
        currentBooking.set_status(biotech::osla::BOOKING_STATUS_ABSENT);
        m_SqlWork->updateBookingStatus(currentBooking, true);
        ServiceEvent::GetInstance()->AddSystemEvent(biotech::osla::SYSTEM_EVENT_BOOKING_ABSENT, currentBooking.id());

    }

    currentBooking.Clear();
    UpdateCurrentBooking();

    return true;
}

void BookInformation::InitWindowContext(WindowContext *context) {
    this->windowContext = context;
}
