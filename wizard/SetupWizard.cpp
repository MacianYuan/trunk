#include <QtCore/QProcess>
#include <QtGui/QApplication>
#include <QtCore/QFile>
#include <QtGui/QMessageBox>
#include <QtCore/QTimer>

#include <unistd.h>
#include <deviceinfo/version.h>
#include <tools/common.h>
#include <imframe/imframe.h>
#include <tools/log.h>
#include <tools/IniParse.h>
#include <deviceinfo/tq2440.h>

#include "SetupWizard.h"
#include "splashframe/splashframe.h"
#include "sqlworker/sqlworker.h"

#include "LanguagePage.h"
#include "DownloadPage.h"
#include "NetchoicePage.h"
#include "ConfirmPage.h"
#include "InstrumentPage.h"
#include "NetworkPage.h"
#include "Public.h"
#include <QTextCodec>

//static const int MaxTryCount = 3;

SetupWizard::SetupWizard(QWidget *parent) : QWizard(parent) {
    languagePage = new LanguagePage(this);
    networkPage = new NetworkPage(this);
    netchoicePage = new NetchoicePage(this);
    instrumentPage = new InstrumentPage(this);
    downloadPage = new DownloadPage(this);
    confirmPage = new ConfirmPage(this);

    setPage(Page_Language, languagePage);
    setPage(Page_Netchoice, netchoicePage);
    setPage(Page_Network, networkPage);
    setPage(Page_Instrument, instrumentPage);
    setPage(Page_Download, downloadPage);
    setPage(Page_Confirm, confirmPage);

    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);


    setWindowTitle(tr("Setup Wizard"));//配置向导
    setWizardStyle(QWizard::ModernStyle);

    QList<QWizard::WizardButton> buttonLayout;
    buttonLayout << QWizard::BackButton << QWizard::Stretch
                 << QWizard::NextButton << QWizard::CommitButton
                 << QWizard::FinishButton;
    setButtonLayout(buttonLayout);

    setOption(QWizard::DisabledBackButtonOnLastPage);

    splash = new SplashFrame(this);  /// 等待wait动画
    tcpconnection = new TcpConnection(this);
    sslService = new SslService(this);
    sslService->setInstrumentPage(instrumentPage);

    state = TcpConnectingState;

    const char *fileList[] = {DATABASE_NAME, SSL_CERT, OSLA_CONFIG, CLIENT_CONFIG, NET_CONFIG, NETTYPE_CONFIG,
                              WLAN_CONFIG, NULL};
    for (int i = 0; fileList[i] != NULL; ++i) {
        if (QFile(fileList[i]).exists()) {
            QFile(fileList[i]).remove();
        }
    }
    QFile wlanConfigDefault(WLAN_CONFIG);
    wlanConfigDefault.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&wlanConfigDefault);
    out << "ctrl_interface=/var/run/wpa_supplicant" << endl;
    wlanConfigDefault.close();

    sqlWorker = SqlWorker::GetSqlWorkByThread();
    if (!sqlWorker->isReady()) {
        QMessageBox::critical(this, tr("Error"), tr("Unable to open database file!"));//错误  无法打开数据库文件!
        QWizard::reject();
    }
    sqlWorker->createTables();
    sslService->setSqlWorker(sqlWorker);

    connect(this, SIGNAL(exit()), this, SLOT(reject()));

    connect(tcpconnection, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(displaySocketError(QAbstractSocket::SocketError)));
    connect(tcpconnection, SIGNAL(connectionError(SetupError)), this, SLOT(handleSetupError(SetupError)));
    connect(tcpconnection, SIGNAL(showMessage(const QString&)), this, SLOT(onShowMessage(const QString&)));
    connect(tcpconnection, SIGNAL(changeRegisterState(int)), this, SLOT(onChangeRegisterState(int)));
    connect(tcpconnection, SIGNAL(connectSslHost()), this, SLOT(onConnectSslHost()));

    connect(sslService, SIGNAL(encrypted()), this, SLOT(onSslConnected()));
    connect(sslService, SIGNAL(showMessage(const QString&)), this, SLOT(onShowMessage(const QString&)));
    connect(sslService, SIGNAL(hideMessage()), this, SLOT(onHideMessage()));
    connect(sslService, SIGNAL(connectionError(SetupError)), this, SLOT(handleSetupError(SetupError)));
    connect(sslService, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(displaySocketError(QAbstractSocket::SocketError)));
    connect(sslService, SIGNAL(changeRegisterState(int)), this, SLOT(onChangeRegisterState(int)));
    connect(sslService, SIGNAL(downloadPageProcess(int)), this, SLOT(onDownloadPageProcess(int)));
    connect(sslService, SIGNAL(downloadPageInfo(const QString&, const QString&)), this,
            SLOT(onDownloadPageInfo(const QString&, const QString&)));
    connect(sslService, SIGNAL(nextPage()), this, SLOT(onNextPage()));
    connect(sslService, SIGNAL(addInstrument(const QString&, int)),
            this, SLOT(onAddInstrument(const QString&, int)));

    connect(this, SIGNAL(error(SetupError)), this, SLOT(handleSetupError(SetupError)));
    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(onCurrentIdChanged(int)));

    connect(downloadPage, SIGNAL(beforeBack()), this, SLOT(onBeforeBack()));

}

SetupWizard::~SetupWizard() {
    google::protobuf::ShutdownProtobufLibrary(); // 关闭google protobuf
}

bool SetupWizard::SaveLanguage() {
    netchoicePage->wlan_delete();
    IniParse ini(OSLA_CONFIG);
    ini.Set("Global.Language", languagePage->currentLanguage().toStdString());
    ini.Set("Global.Version", (long) languagePage->currentVersion());
    ini.SaveFile();
    splash->showMessage(tr("Set the system date and time..."));//设置系统日期/时间
    languagePage->SetSysTime();
    msleep(1500);
    splash->hide();
    return true;
}

bool SetupWizard::SaveNetchoice() {
    //temp::flag = 1;
    if (field("wlanConfig").toInt()) {
        if (false == netchoicePage->wlanSetToConfig()) {
            //InputType::setFlag(InputTypeDefine::ALL_INPUT_V_setup);
            //temp::flag = 0;
            return false;
        }
    }
    return true;
}

bool SetupWizard::SaveNetWork() {
    if (state == TcpConnectingState)
    { // 若TCP未连接，发送TCP连接请求,
        splash->showMessage(tr("Save network settings..."));//保存网络设置
        msleep(1000);
        writeNetConfigFiles();

        splash->showMessage(tr("Get network address..."));//获取网络地址
        msleep(1000);
        QProcess::execute("net_set");

        //使用aliyun ntp server
        do {
            splash->showMessage(tr("Calibration time..."));//校准时间
            msleep(1000);
            QProcess process;
            process.start("ntpdate ntp1.aliyun.com");
            process.waitForFinished(30000);
            int nCode = process.exitCode();
            if (0 == nCode) {
                QProcess::execute("hwclock -w --utc");
                break;
            }

            if (QMessageBox::question(this, tr("Notice"), tr("Calibration failure,whether to continue?"), tr("Yes"), tr("No")) != 0) {
                break;//注意   校时失败,是否继续校时?  是  否
            }
        } while (true);

        {
            QFile::remove("/storage/conf/client.ini");
            QFile::copy("/etc/client.ini", "/storage/conf/client.ini");
            IniParse zhClient("/storage/conf/client.ini");
            zhClient.Set("HostInfo.IP", field("tcpHost").toString().toStdString());
            zhClient.Set("HostInfo.IP", 25000);
            zhClient.SaveFile();
        }

        tcpconnection->setPasswd(field("password").toInt());
        sslService->setOrgBindCode(field("password").toString().toLocal8Bit().data());
        tcpconnection->resetConnection();
        splash->showMessage(tr("Connect to server..."));//连接服务器
        msleep(1500);
        DEBUG_LOG("host:" << field("tcpHost").toString().toLocal8Bit().data() << ", port:"
                          << field("tcpPort").toString().toLocal8Bit().data());
        tcpconnection->connectToHost(field("tcpHost").toString(), field("tcpPort").toInt());
        return false;
    }

    if (state == SslConnectingState) { // 若SSL未连接，发送SSL连接请求
        onConnectSslHost();
        return false;
    }
    return true;
}

bool SetupWizard::SaveInstrument() {
    if (state == WaitingForBindingAck) {//确认 您选择了仪器：是否确定？
        if (QMessageBox::question(this, tr("Confirm"),
                                  tr("You have selected the instrument: %1").arg(instrumentPage->selectedInstrumentName()) + "\n" +
                                  tr("Whether or not sure?"),
                                  tr("Yes"), tr("No")) == 0) {

            if (sslService->state() == QAbstractSocket::UnconnectedState) {
                sslService->stop();
                splash->showMessage(tr("Reestablish the SSL connection..."));//重新建立SSL连接
                sslService->start(false);
                return false;
            }

            splash->showMessage(tr("Send instrument association request..."));//发送仪器关联请求
            sslService->sendBindingRequest(biotech::osla::InstrumentBinding_Option_REQUEST_BINDING);
        }
        return false;
    }
    return true;
}

bool SetupWizard::SaveDownload() {
    if (!downloadPage->complete) {
        return false;
    }
    return true;
}

bool SetupWizard::SaveConfirm() {
    return true;
}

bool SetupWizard::validateCurrentPage() {
    bool res = false;
    switch (currentId()) {
        case Page_Language:
            res = SaveLanguage();
            //InputType::setFlag(InputTypeDefine::ALL_INPUT_V_setup);
            break;

        case Page_Netchoice:
            //InputType::setFlag(InputTypeDefine::ALL_INPUT_V_setup);
            res = SaveNetchoice();
           // InputType::setFlag(InputTypeDefine::NUM_INPUT_FOR_REGISTER);
            break;

        case Page_Network:
            //InputType::setFlag(InputTypeDefine::NUM_INPUT_FOR_REGISTER);
            res = SaveNetWork();

            break;

        case Page_Instrument:
            res = SaveInstrument();
            break;

        case Page_Download:
            res = SaveDownload();
            break;

        case Page_Confirm:
            res = SaveConfirm();
            break;
    }

    return res;
}

void SetupWizard::accept() {
    sqlWorker->closeDatabase();
    QWizard::accept();
    qApp->exit(777);
}

void SetupWizard::reject() {//确认   配置还没有完成，确定要退出吗？
    if (QMessageBox::question(this, tr("Confirm"), tr("Configuration is not complete,are you shure to exit?"), tr("Yes"), tr("No")) == 0) {
        QFile netConfigFile(TQ2440_WLAN_SET);
        if (netConfigFile.open(QIODevice::WriteOnly)) {
            QTextStream out(&netConfigFile);
            out << "ctrl_interface=/var/run/wpa_supplicant" << endl;
            DEBUG_LOG(TQ2440_WLAN_SET << "updated");
        } else {
            DEBUG_LOG("Failed to open" << TQ2440_WLAN_SET);
        }
        netConfigFile.close();
        QProcess::startDetached("sync");

        // 关闭所有网络链接
        tcpconnection->resetConnection();
        sslService->stop();
        // 删除数据库文件
        sqlWorker->removeDatabase();
        // 删除配置文件
        QFile(OSLA_CONFIG).remove();
        // 删除SSL证书文件
        QFile(SSL_CERT).remove();
        // 退出程序
        QWizard::reject();
        qApp->exit(773);
    }
}

void SetupWizard::displaySocketError(QAbstractSocket::SocketError socketError) {
    splash->hide();
    tcpconnection->resetConnection();
    sslService->stop();

    switch (socketError) {
        case QAbstractSocket::ConnectionRefusedError: // 0
            QMessageBox::information(this, tr("Error"), tr("Connection was refused by the peer"));//连接被对方拒绝
            break;

        case QAbstractSocket::RemoteHostClosedError: //1
            if (state != TcpReceivedState) {
                QMessageBox::information(this, tr("Error"), tr("The remote host closed the connection"));//远程主机已关闭连接
            }
            break;

        case QAbstractSocket::HostNotFoundError: // 2
            QMessageBox::information(this, tr("Error"), tr("Host was not found"));//没找到目标主机
            break;

        case QAbstractSocket::SocketTimeoutError: // 5
            QMessageBox::information(this, tr("Error"), tr("Connection time out"));//连接超时
            break;

        case QAbstractSocket::NetworkError: // 7
            QMessageBox::information(this, tr("Error"), tr("Host unreachable"));//目标主机无法到达
            break;

        case QAbstractSocket::SslHandshakeFailedError: // 13
            QMessageBox::information(this, tr("Error"), tr("SSL handshake failed"));//SSL握手失败

        default:
            QMessageBox::information(this, tr("Error"),
                                     tr("Error (Code: %1): %2").arg(socketError).arg(tcpconnection->errorString()));
            break;
    }
}

void SetupWizard::displaySocketState(QAbstractSocket::SocketState socketState) {
    DEBUG_LOG("Socket state:" << socketState);
}


void SetupWizard::handleSetupError(SetupError setupError) {
    splash->hide();
    tcpconnection->resetConnection();
    sslService->stop();

    switch (setupError) {
        case AuthFailedError:
            state = TcpConnectingState;
            QMessageBox::information(this, tr("Error"), tr("Authentication failed"));//授权失败
            break;
        case CertChecksumError:
            state = TcpConnectingState;
            QMessageBox::information(this, tr("Error"), tr("SSL certification checksum error"));//SSL证书校验错误
            break;
        case CertFileOpenError:
            state = TcpConnectingState;
            QMessageBox::critical(this, tr("Error"), tr("Failed to open SSL certification"));//无法打开SSL证书
            break;
        case SslEncryptedError:
            sslService->stop();
            QMessageBox::warning(this, tr("Error"), tr("SSL encryption failed"));//SSL加密失败
            break;
        case BindingError:
            QMessageBox::information(this, tr("Error"), tr("Instrument binding failed"));//仪器关联失败
            break;
        case TimeOutError:
            sslService->stop();
            QMessageBox::warning(this, tr("Error"), tr("Transfer time out"));//传输超时
            //emit exit();
            break;
        case TransferTimeOut:
            QMessageBox::information(this, tr("Error"), tr("Transfer time out"));//传输超时
            break;
        case InvalidFrameLength:
            QMessageBox::information(this, tr("Error"), tr("lllegal message length"));//非法报文长度
            break;
        default:
            break;
    }
}

void SetupWizard::writeNetConfigFiles() {
    QFile netTypeConfigFile(TQ2440_NETTYPE_CONF);
    if (netTypeConfigFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&netTypeConfigFile);
        out << "DHCP=" << field("autoConfig").toInt() << endl;
        out << "NET=" << field("lanConfig").toInt() << endl;
        out << "LOCKER_OVER_ETH=" << field("lockerConfig").toInt() << endl;
        DEBUG_LOG(TQ2440_NETTYPE_CONF << "updated");
    } else {
        DEBUG_LOG("Failed to open" << TQ2440_NETTYPE_CONF);
    }
    netTypeConfigFile.close();

    QFile netConfigFile(TQ2440_NET_CONF);
    if (netConfigFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&netConfigFile);
        out << "IPADDR=" << field("ipAddress").toString().toLatin1() << endl;
        out << "NETMASK=" << field("netmask").toString().toLatin1() << endl;
        out << "GATEWAY=" << field("gateway").toString().toLatin1() << endl;
        DEBUG_LOG(TQ2440_NET_CONF << "updated");
    } else {
        DEBUG_LOG("Failed to open" << TQ2440_NET_CONF);
    }
    netConfigFile.close();

    QProcess::startDetached("sync");
}

void SetupWizard::onShowMessage(const QString &info) {
    splash->showMessage(info);
}

void SetupWizard::onChangeRegisterState(int s) {
    state = (ProcessState) s;
    DEBUG_LOG("state is : " << state);
}

void SetupWizard::onDownloadPageInfo(const QString &label, const QString &ap) {
    if (label.length() > 0) {
        downloadPage->label->setText(label);
    }
    if (ap.length() > 0) {
        downloadPage->information->append(ap);
    }
}

void SetupWizard::onDownloadPageProcess(int num) {
    downloadPage->progressBar->setRange(0, 100);
    downloadPage->progressBar->setValue(100);
    downloadPage->complete = true;
}

void SetupWizard::onHideMessage() {
    splash->hide();
}

void SetupWizard::onNextPage() {
    DEBUG_LOG("");
    //QTimer::singleShot(0, downloadPage, SIGNAL(completeChanged()));
    QTimer::singleShot(0, this, SIGNAL(next()));
}

void SetupWizard::onConnectSslHost() {
    IniParse ini(OSLA_CONFIG);
    std::string h = ini.GetString("Network.SslHost");
    long p = ini.GetLong("Network.SslPort");
    DEBUG_LOG("Ssl connect to server [" << h << ":" << p << "]");
    sslService->setHostInfo(QString(h.c_str()), p);
    sslService->stop();
    sslService->start();
    splash->showMessage(tr("Establishing SSL connection..."));//建立SSL连接...
}

void SetupWizard::onAddInstrument(const QString &name, int id) {
    instrumentPage->addInstrument(name, id); // 添加到仪器选择页的列表视图控件
}

void SetupWizard::onCurrentIdChanged(int id)
{
    switch (id) {
       case Page_Language:
            InputType::setFlag(InputTypeDefine::ALL_INPUT_V_setup);
            break;
       case Page_Netchoice:
             InputType::setFlag(InputTypeDefine::ALL_INPUT_V_setup);
            break;
       case Page_Network:
            InputType::setFlag(InputTypeDefine::NUM_INPUT_FOR_REGISTER);
            tcpconnection->resetConnection();
            sslService->stop();
            state = TcpConnectingState;
            break;
       default:break;

    }
}


void SetupWizard::onBeforeBack()
{
    if (currentId() == Page_Download)
    {
        state = WaitingForBindingAck;
        sslService->stop();
    }
}
