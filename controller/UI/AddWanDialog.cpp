#include "AddWlanDialog.h"
#include "ui_AddWlanDialog.h"
#include "tools/cscanapresults.h"
#include "tools/cwlanconfigfile.h"
#include <deviceinfo/tq2440.h>
#include <QtCore/QProcess>
#include <tools/IniParse.h>
#include <QtGui/QMessageBox>
AddWlanDialog::AddWlanDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddWlanDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(savewlanconfig())) ;
}

void AddWlanDialog::savewlanconfig()
{
    CWlanConfigFile wlanConfigFile;
    //CScanAPResults scanAPResults;

    if ((0 == ui->lineEdit_ssid->text().length()) ||(8 > ui->lineEdit_password->text().length())) {
        //tr("注意"), tr("使用无线，输入信息有误，请查看热点和密码"));
        QMessageBox::about(this, tr("Notice"), tr("Please check the AP ssid and password"));
        emit hideSsidInputError(false);
        return;
    }

    if ((0 != ui->lineEdit_ssid->text().length()) && (0 == ui->lineEdit_password->text().length()) ) {
        //tr("请确认是否使用该wifi热点(Wifi无密码)？"),
        QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                                  tr("Please confirm whether to use the wifi AP(Wifi no password)?"),
                                                  QMessageBox::Yes | QMessageBox::No, this);
        messageBox->setDefaultButton(QMessageBox::No);
        messageBox->setDefaultButton(QMessageBox::Yes);
        messageBox->setAttribute(Qt::WA_DeleteOnClose);
        if (messageBox->exec() != QMessageBox::Yes) {

        }
    }
    emit hideSsidInputError(true);
    wlanConfigFile.setSsid(ui->lineEdit_ssid->text());
    wlanConfigFile.setBssid(ui->lineEdit_bssid->text());
    wlanConfigFile.setPassword(ui->lineEdit_password->text());
    wlanConfigFile.writeConfigFile(TQ2440_WLAN_SET);

}


AddWlanDialog::~AddWlanDialog()
{
    delete ui;
}
