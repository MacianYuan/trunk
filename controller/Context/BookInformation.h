
#ifndef BOOKINFORMATION_H
#define BOOKINFORMATION_H

#include <QtCore/QObject>
#include <protobuf/message.pb.h>
#include <deviceinfo/typedef.h>

class QLabel;
class QToolButton;
class UserInformation;
class InstrumentStatus;
class SqlWorker;
class WindowContext;

class BookInformation : public QObject
{
    Q_OBJECT
public:
    explicit BookInformation(QObject* parent = nullptr);
    ~BookInformation();
    
    /// <summary>
    /// 初始化预约需要展示的标签信息.
    /// </summary>
    /// <param name="status">状态.</param>
    /// <param name="period">时间段.</param>
    /// <param name="user">用户.</param>
    /// <param name="more">更多预约.</param>
    void InitLableInfo(QObject* status, QObject* period, QObject* user, QObject* more);
    void InitWindowContext(WindowContext* context);

    /// <summary>
    /// 当前是否有预约信息.
    /// </summary>
    /// <returns>有预约信息返回true，其它返回false.</returns>
    bool IsHaveBook();

    /// <summary>
    /// 获取当前预约.
    /// </summary>
    /// <returns>有预约，返回预约信息；无预约，返回空信息.</returns>
    biotech::osla::Booking GetCurrentBook();

    /// <summary>
    /// 获取最近7天预约列表.
    /// </summary>
    /// <returns>有预约，返回预约信息；无预约，返回空信息.</returns>
    biotech::osla::BookingList GetCurrentBookList();

     /// <summary>
    /// 获取最近的预约信息，包括现在的实验.
    /// </summary>
    /// <returns>有预约，返回预约信息；无预约，返回空信息.</returns>
    biotech::osla::BookingList GetRecentBookList();

    /// <summary>
    /// 刷新预约的展示信息.
    /// </summary>
    /// <param name="ui">用户信息.</param>
    /// <param name="is">设备信息.</param>
    /// <returns>成功返回true，其它返回false.</returns>
    bool RefreshShowBookInfo(UserInformation* ui, InstrumentStatus* is);

    /// <summary>
    /// 当用户登录之后，需要更新预约信息.
    /// </summary>
    /// <param name="ui">用户信息.</param>
    /// <param name="is">设备信息.</param>
    /// <returns>成功返回true，其它返回false.</returns>
    bool UpdateBookInfo(UserInformation* ui, InstrumentStatus* is, biotech::osla::Booking& book);

    /// <summary>
    /// 是否在预约保护范围内.
    /// <param name="is">设备信息.</param>
    /// </summary>
    /// <returns>是返回true，其它返回false.</returns>
    bool IsBookHolding(uint timestamp, InstrumentStatus* is);

    /// <summary>
    /// 是否是当前预约用户.
    /// </summary>
    /// <param name="user">用户信息.</param>
    /// <returns>是返回true，其它返回false.</returns>
    bool IsCurrentBookUser(User& user);

    /// <summary>
    /// 从同步服务器的数据，获取预约信息.
    /// </summary>
    /// <returns>成功返回true，其它返回false.</returns>
    bool UpdateCurrentBooking();

private:
    ///              A    B    C           D
    /// -------------|----|----|-----------|----
    /// B时间点位预约开始，D时间点预约结束。C-B == B-A
    /// A到B这段时间为预约保护时间，如果A时间点之前无用户登录，那么，在A到B这段时间，禁止用户登录；
    /// 并且，在时间到达A这个点的时候，想预约用户发送提醒
    bool RefreshStatusLableBeforeA(UserInformation* ui, InstrumentStatus* is);
    bool RefreshStatusLableAToC(UserInformation* ui, InstrumentStatus* is);
    bool RefreshStatusLableArrivalA(UserInformation* ui, InstrumentStatus* is);
    bool RefreshStatusLableDToB(UserInformation* ui, InstrumentStatus* is);
    bool RefreshStatusLableArrivalB(UserInformation* ui, InstrumentStatus* is);
    bool RefreshStatusLableCToD(UserInformation* ui, InstrumentStatus* is);

private:
    int currentBookStatus{ 0 };
    biotech::osla::Booking currentBooking;              /// 当前预约信息
    biotech::osla::BookingList recentBookings;          /// 最近一周预约信息列表

    QLabel* m_StatusLabel{ nullptr };             /// 预约状态信息
    QLabel* m_PeriodLable{ nullptr };             /// 预约时间信息
    QLabel* m_UserLable{ nullptr };               /// 预约用户信息
    QToolButton* m_MoreLable{ nullptr };          /// 更多预约信息

    uint currentDateTime_t;
    WindowContext* windowContext;
};

#endif //BOOKINFORMATION_H
