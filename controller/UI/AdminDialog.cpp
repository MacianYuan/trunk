#include <QtCore/QSignalMapper>
#include <tools/log.h>
#include <tools/IniParse.h>
#include <deviceinfo/version.h>
#include <deviceinfo/enum.h>

#include "controller/UI/AdminDialog.h"
#include "ui_AdminDialog.h"
#include "Context/WindowContext.h"

AdminDialog::AdminDialog(QWidget *parent) : QDialog(parent),
                                            ui(new Ui::AdminDialog), isUsing(false) {
    ui->setupUi(this);
    ui->shutdownButton->setEnabled(false);

    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(done(int)));
    mapper->setMapping(ui->trainingButton, START_TRAINING);
    mapper->setMapping(ui->maintainButton, START_MAINTENANCE);
    mapper->setMapping(ui->assistButton, START_ASSISTANCE);
    mapper->setMapping(ui->shutdownButton, END_SESSION);
    mapper->setMapping(ui->settingsButton, CONFIG_SYSTEM);
    mapper->setMapping(ui->recordButton, VIEW_RECORDS);
    mapper->setMapping(ui->projectStartButton, START_PROJECT);
    mapper->setMapping(ui->educationButton, START_EDUCATION);

    connect(ui->trainingButton, SIGNAL(clicked()), mapper, SLOT(map()));
    connect(ui->maintainButton, SIGNAL(clicked()), mapper, SLOT(map()));
    connect(ui->assistButton, SIGNAL(clicked()), mapper, SLOT(map()));
    connect(ui->shutdownButton, SIGNAL(clicked()), mapper, SLOT(map()));
    connect(ui->settingsButton, SIGNAL(clicked()), mapper, SLOT(map()));
    connect(ui->recordButton, SIGNAL(clicked()), mapper, SLOT(map()));
    connect(ui->projectStartButton, SIGNAL(clicked()), mapper, SLOT(map()));
    connect(ui->educationButton, SIGNAL(clicked()), mapper, SLOT(map()));
}

AdminDialog::~AdminDialog() {
    delete ui;
}


void AdminDialog::setCurrentUserInfo(const User &user) {
    DEBUG_LOG("");
    ui->userInfoGroupBox->setVisible(true);
    ui->userInfoLabel->setText(user.fullName);
    ui->shutdownButton->setEnabled(true);
    ui->trainingButton->setEnabled(false);
    ui->educationButton->setEnabled(false);
    ui->maintainButton->setEnabled(false);
    ui->assistButton->setEnabled(false);
    ui->projectStartButton->setEnabled(false);


    resize(606, 253);
}

void AdminDialog::clearCurrentUserInfo() {
    ui->userInfoGroupBox->setVisible(false);
    ui->userInfoLabel->clear();
    ui->shutdownButton->setEnabled(false);
    ui->trainingButton->setEnabled(true);
    ui->educationButton->setEnabled(true);
    ui->maintainButton->setEnabled(true);
    ui->assistButton->setEnabled(true);
    ui->projectStartButton->setEnabled(true);

    resize(456, 253);
}

void AdminDialog::setUsing(bool flag)
{
    isUsing = flag;
}

void AdminDialog::showEvent(QShowEvent *) {   
    /// 开启了项目委托功能，只能使用项目委托，其他刷卡不能用
    IniParse ini(OSLA_CONFIG);
    long bookingModelId = ini.GetLong("Instrument.BookingModelId", 0);
    if (((bookingModelId & BookingModelType::Project) > 0)
       && (!isUsing)){
        ui->projectStartButton->setEnabled(true);
    } else {
        ui->projectStartButton->setEnabled(false);
    }

    if (((bookingModelId & BookingModelType::Book) > 0)
        && (!isUsing)){
        ui->assistButton->setEnabled(true);
    } else {
        ui->assistButton->setEnabled(false);
    }
}


