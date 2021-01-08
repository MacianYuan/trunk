#include <QtGui/QApplication>
#include <QtGui/QWSServer>
#include <QtSql/QtSql>
#include <deviceinfo/version.h>
#include <imframe/imframe.h>
#include "wizard/SetupWizard.h"
#include "tools/log.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(wizard);
    LoggerControl::getInstance().init("/tmp/wizard.log", 3 * 1024 * 1024, 3);
    INFO_LOG("wizard staring");
    QApplication app(argc, argv);
    QWSServer::setCursorVisible(false);
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION_STR);
    app.setOrganizationName(ORG_NAME);
    app.setOrganizationDomain(ORG_DOMAIN);

    QFont font("WenQuanYi Micro Hei");
    font.setPixelSize(20);
    app.setFont(font);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QProcess::execute("killall ntpd");
    QProcess::execute("rm -fr /storage/sd0/*");

    QProcess::execute("rm -fr /storage/conf/config.db");
    QProcess::execute("cp /etc/config.db /storage/conf/config.db");
    QProcess::execute("rm -fr /storage/conf/log.ini");
    QProcess::execute("cp /etc/log.ini /storage/conf/log.ini");

    SetupWizard setup;
    setup.showFullScreen();

    IMFrame *im = new IMFrame;
    QWSServer::setCurrentInputMethod(im);

    int ret = app.exec();

//    if (ret == 773) {
//        QProcess::startDetached("./wizard", QStringList() << "-qws");
//        return 0;
//    } else if (ret == 777) {
//        QProcess::execute("killall zhClient");
//        QProcess::startDetached("zhClient -d true -c /storage/conf/client.ini");
//        QProcess::startDetached("./controller", QStringList() << "-qws");
//        return 0;
//    }

    QProcess::startDetached("reboot");

    return ret;
}
