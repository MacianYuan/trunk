#ifndef SETUPWIZARD_H
#define SETUPWIZARD_H

#include <functional>

#include <QtNetwork/QAbstractSocket>
#include <QtCore/QDateTime>
#include <QtSql/QSqlDatabase>
#include <QtCore/QTranslator>
#include <QtGui/QWizard>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>

#include "protobuf/message.pb.h"
#include "wizard/TcpConnection.h"
#include "wizard/Public.h"
#include "wizard/RegisterSslService.h"
#include "sslconnection/SslConnection.h"
#include <QDebug>

class QComboBox;
class QDateEdit;
class QTimeEdit;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QProgressBar;
class QRadioButton;
class QTextBrowser;

class DownloadPage;
class InstrumentPage;
class LanguagePage;
class NetworkPage;
class NetchoicePage;
class ConfirmPage;
class SplashFrame;
class SqlWorker;

class SetupWizard : public QWizard
{
    Q_OBJECT
public:
    explicit SetupWizard(QWidget* parent = 0);
    ~SetupWizard();

    // 处理用户点击“下一步”需要做的工作
    bool validateCurrentPage();

signals:
    void error(SetupError setupError);
    void exit();

public slots:
    void accept();
    void reject();
    void displaySocketError(QAbstractSocket::SocketError socketError);
    void displaySocketState(QAbstractSocket::SocketState socketState);
    void handleSetupError(SetupError setupError);
    void onShowMessage(const QString& info);
    void onHideMessage();
    void onChangeRegisterState(int s);
    void onDownloadPageInfo(const QString& label, const QString& ap);
    void onDownloadPageProcess(int num);
    void onNextPage();
    void onConnectSslHost();
    void onAddInstrument(const QString& name, int id);
    void onCurrentIdChanged(int id);
    void onBeforeBack(void);

protected:
    inline void writeNetConfigFiles();

    bool SaveLanguage();
    bool SaveNetchoice();
    bool SaveNetWork();
    bool SaveInstrument();
    bool SaveDownload();
    bool SaveConfirm();

private:
    LanguagePage* languagePage;
    NetworkPage* networkPage;
    NetchoicePage* netchoicePage;
    InstrumentPage* instrumentPage;
    DownloadPage* downloadPage;
    ConfirmPage* confirmPage;

    SplashFrame* splash;
    SqlWorker* sqlWorker;

    TcpConnection* tcpconnection;
    SslService* sslService;

    ProcessState state;
    static int page_index_pre;
    static int page_index_new;
};

#endif // SETUPWIZARD_H
