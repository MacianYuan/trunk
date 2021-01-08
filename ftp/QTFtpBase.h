
#ifndef QTFTPBASE_H
#define QTFTPBASE_H

#include <QtCore/QString>

struct FtpLogin {
    QString name;
    QString passwd;
    QString host;
	int port;
    QString rootpath;
    int maxSpeed;       // 单位是k
};

struct InnerProgress;

struct Progress {
    quint64 id;
    unsigned int userId;
    unsigned int instrumentId;
	unsigned int group_id;
	unsigned int actived_account;
    QString fileName;
	QString createTime;
    qint64 fileSize;
    int consume_time;
    QString path;
	int percentage;
    unsigned char md5Result[16];     /// 重新发送的时候，需要重新计算md5值
};

typedef void(*UpdateUploadProgress)(void* ctx, Progress& info);


void ResetProgress(Progress& info);


#endif //QTFTPBASE_H
