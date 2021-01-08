

#ifndef QTFTPINFO_H
#define QTFTPINFO_H

#include <mutex>
#include <list>
#include <openssl/md5.h>
#include <QtCore/QObject>

#include "QTFtpBase.h"

struct FtpSessionInfo {
    int user_id{ 0 };                    /// 用户ID
    int instrument_id{ 0 };              /// 设备ID
    int group_id{ 0 };                   /// 所在组
    int actived_account{ 0 };            /// 付款账户
};

class QTFtpInfo : public QObject
{
    Q_OBJECT
public:
    explicit QTFtpInfo(UpdateUploadProgress cb, void* ctx, QObject* parent = NULL);
    ~QTFtpInfo();

    /// <summary>
    /// 初始化samba工作目录.
    /// </summary>
    /// <param name="sambaPath">samba目录.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool InitDir(QString sambaPath);

    /// <summary>
    /// 初始化ftp服务器登陆信息.
    /// </summary>
    /// <param name="l">ftp服务器登陆信息.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool InitFtp(FtpLogin& l);

    /// <summary>
    /// 运行ftp上传功能.
    /// </summary>
    /// <returns>成功返回true，失败返回false.</returns>
    bool Run();

    bool HandlerException(quint64 id, FtpSessionInfo sessionInfo, bool netStatus);

    /// <summary>
    /// 用户登录，创建samba目录，启动samba服务.
    /// </summary>
    /// <param name="id">事件id.</param>
    /// <param name="netStatus">网络状况，true：网络正常，false：网络异常.</param>
    /// <param name="sambaPath">对外开放的samba目录.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool UserLogin(quint64 id, FtpSessionInfo sessionInfo, bool netStatus, QString& sambaPath);

    /// <summary>
    /// 用户退出，samba目录内容需要上传ftp服务器.
    /// </summary>
    /// <returns>成功返回true，失败返回false.</returns>
    bool UserLogout();

    bool ManualStartSmb();
    bool ManualStopSmb();

    /// <summary>
    /// 更新文件上传的大小.
    /// </summary>
    /// <param name="ul">上传文件大小.</param>
    void UpdateProgress(long ul);

    /// <summary>
    /// 获取当前正在上传文件名和上传进度.
    /// </summary>
    /// <param name="info">正在上传的信息.</param>
    /// <returns>正在上传文件true，无文件上传返回false.</returns>
    bool GetProgress(Progress& info);
signals:
    void errorOccured(QString errorText);
public slots:
    void stopThread(void);
protected:
    /// <summary>
    /// 将本地文件上传到ftp上，使用与本地文件路径一致.
    /// </summary>
    /// <param name="dirList">文件的路径.</param>
    /// <param name="needDelete">成功发送之后，删除本地文件.true:删除，false:不删除.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool Upload(QString dirList, bool needDelete = true);

    /// <summary>
    /// 准备上传文件夹之前，需要做的更新进度信息，获取文件夹大小信息.
    /// </summary>
    /// <param name="path">准备上传的路径.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool BeforeUpload(QString path);

    /// <summary>
    /// 发送文件至ftp服务器.
    /// </summary>
    /// <param name="localPath">本地文件路径.</param>
    /// <param name="startPos">文件起始位置，大于0：追加，其它正常模式.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool SendFile(QString path, qint64 startPos);

    /// <summary>
    /// 上传文件夹成功之后，需要做的更新进度信息
    /// </summary>
    /// <param name="path">上传的路径.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool AfterUpload(QString path);

    /// <summary>
    /// 获取ftp上文件的大小.
    /// </summary>
    /// <param name="path">文件的路径.</param>
    /// <param name="len">文件的长度.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool FtpServerFileSize(QString path, qint64 &len);

    /// <summary>
    /// 在上传文件之前，更新文件的进度信息.
    /// </summary>
    /// <param name="file">文件名.</param>
    /// <param name="pos">文件的起始位置.</param>
    /// <returns>成功返回true，失败返回false.</returns>
    bool InitSendFile(QString file, qint64 startPos);

    bool EndSendFile(bool status);

    bool StartSmb(QString path);

    bool IsNumeric(const char* cList);
    pid_t GetPIDByName(const char* processName);

private:
    QString m_SambaRoot;
    FtpLogin login;

    std::mutex m_Mutex;
    QString m_NowSambaDir;
    std::list<QString> m_NeedSendDir;
    bool m_NeedStop;

    UpdateUploadProgress m_ProgressCB;
    void* m_Ctx;
    InnerProgress* m_Progress;
    MD5_CTX m_Md5Ctx;
    bool smbIsRun;
};

#endif //QTFTPINFO_H
