#include <iostream>
#include <QtCore/QRegExp>
#include <QtGui/QGridLayout>
#include <QtGui/QRegExpValidator>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <deviceinfo/tq2440.h>
#include <tools/log.h>

#include "wizard/NetworkPage.h"

NetworkPage::NetworkPage(QWidget* parent) : QWizardPage(parent)
{
    //setCommitPage(true);
    //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/bk2.jpg"));

    //setTitle(tr("Network Setup"));//配置完成
    autoButton = new QRadioButton;
    autoButton->setChecked(true);
    manualButton = new QRadioButton;

    ipLabel = new QLabel;
    netmaskLabel = new QLabel;
    gatewayLabel = new QLabel;

    ipEdit = new QLineEdit;
    netmaskEdit = new QLineEdit;
    gatewayEdit = new QLineEdit;

    ipEdit->setEnabled(false);
    netmaskEdit->setEnabled(false);
    gatewayEdit->setEnabled(false);

    tcpHostLabel = new QLabel;
    tcpPortLabel = new QLabel;
    passwordLabel = new QLabel;
    tcpHostEdit = new QLineEdit;
    tcpPortEdit = new QLineEdit;
    passwordEdit = new QLineEdit;

    QRegExp ipRegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator* ipRegExpValidator = new QRegExpValidator(ipRegExp, this);
    ipEdit->setValidator(ipRegExpValidator);
    netmaskEdit->setValidator(ipRegExpValidator);
    gatewayEdit->setValidator(ipRegExpValidator);
    tcpHostEdit->setValidator(ipRegExpValidator);

    QIntValidator* portIntValidator = new QIntValidator(1, 65535, this);
    tcpPortEdit->setValidator(portIntValidator);

    QRegExp codeRegExp("\\d{6}");
    QRegExpValidator* codeRegExpValidator = new QRegExpValidator(codeRegExp, this);
    passwordEdit->setValidator(codeRegExpValidator);

    ipEdit->setFixedWidth(151);
    netmaskEdit->setFixedWidth(151);
    gatewayEdit->setFixedWidth(151);
    tcpHostEdit->setFixedWidth(151);
    tcpPortEdit->setFixedWidth(151);
    passwordEdit->setFixedWidth(151);

    QGridLayout* manualLayout = new QGridLayout;
    manualLayout->addWidget(ipLabel, 0, 0, Qt::AlignLeft);
    manualLayout->addWidget(ipEdit, 0, 1, Qt::AlignRight);
    manualLayout->addWidget(netmaskLabel, 1, 0, Qt::AlignLeft);
    manualLayout->addWidget(netmaskEdit, 1, 1, Qt::AlignRight);
    manualLayout->addWidget(gatewayLabel, 2, 0, Qt::AlignLeft);
    manualLayout->addWidget(gatewayEdit, 2, 1, Qt::AlignRight);

    QGridLayout* serverLayout = new QGridLayout;
    serverLayout->addWidget(tcpHostLabel, 0, 0, Qt::AlignLeft);
    serverLayout->addWidget(tcpHostEdit, 0, 1, Qt::AlignRight);
    serverLayout->addWidget(tcpPortLabel, 1, 0, Qt::AlignLeft);
    serverLayout->addWidget(tcpPortEdit, 1, 1, Qt::AlignRight);
    serverLayout->addWidget(passwordLabel, 2, 0, Qt::AlignLeft);
    serverLayout->addWidget(passwordEdit, 2, 1, Qt::AlignRight);

    QVBoxLayout* localLayout = new QVBoxLayout;
    localLayout->addWidget(autoButton);
    localLayout->addWidget(manualButton);
    localLayout->addLayout(manualLayout);

    localConfig = new QGroupBox;
    localConfig->setFixedWidth(271);
    localConfig->setLayout(localLayout);

    serverConfig = new QGroupBox;
    serverConfig->setFixedWidth(271);
    serverConfig->setLayout(serverLayout);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(localConfig);
    layout->addWidget(serverConfig);
    setLayout(layout);

    registerField("autoConfig", autoButton);
    registerField("manualConfig", manualButton);
    registerField("ipAddress", ipEdit);
    registerField("netmask", netmaskEdit);
    registerField("gateway", gatewayEdit);
    registerField("tcpHost", tcpHostEdit);
    registerField("tcpPort", tcpPortEdit);
    registerField("password", passwordEdit);

    connect(manualButton, SIGNAL(toggled(bool)), this, SLOT(modeToggled(bool)));
    connect(manualButton, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
    connect(ipEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(netmaskEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(gatewayEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(tcpHostEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(tcpPortEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(passwordEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
}

void NetworkPage::initializePage()
{
   // setTitle(tr("Network Setup"));//网络设置
   // setSubTitle(tr("  "));
    autoButton->setChecked(true);

    QFile netTypeConfigFile(TQ2440_NETTYPE_CONF);
    if (netTypeConfigFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&netTypeConfigFile);
        QStringList items;
        while (!stream.atEnd()) {
            items.append(stream.readLine());
        }
        foreach(QString item, items) {
            QStringList list = item.split("=");
            if (list[0] == "DHCP") {
                if (list[1] == "0") {
                    manualButton->setChecked(true);
                } else {
                    autoButton->setChecked(true);
                }
            }
        }
    } else {
        //DEBUG_LOG("Failed to oepn" << TQ2440_NETTYPE_CONF);
    }
    netTypeConfigFile.close();

    QFile netConfigFile(TQ2440_NET_CONF);
    if (netConfigFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&netConfigFile);
        QStringList items;
        while (!stream.atEnd()) {
            items.append(stream.readLine());
        }
        foreach(QString item, items) {
            QStringList list = item.split("=");
            if (list[0] == "IPADDR") {
                ipEdit->setText(list[1]);
            } else if (list[0] == "NETMASK") {
                netmaskEdit->setText(list[1]);
            } else if (list[0] == "GATEWAY") {
                gatewayEdit->setText(list[1]);
            }
        }
    } else {
        //DEBUG_LOG("Failed to open" << TQ2440_NET_CONF);
    }
    netConfigFile.close();

    autoButton->setText(tr("Auto (DHCP)"));//自动
    manualButton->setText(tr("Manual"));//手动
    ipLabel->setText(tr("IP Address:"));//IP地址
    netmaskLabel->setText(tr("Netmask:"));//子网掩码
    gatewayLabel->setText(tr("Gataway:"));//网关
    tcpHostLabel->setText(tr("Host:"));//主机
    tcpPortLabel->setText(tr("Port:"));//端口
    passwordLabel->setText(tr("A-Code:"));//授权码
    localConfig->setTitle(tr("Local"));//本地
    serverConfig->setTitle(tr("Server"));//服务器

    setButtonText(QWizard::NextButton, tr("Next >"));//下一步
    setButtonText(QWizard::BackButton, tr("< Back"));//上一步

//    tcpHostEdit->setText(tr("192.168.8.105"));
//    tcpPortEdit->setText(tr("1234"));
//    passwordEdit->setText(tr("123456"));
}

bool NetworkPage::isComplete() const    //输入检查
{
    return (autoButton->isChecked()
            || (ipEdit->hasAcceptableInput()
                && netmaskEdit->hasAcceptableInput()
                && gatewayEdit->hasAcceptableInput()))
           && tcpHostEdit->hasAcceptableInput()
           && tcpPortEdit->hasAcceptableInput()
           && passwordEdit->hasAcceptableInput();
}

void NetworkPage::modeToggled(bool checked)
{
    ipEdit->setEnabled(checked);
    netmaskEdit->setEnabled(checked);
    gatewayEdit->setEnabled(checked);

    //    ipEdit->clear();
    //    netmaskEdit->clear();
    //    gatewayEdit->clear();
}
