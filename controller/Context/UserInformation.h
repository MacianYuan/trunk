#ifndef USERINFORMATION_H
#define USERINFORMATION_H

#include <QtCore/QObject>
#include <deviceinfo/typedef.h>
#include "protobuf/message.pb.h"
#include "userqualification.h"

class SqlWorker;

class UserInformation : public QObject
{
    Q_OBJECT
public:
    explicit UserInformation(QObject* parent);
    ~UserInformation();

    ///要从配置中更新
    void updateSettings(void);

    /// <summary>
    /// 从数据库初始化用户信息.
    /// </summary>
    /// <returns>成功返回true，其它返回false.</returns>
    bool InitFromDB();

    /// <summary>
    /// 根据卡号，获取新的用户信息.
    /// </summary>
    /// <param name="cardSerial">卡号.</param>
    /// <returns>有对应的卡号，返回true，其它情况返回false.</returns>
    bool GetNewUserInfo(uint cardSerial);

    /// <summary>
    /// 获取关于新用户会话信息.
    /// </summary>
    /// <returns>成功返回true，其它返回false.</returns>
    bool GetNewSessionFromNewUser();

    /// <summary>
    /// 获取当前刷卡的用户信息.
    /// </summary>
    /// <returns>返回用户信息.</returns>
    User& GetNewUser();

    /// <summary>
    /// 获取正在使用设备的用户信息.
    /// </summary>
    /// <returns>返回用户信息.</returns>
    User& GetCurrentUser();

    /// <summary>
    /// 保存新用户为当前用户.
    /// </summary>
    void StartSession();

    /// <summary>
    /// 清除正在使用设备的用户信息.
    /// </summary>
    void EndSession();

    /// <summary>
    /// 新增培训签到记录.
    /// </summary>
    /// <param name="sessionId">会话ID.</param>
    /// <param name="mode">模式,0:培训,1:教学.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool AddTraningUser(qint64 sessionId);

    /// <summary>
    /// 获取参加培训的人数.
    /// </summary>
    /// <returns>培训的人数.</returns>
    int GetCountInTraining();

    /// <summary>
    /// 设置参加培训的人数.
    /// </summary>
    void SetCountInTraining(int num);

    /// <summary>
    /// 清除培训信息.
    /// </summary>
    void ClearCountInTraining();

    /// <summary>
    /// 新的刷卡用户与当前正在使用的用户是否是同一个.
    /// </summary>
    /// <returns>同一用户返回true，其它返回false.</returns>
    bool CurrentUserIsNewUser();

    /// <summary>
    /// 当前用户是否是管理员.
    /// </summary>
    /// <returns>是管理员返回true，其它返回false.</returns>
    bool CurrentUserIsAdmin();

    /// <summary>
    /// 新用户是否是管理员.
    /// </summary>
    /// <returns>是管理员返回true，其它返回false.</returns>
    bool NewUserIsAdmin();

    /// 根据用户ID去查询用户是否是管理员
    bool NewUserIsAdmin(uint userId);

    /// <summary>
    /// 检查用户资质，看是否有资质使用一起.
    /// </summary>
    /// <returns>有资质使用设备返回true，其它情况返回false.</returns>
    bool UserQualificationCheck();

    /// <summary>
    /// 检查新用户卡是否合法.
    /// </summary>
    /// <returns>合法返回true，其它情况返回false.</returns>
    bool NewUserCardCheck();

    /// <summary>
    /// 检查新用户的账户是否合法.
    /// </summary>
    /// <param name="aIsPaymentAccountEnabled">使用付款帐户</param>
    /// <param name="denyDeposit">不允许超支</param>
    /// <returns>合法返回true，其它情况返回false.</returns>
    bool NewUserAccountCheck(bool aIsPaymentAccountEnabled, bool denyDeposit);

    /// <summary>
    /// 获取会话信息.
    /// </summary>
    /// <returns>返回会话句柄.</returns>
    biotech::osla::Session& GetSessionInfo();

    /// <summary>
    /// 定时更新会话结束时间，防止中途关机或重启操作.
    /// </summary>
    /// <returns>成功返回true，失败返回false.</returns>
    bool UpdateSessionEndTime();

    uint& GetDeviceTimeElapsed();

private:
    uint timeElapsed{ 0 };                                 /// 当前用户使用仪器的时间
    int m_CountUserInTraining{ 0 };                        /// 参加培训的人数
    User m_NewUser;                                        /// 新用户
    User m_CurrentUser;                                    /// 当前用户
    User m_CurrentBookingUser;                             /// 当前预约用户
    biotech::osla::Session session;                        /// 当前会话数据
    UserQualification userQualification;                   /// 用户资格检查
};

#endif //USERINFORMATION_H
