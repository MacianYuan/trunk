#include <QtCore/QDir>

#include "ftp/QTFtpTools.h"

QFileInfoList QTFtpTools::GetFileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (int i = 0; i != folder_list.size(); i++) {
        QString name = folder_list.at(i).absoluteFilePath();
        QFileInfoList child_file_list = GetFileList(name);
        file_list.append(child_file_list);
    }

    return file_list;
}

bool QTFtpTools::DeletePath(QString dirName)
{
    QFileInfo fileInfo(dirName);
    if (true == fileInfo.isDir()) {
        QDir dir(dirName);
        if (!dir.exists()) {
            return false;
        }

        QFileInfoList fileInfoList = dir.entryInfoList();
        foreach(QFileInfo fileInfo, fileInfoList){
            if (fileInfo.fileName() == "." || fileInfo.fileName() == "..") {
                continue;
            }
            if (fileInfo.isDir()){
                QString tmpdir = dirName + ("/") + fileInfo.fileName();
                DeletePath(tmpdir);
                dir.rmdir(fileInfo.fileName()); /**< 移除子目录 */
            } else if (fileInfo.isFile()){
                QFile tmpFile(fileInfo.fileName());
                dir.remove(tmpFile.fileName()); /**< 删除临时文件 */
            }
        }

        dir.cdUp();            /**< 返回上级目录，因为只有返回上级目录，才可以删除这个目录 */
        if (dir.exists(dirName)){
            if (!dir.rmdir(dirName))
                return false;
        }
    } else if (true == fileInfo.isFile()) {
        QFile file(dirName);
        file.remove();
    }

    return true;
}

qint64 QTFtpTools::GetFileSize(QString path)
{
    QFileInfo info(path);
    return info.size();
}

qint64 QTFtpTools::GetDirSize(QString dir)
{
    qint64 size = 0;

    QFileInfoList fileListInfo = GetFileList(dir);
    for (auto v : fileListInfo) {
        size += v.size();
    }

    return size;
}

bool QTFtpTools::CreateDir(QString dir)
{
    QDir d(dir);
    if (false == d.exists()) {
        d.mkdir(dir);
    }
    return d.exists();
}

void QTFtpTools::PrintMd5(unsigned char* val, int len)
{
    for (int i = 0; i < 32; ++i) {
        if (i % 2 == 0) {
            printf("%x", (val[i / 2] >> 4) & 0xf);
        }
        else {
            printf("%x", (val[i / 2]) & 0xf);
        }
    }
    printf("\n");
}


//将 &%#= 这些文件名中允许的字符但url特殊意义的字符转义
//% 转义符为 %25
//# 转义符为 %23
//& 转义符为 %26
//= 转义符为 %3D
void QTFtpTools::convertSpecialChar(QString &str)
{
   str.replace("%", "%25");
   str.replace("#", "%23");
   str.replace("&", "%26");
   str.replace("=", "%3D");
   str.replace(" ", "%20");
}
