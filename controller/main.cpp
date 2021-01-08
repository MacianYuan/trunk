#include <QtGui/QApplication>
#include <QtGui/QWSServer>
#include <QtCore/QFile>
#include <QtSql/QtSql>
#include <sys/prctl.h>
#include <signal.h>
#include <QtGui/QFont>
#include <QtCore/QObject>
#include <tools/WatchDog.h>

#include "deviceinfo/version.h"
#include "imframe/imframe.h"
#include "controller/UI/MainWindow.h"
#include "controller/UI/ScreenSaver.h"
#include "Context/ServiceEvent.h"
#include "Base/GlobalData.h"

#include "controller/Service/SslService.h"
#include "tools/IniParse.h"
#include "tools/log.h"
#include "quaadler32.h"
#include "controller/UI/SettingsDialog.h"
#include <QtCore/QTranslator>
#include <QtGui/QApplication>

bool checkIntegrity() {
    if (!QFile(SSL_CERT).exists()) {
        return false;
    }

    if (!QFile(DATABASE_NAME).exists()) {
        return false;
    }
    if (!QFile(OSLA_CONFIG).exists()) {
        return false;
    }
    if (!QFile(CLIENT_CONFIG).exists()) {
        IniParse client(CLIENT_CONFIG);
        client.Set("CAS.ClientAddr", 0);
        client.Set("CAS.Enabled", false);
        client.Set("Energy.ScreenDim", 60);
        client.Set("Energy.ScreenOff", 300);
        client.Set("Security.AutoCloseDialog", 15);
        client.Set("Serial.BaudRate", 19200);
        client.Set("Serial.DataBits", 8);
        client.Set("Serial.Flow", 0);
        client.Set("Serial.Parity", 0);
        client.Set("Serial.StopBits", 0);
        client.Set("Voice.Enabled", true);
        client.Set("Voice.Volume", 54);
        client.Set("Reader.Guarding", 5 * 1000);
        client.Set("Reader.SeniorUserCanLogin", true);
        client.SaveFile();
    }

    IniParse osla(OSLA_CONFIG);
    return (osla.GetLong("Global.Setup") > 0);
}

int main(int argc, char *argv[]) {

    //退出时结束所有子进程
    prctl(PR_SET_PDEATHSIG, SIGKILL);

    Q_INIT_RESOURCE(controller);
    LoggerControl::getInstance().init("/tmp/controller.log", 3 * 1024 * 1024, 5);

    QApplication app(argc, argv);
    //app.setQuitOnLastWindowClosed(false);

    QWSServer::setCursorVisible(false);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION_STR);
    app.setOrganizationName(ORG_NAME);
    app.setOrganizationDomain(ORG_DOMAIN);

    QFont font("WenQuanYi Micro Hei");
    font.setPixelSize(18);
    app.setFont(font);

    // 设置工作目录为应用程序所在目录
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    //QProcess::execute("net_clear");

    if (checkIntegrity() == false) {
        QProcess::startDetached("./wizard", QStringList() << "-qws");
        return 1;
    }

    IniParse osla(OSLA_CONFIG);
    GlobalData::currentLocale  = osla.GetString("Global.Language").c_str();

    if (GlobalData::currentLocale.isEmpty()) {
        GlobalData::currentLocale = "en_US";
    }
    QTranslator appTranslator;

    if(QString::compare(GlobalData::currentLocale,"en_US") == 0)
    {
            if(appTranslator.load(":/lang/controller_en_US.qm"))// 加载相应的语言文件
            {
                qApp->installTranslator(&appTranslator);
                //app.installTranslator(&appTranslator);
                //retranslateUi(); // 刷新界面
                qDebug()<< "load english success";
            }
            else{
                qDebug()<< "load english  fail";
            }

    }
    if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0)
    {
            if(appTranslator.load(":/lang/controller_zh_CN.qm"))// 加载相应的语言文件
            {
                qApp->installTranslator(&appTranslator);
                //app.installTranslator(&appTranslator);
                //retranslateUi(); // 刷新界面
                qDebug()<< "load chinese success";
            }
            else{
                qDebug()<< "load chinese fail";
            }
    }


//    app.installTranslator(appTranslator);
//    appTranslator->load("controller_" + lang, ":/lang");
//    appTranslator->load("qt_" + lang, ":/lang");

    MainWindow * mainWindow = new MainWindow();
    SslService *sslService = new SslService;
    QThread sslThread;

    QObject::connect(sslService, SIGNAL(networkStateChanged(bool)), mainWindow, SLOT(setNetworkState(bool)), Qt::QueuedConnection);
    QObject::connect(sslService, SIGNAL(bookingUpdated()), mainWindow, SLOT(updateCurrentBooking()));
    //QObject::connect(sslService, SIGNAL(reservationUpdated()), &mainWindow, SLOT(updateReservation()));
    QObject::connect(sslService, SIGNAL(settingsUpdated()), mainWindow, SLOT(readSettings()));
    QObject::connect(sslService, SIGNAL(bindingReleased(bool)), mainWindow, SLOT(onBindingReleased(bool)));
    QObject::connect(sslService, SIGNAL(cardDetectedFromNet(int, int, uint, int, int)), mainWindow,
                     SLOT(onCardDetectedFromNet(int, int, uint, int, int)));
    QObject::connect(sslService, SIGNAL(upgradeParamsUpdated()), mainWindow, SLOT(onUpgradeParamsUpdated()));
    QObject::connect(sslService, SIGNAL(upgradePlanUpdated()), mainWindow, SLOT(onUpgradePlanUpdated()));

    /// 发送解绑请求
    WindowContext* windowContext = mainWindow->getWindowContext();
    QObject::connect(windowContext, SIGNAL(sendRequestUnbinding()), sslService, SLOT(sendUnbindingRequest()));

    QObject::connect(mainWindow, SIGNAL(sendDetectedFromNetResp(int, int, int)), sslService,
                     SLOT(sendDetectedFromNetResp(int, int, int)));
    QObject::connect(mainWindow, SIGNAL(stateChanged(int)), sslService, SLOT(setClientState(int)));
    QObject::connect(mainWindow, SIGNAL(startSync()), sslService, SLOT(start()));
    QObject::connect(mainWindow, SIGNAL(stopSync()), sslService, SLOT(stop()));
    //QObject::connect(mainWindow, SIGNAL(restartSync()), sslService, SLOT(restart()));
    //QObject::connect(mainWindow, SIGNAL(requestUnbinding()), sslService, SLOT(sendUnbindingRequest()));

    QObject::connect(&sslThread, SIGNAL(started()), sslService, SLOT(start()));
    QObject::connect(&sslThread, SIGNAL(finished()), sslService, SLOT(deleteLater()));

    sslService->moveToThread(&sslThread);
    sslThread.start();

    WatchDog::GetInstance();
    mainWindow->showFullScreen();

    IMFrame *im = new IMFrame;
    QWSServer::setCurrentInputMethod(im);

    ScreenSaver *screenSaver = new ScreenSaver();
    QWSServer::setScreenSaver(screenSaver);

    int ret = app.exec();

    sslThread.quit();
    sslThread.wait();

    WatchDog::DestroyInstance();

    QWSServer::screenSaverActivate(false);

//    if (ret == 773) {
//        //QProcess::execute("killall zhClient");
//        //QProcess::startDetached("./wizard", QStringList() << "-qws");
//        return 0;
//    }
//    else if (ret == 777) {
//        if (QProcess::execute("tar zxf controller.tar.gz") < 0) {
//            DEBUG_LOG("Main: Failed to extract file");
//        } else {
//            DEBUG_LOG("Main: Extracted successfully!");
//        }
//        QProcess::execute("rm controller.tar.gz");
//        DEBUG_LOG("Main: Restarting...");
//        QProcess::startDetached("./controller", QStringList() << "-qws");
//        return 0;
//    }

    QProcess::startDetached("reboot");

    return ret;
}
