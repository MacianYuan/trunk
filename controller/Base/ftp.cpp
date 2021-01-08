
#include <iostream>

#include "controller/Base/ftp.h"
#include "tools/IniParse.h"
#include <deviceinfo/version.h>
#include <UI/MainWindow.h>
#include <QApplication>


FtpParse::FtpParse(QObject *parent /*= 0*/) : QThread(parent)
{
    initSuccess = false;
    netStatus = false;
    handler = NULL;
}

FtpParse::~FtpParse()
{
}

bool FtpParse::Init(UpdateUploadProgress cb, void* ctx)
{
    if (NULL == handler) {
        delete handler;
    }

    handler = new QTFtpInfo(cb, ctx);

    if (NULL == handler) {
        throw;
    }

    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        MainWindow *mainWindow = qobject_cast<MainWindow *>(widget);
        if (mainWindow)
        {
            connect(handler, SIGNAL(errorOccured(QString)), mainWindow, SLOT(showWaringText(QString)));
            break;
        }
    }

    loadFtpParams();

    initSuccess = true;
    return true;
}

bool FtpParse::SetNetStatus(bool status)
{
    netStatus = status;
    if (true == status && NULL != handler) {
        handler->ManualStartSmb();
    }
    return true;
}

void FtpParse::run()
{
    handler->Run();
}

bool FtpParse::HandlerException(quint64 id, FtpSessionInfo si)
{
    if (NULL == handler) {
        return true;
    }
    return handler->HandlerException(id, si, netStatus);
}

bool FtpParse::UserLogin(quint64 id, FtpSessionInfo si, QString& sambaPath)
{
    if (NULL == handler) {
        return true;
    }

    if (false == initSuccess) {
        return false;
    }
    bool status = handler->UserLogin(id, si, netStatus, sambaPath);
    return status;
}

bool FtpParse::UserLoginMustStartSmb(quint64 id, FtpSessionInfo si, QString & sambaPath)
{
    if (NULL == handler) {
        return true;
    }

    if (false == initSuccess) {
        return false;
    }
    bool status = handler->UserLogin(id, si, true, sambaPath);
    return status;
}

bool FtpParse::UserLogout()
{
    if (false == initSuccess || NULL == handler) {
        return false;
    }
    return handler->UserLogout();
}

void FtpParse::loadFtpParams()
{
    IniParse osla(OSLA_CONFIG);
    QString sambaRootPath(osla.GetString("Ftp.sambapath").c_str());
    if (0 >= sambaRootPath.length()) {
        return;
    }
    handler->InitDir(sambaRootPath);

    FtpLogin loginInfo;
    loginInfo.host = QString::fromStdString(osla.GetString("Ftp.host").c_str());
    loginInfo.port = osla.GetLong("Ftp.port");
    loginInfo.name = QString::fromStdString(osla.GetString("Ftp.name"));
    loginInfo.passwd = QString::fromStdString(osla.GetString("Ftp.passwd"));
    loginInfo.rootpath = QString::fromStdString(osla.GetString("Ftp.rootpath"));
    loginInfo.maxSpeed = osla.GetLong("Ftp.maxspeed");
    handler->InitFtp(loginInfo);
}
