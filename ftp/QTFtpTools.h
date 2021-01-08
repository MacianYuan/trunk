
#ifndef QTFILETOOLS_H
#define QTFILETOOLS_H

#include <QtCore/QString>
#include <QtCore/QFileInfoList>

enum FTP_PATH_TYPE {
    PATH_ERROR = -1,
    PATH_DONT_EXIST,
    PATH_IS_FILE,
    PATH_IS_DIR,
};

class QTFtpTools
{
public:
    static QFileInfoList GetFileList(QString path);
    static bool DeletePath(QString dir);
    static qint64 GetFileSize(QString dir);
    static qint64 GetDirSize(QString dir);
    static bool CreateDir(QString dir);
    static void PrintMd5(unsigned char* val, int len);
    static void convertSpecialChar(QString &str);
};


#endif //QTFILETOOLS_H
