#include <QtGui/QLineEdit>
#include <QtCore/QProcess>
#include <QtGui/QGridLayout>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QListView>
#include <QtGui/QMessageBox>
#include <QtGui/QCheckBox>
#include <deviceinfo/tq2440.h>
#include <tools/log.h>

#include "NetchoicePage.h"
#include "tools/cwlanconfigfile.h"
#include "tools/cscanapresults.h"
#include <imframe/imframe.h>
#include <QDebug>
#include <QByteArray>
#include <QTextCodec>
#include <QtGui/QApplication>
QStringList strList;

NetchoicePage::NetchoicePage(QWidget* parent) : QWizardPage(parent)
{
    lanButton = new QRadioButton;
    lanButton->setChecked(false);

    wlanButton = new QRadioButton;
    wlanButton->setChecked(true);
    wlanSsidLabel = new QLabel;
    wlanPasswdLabel = new QLabel;
    wlanSsidCombox = new QComboBox();
    wlanFlashButton = new QPushButton();
    //wlanFlashButton->setText(tr("Refresh"));//刷新
    wlanConnectingButton = new QPushButton();
    //wlanConnectingButton->setText(tr("Connect"));//连接AP
    wlanPasswdEdit = new QLineEdit;
    wlanPasswdEdit->setFixedWidth(250);
    wlanSsidCombox->setStyleSheet("QComboBox {min-width: 250px; max-width:250px;}"
                                  "QComboBox QAbstractItemView::item { min-width: 250px; max-width:250px;}");

    lockerOverEthCheckBox = new QCheckBox;//使用以太网与计算机通信
    connect(lockerOverEthCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onLockerOverEthCheckBoxStateChanged(int)));

    //QProcess::execute("Net");
    //wlan_scan()
    wlanFlash();

    registerField("wlanConfig", wlanButton);
    registerField("lanConfig", lanButton);
    registerField("lockerConfig", lockerOverEthCheckBox);

    QGridLayout* NetworkselectionLayout = new QGridLayout;
    NetworkselectionLayout->addWidget(wlanButton, 0, 0, Qt::AlignLeft);//无线
    NetworkselectionLayout->addWidget(wlanFlashButton, 1, 0, Qt::AlignLeft);//刷新
    NetworkselectionLayout->addWidget(wlanSsidLabel, 2, 0, Qt::AlignLeft);//网络名称
    NetworkselectionLayout->addWidget(wlanSsidCombox, 3, 0, Qt::AlignLeft);//网络选择


    NetworkselectionLayout->addWidget(wlanPasswdLabel, 4, 0, Qt::AlignLeft);//密码
    NetworkselectionLayout->addWidget(wlanPasswdEdit, 5, 0, Qt::AlignLeft);//密码
    NetworkselectionLayout->addWidget(wlanConnectingButton, 6, 0, Qt::AlignLeft);//连接AP

    NetworkselectionLayout->addWidget(lanButton, 7, 0, Qt::AlignLeft);//有线

    NetworkselectionLayout->addWidget(lockerOverEthCheckBox, 8, 0,Qt::AlignLeft);//使用以太网与计算机通信

    setLayout(NetworkselectionLayout);

    connect(wlanButton, SIGNAL(toggled(bool)), this, SLOT(modeToggled(bool)));
    connect(lanButton, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
    connect(wlanSsidCombox, SIGNAL(currentIndexChanged(QString)), this, SLOT(wlan_edit(QString)));
    connect(wlanPasswdEdit, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
    connect(wlanFlashButton, SIGNAL(clicked()), this, SLOT(wlanFlash()));
    connect(wlanConnectingButton, SIGNAL(clicked()), this, SLOT(wlanConnecting()));
}

void NetchoicePage::initializePage()
{
   // setTitle(tr("Network selection:"));//网络选择

    wlanButton->setChecked(true);

    QFile netTypeConfigFile(TQ2440_NETTYPE_CONF);
    if (netTypeConfigFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&netTypeConfigFile);
        QStringList items;
        while (!stream.atEnd()) {
            items.append(stream.readLine());
        }
        foreach(QString item, items) {
            QStringList list = item.split("=");
            if (list[0] == "NET") {
                if (list[1] == "0") {
                    wlanButton->setChecked(true);
                } else {
                    lanButton->setChecked(true);
                }
            }
            else if (list[1] == "LOCKER_OVER_ETH")
            {
                if (list[1] == "1")
                {
                    onLockerOverEthCheckBoxStateChanged(Qt::Checked);
                }
                else
                {
                    onLockerOverEthCheckBoxStateChanged(Qt::Unchecked);
                }
            }
        }
        netTypeConfigFile.close();
    } else {
        DEBUG_LOG("Failed to open" << TQ2440_NETTYPE_CONF);
    }


    wlanFlashButton->setText(tr("Refresh"));//刷新
    wlanConnectingButton->setText(tr("Connect"));//连接AP
    lockerOverEthCheckBox->setText(tr("Use Ethernet to communicate with computers"));

    wlanButton->setText(tr("Wireless Network"));//无线
    lanButton->setText(tr("Wired Network"));//有线

    wlanSsidLabel->setText(tr("SSID:"));//网络名称
    wlanPasswdLabel->setText(tr("Password:"));//密码

    setButtonText(QWizard::NextButton, tr("Next >"));//下一步
    setButtonText(QWizard::BackButton, tr("< Back"));//上一步

    //wlanSsidCombox->addItem(tr("Select SSID"));//选择SSID
}

void NetchoicePage::modeToggled(bool checked)
{
    wlanPasswdEdit->setEnabled(checked);
    wlanSsidCombox->setEnabled(checked);
    wlanFlashButton->setEnabled(checked);
    wlanConnectingButton->setEnabled(checked);
    wlanSsidCombox->setCurrentIndex(0);

    //InputType::setFlag(InputTypeDefine::ALL_INPUT_V_setup);
}

void NetchoicePage::initWlanInfo()
{
    wlanSsidCombox->clear();
    wlanSsidCombox->setView(new QListView());
    wlanSsidCombox->addItem(tr("SSID"));//选择SSID
    CScanAPResults scanAPResults;
    scanAPResults.setResultFile(TQ2440_WLAN_CONF);

    foreach (SSID_INFO ssidInfo, scanAPResults.getAPList()) {
        int hex;
        QByteArray ba ;
        if(ssidInfo.ssid.contains("\\x",Qt::CaseInsensitive)){

            int index1 = ssidInfo.ssid.indexOf("\\x",0);
            QString str1 = ssidInfo.ssid.mid(0,index1);
            //qDebug()<<str1;
            int index2 = ssidInfo.ssid.lastIndexOf("\\x",-1);
            //qDebug()<<"ssidInfo.ssid.length()"<<ssidInfo.ssid.length();
            //qDebug()<<"index2"<<index2;

            for(int i = index1;i < index2+4;i+=4){
                hex = ssidInfo.ssid.mid(i+2,2).toInt(0,16);
                ba.append(hex);
            }
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
            QString text = codec->toUnicode(ba.data());
            //qDebug()<<"text"<<text;

            QString str2 =  ssidInfo.ssid.mid(index2+4,ssidInfo.ssid.length());
            //qDebug()<<str2;
            wlanSsidCombox->addItem("[" + QString::number(ssidInfo.level) + "]" + str1 + text + str2);
        }

        else{
            wlanSsidCombox->addItem("[" + QString::number(ssidInfo.level) + "]" + ssidInfo.ssid);
        }
    }

    wlanSsidCombox->setMaxVisibleItems(5);
    wlanSsidCombox->setCurrentIndex(0);
}

void NetchoicePage::wlan_edit(QString s)
{
    wlanSsid = s;
}

void NetchoicePage::wlanFlash()
{
    QProcess::execute("wlan_scanf");
    initWlanInfo();
}

void NetchoicePage::wlanConnecting()
{
	if ("ko" == wlanPasswdEdit->text() && 0 == wlanSsidCombox->currentIndex()) {
		QProcess::startDetached("testforsmarc -qws");
		exit(0);
	}
    if (false == wlanSetToConfig()) {
        return;
    }
    QProcess::execute("wlan_shutdown");
    QProcess::execute("wlan_connect");
    QProcess* ptr = new QProcess();
    ptr->start("wlan_status");
    ptr->waitForFinished(); ///default time out 30s
    int status = ptr->exitCode();
    DEBUG_LOG("wait for wlan status result is : " << status);
    if (status < 1) {
        QMessageBox msgBox(QMessageBox::Warning, tr("The Network State"), tr("Network Connection Failure"), 0, this);//网络状态   网络连接失败
        msgBox.exec();
    } else {
        QMessageBox msgBox(QMessageBox::Information, tr("The Network State"), tr("Network Connection Successful"), 0, this);//网络状态  网络连接成功
        msgBox.exec();
    }
    delete ptr;
}

void NetchoicePage::onLockerOverEthCheckBoxStateChanged(int state)
{
    if (state == Qt::Unchecked)
    {
        lanButton->setEnabled(true);
    }
    else
    {
        wlanButton->setChecked(true);
        lanButton->setDisabled(true);
    }
}

bool NetchoicePage::wlanSetToConfig()
{
    if (wlanSsidCombox->currentIndex() < 1) {
        QMessageBox msgBox(QMessageBox::Warning, tr("The Network State"), tr("Wireless network is not selected!"), 0, this);//网络状态  未选择无线网络
        msgBox.exec();
        return false;
    }

    CWlanConfigFile wlanConfigFile;
    CScanAPResults scanAPResults;
    scanAPResults.setResultFile(TQ2440_WLAN_CONF);
    QString ssid = CWlanConfigFile::removeLevleForSsid(wlanSsidCombox->currentText());
    wlanConfigFile.setSsid(ssid);
    wlanConfigFile.setBssid(scanAPResults.getAPbssid(ssid));
    wlanConfigFile.setPassword(wlanPasswdEdit->text());
    wlanConfigFile.writeConfigFile(TQ2440_WLAN_SET);

    return true;
}

void NetchoicePage::wlan_delete()
{
    QFile netConfigFile(TQ2440_WLAN_SET);
    if (netConfigFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&netConfigFile);
        out << "ctrl_interface=/var/run/wpa_supplicant" << endl;
    } else {
        DEBUG_LOG("Failed to open" << TQ2440_WLAN_SET);
    }
    netConfigFile.close();
}
