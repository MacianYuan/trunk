#ifndef FTP_H
#define FTP_H

#include <QtCore/QThread>
#include "ftp/QTFtpInfo.h"

/// 在无网络连接的情况下，会创建用户可以使用的samba目录，但是不启动samba服务
/// 当网络变得可用的情况下，会看是有由用户登录，如果有，启动samba服务

class FtpParse : public QThread
{
    Q_OBJECT
public:
    explicit FtpParse(QObject *parent = NULL);
    ~FtpParse();
    bool Init(UpdateUploadProgress cb, void* ctx);
    bool SetNetStatus(bool status);
    void run();
    bool HandlerException(quint64 id, FtpSessionInfo si);
    bool UserLogin(quint64 id, FtpSessionInfo si, QString& sambaPath);
    bool UserLoginMustStartSmb(quint64 id, FtpSessionInfo si, QString& sambaPath);
    bool UserLogout();
    void loadFtpParams(void);
private:
    bool netStatus;
    QTFtpInfo* handler;
    bool initSuccess;
};




#endif //FTP_H
