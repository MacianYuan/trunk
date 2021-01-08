#include <iostream>
#include <fstream>
#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtCore/QProcess>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QApplication>
#include <deviceinfo/version.h>
#include <tools/IniParse.h>
#include "LanguagePage.h"
#include <QDebug>

LanguagePage::LanguagePage(QWidget* parent)
    : QWizardPage(parent)
{
    //setCommitPage(true);
    //setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/bk1.jpg"));

    IniParse ini(OSLA_CONFIG);
    yearCombox = new QComboBox();
    monthCombox = new QComboBox();
    dayCombox = new QComboBox();
    hourCombox = new QComboBox();
    minuteCombox = new QComboBox();

    langLabel = new QLabel;
    langComboBox = new QComboBox;
    timeLabel = new QLabel;

    langComboBox->addItem("English","en_US");
    langComboBox->addItem("简体中文","zh_CN");//简体中文

    currentLocale = ini.GetString("Global.Language").c_str();
    //qDebug()<< currentLocale;

    if (currentLocale.isEmpty()) {
        currentLocale = "en_US";
        langComboBox->setCurrentIndex(0);
    }
    if(QString::compare(currentLocale,"zh_CN") == 0){
        langComboBox->setCurrentIndex(1);
    }


	this->versionLabel = new QLabel;
	this->versionComboBox = new QComboBox;


        if(appTranslator.load("wizard_" + currentLocale, ":/lang")){
              qDebug()<< "load success";
        }
        else{
             qDebug()<< "load fail";
        }

    qApp->installTranslator(&appTranslator);
    retranslateUi();
   // langComboBox->setCurrentIndex(langComboBox->findText("简体中文")); // 缺省使用中文
    connect(langComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(switchLanguage(int))); // 关联到切换语言的槽

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(langLabel);
    layout->addWidget(langComboBox);
    layout->addWidget(timeLabel);
    QHBoxLayout* dateLayout = new QHBoxLayout;
    dateLayout->addWidget(yearCombox);
    dateLayout->addWidget(monthCombox);
    dateLayout->addWidget(dayCombox);
    layout->addLayout(dateLayout);

    QHBoxLayout* timeLayout = new QHBoxLayout;
    timeLayout->addWidget(hourCombox);
    timeLayout->addWidget(minuteCombox);
    layout->addLayout(timeLayout);

   // versionComboBox->addItem(tr("Select Protocol Version"));//选择SSID
	layout->addWidget(versionLabel);
    //for (int i = 5; i <= 5; ++i) {
    QString info = QString(tr("Protocol Version 5"));//协议版本%5
    versionComboBox->addItem(info);
    //}
    //versionComboBox->setCurrentIndex(versionComboBox->findText(tr("Protocol Version 5"))); // 缺省使用中文  协议版本5
	layout->addWidget(versionComboBox);

    setLayout(layout);


    now = QDateTime::currentDateTime();
    SetShowTime(now);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
    timer->start(1000);


}

LanguagePage::~LanguagePage()
{

}

// 返回当前语言
QString LanguagePage::currentLanguage() const
{
    return currentLocale;
}

int LanguagePage::currentVersion()
{
	/// 默认是3，不处理
	return 3;
}

void LanguagePage::SetSysTime()
{
    if (QProcess::execute(QString("date -s %1-%2").arg(now.date().toString("yyyy.MM.dd")).arg(now.time().toString("hh:mm"))) == 0) {
        QProcess::execute("hwclock -w --utc");
    }
}

void LanguagePage::onTimerTimeout()
{
    now = now.addSecs(1);
    QTime ok = now.time();
    if (0 == ok.second()) {
        SetShowTime(now);
    }
}

// 动态切换语言
void LanguagePage::switchLanguage(int langIndex)
{
    currentLocale = langComboBox->itemData(langIndex).toString();
    qDebug()<< currentLocale;
    //QString qmPath = directoryOf("lang").absolutePath();

    if(QString::compare(currentLocale,"en_US") == 0)
    {
            if(appTranslator.load(":/lang/wizard_en_US.qm"))// 加载相应的语言文件
            {
                qApp->installTranslator(&appTranslator);
                retranslateUi(); // 刷新界面
               qDebug()<< "load english success";
            }
            else{
                 qDebug()<< "load english  fail";
            }

    }
    if(QString::compare(currentLocale,"zh_CN") == 0)
    {
            if(appTranslator.load(":/lang/wizard_zh_CN.qm"))// 加载相应的语言文件
            {
                qApp->installTranslator(&appTranslator);
                retranslateUi(); // 刷新界面
                qDebug()<< "load chinese success";
            }
            else{
                 qDebug()<< "load chinese fail";
            }
    }
}

void LanguagePage::UpdateYearCombox(int index)
{
    QDate info = now.date();
    int februaryDays = (true == QDate::isLeapYear(index + 1970) ? 29 : 28);
    int daysArr[] = { 31, februaryDays, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    int day = info.day();
    if (day > daysArr[info.month() - 1]) {
        day = daysArr[info.month() - 1];
    }

    info.setYMD(index + 1970, info.month(), day);
    now.setDate(info);
    SetShowTime(now);
}

void LanguagePage::UpdateMonthCombox(int index)
{
    QDate info = now.date();
    int februaryDays = (true == QDate::isLeapYear(info.year()) ? 29 : 28);
    int daysArr[] = { 31, februaryDays, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    int day = info.day();
    if (day > daysArr[index]) {
        day = daysArr[index];
    }

    info.setYMD(info.year(), index + 1, day);
    now.setDate(info);
    SetShowTime(now);
}

void LanguagePage::UpdateDayCombox(int index)
{
    QDate info = now.date();
    info.setYMD(info.year(), info.month(), index + 1);
    now.setDate(info);
    SetShowTime(now);
}

void LanguagePage::UpdateHourCombox(int index)
{
    QTime info = now.time();
    info.setHMS(index, info.minute(), info.second());
    now.setTime(info);
    SetShowTime(now);
}

void LanguagePage::UpdateMinuteCombox(int index)
{
    QTime info = now.time();
    info.setHMS(info.hour(), index, info.second());
    now.setTime(info);
    SetShowTime(now);
}

void LanguagePage::SetShowTime(QDateTime info)
{
    DisConnectTimeCombox();
    QDate hello = info.date();
    QStringList year;
    for (int i = 1970; i <= 2070; ++i) {
        QString temp = QString(tr("%1 Year").arg(i));
       // temp.sprintf(tr("%4d", i));//年
        year.append(temp);
    }
    yearCombox->clear();
    yearCombox->addItems(year);
    yearCombox->setCurrentIndex(hello.year() - 1970);

    QStringList month;
    for (int i = 1; i <= 12; ++i) {
        QString temp = QString(tr("%1 Month").arg(i));
        //temp.sprintf("%02d", i);//月
        month.append(temp);
    }
    monthCombox->clear();
    monthCombox->addItems(month);
    monthCombox->setCurrentIndex(hello.month() - 1);

    int februaryDays = (true == hello.isLeapYear(hello.year()) ? 29 : 28);
    int daysArr[] = { 31, februaryDays, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    QStringList day;
    for (int i = 1; i <= daysArr[hello.month() - 1]; ++i) {
        QString temp = QString(tr("%1 Day").arg(i));
        //temp.sprintf("%02d", i);//日
        day.append(temp);
    }
    dayCombox->clear();
    dayCombox->addItems(day);
    dayCombox->setCurrentIndex(hello.day() - 1);

    QTime world = info.time();

    QStringList hour;
    for (int i = 0; i <= 23; ++i) {
        QString temp = QString(tr("%1 Hour").arg(i));
        //temp.sprintf("%02d", i);//时
        hour.append(temp);
    }
    hourCombox->clear();
    hourCombox->addItems(hour);
    hourCombox->setCurrentIndex(world.hour());

    QStringList minute;
    for (int i = 0; i <= 59; ++i) {
        QString temp = QString(tr("%1 Minute").arg(i));
        //temp.sprintf("%02d", i);//分
        minute.append(temp);
    }
    minuteCombox->clear();
    minuteCombox->addItems(minute);
    minuteCombox->setCurrentIndex(world.minute());
    ConnectTimeCombox();
}

void LanguagePage::ConnectTimeCombox()
{
    connect(yearCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateYearCombox(int)));
    connect(monthCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateMonthCombox(int)));
    connect(dayCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateDayCombox(int)));
    connect(hourCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateHourCombox(int)));
    connect(minuteCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateMinuteCombox(int)));
}

void LanguagePage::DisConnectTimeCombox()
{
    disconnect(yearCombox, NULL, this, NULL);
    disconnect(monthCombox, NULL, this, NULL);
    disconnect(dayCombox, NULL, this, NULL);
    disconnect(hourCombox, NULL, this, NULL);
    disconnect(minuteCombox, NULL, this, NULL);
}

// 刷新本页面
void LanguagePage::retranslateUi()
{
    setTitle(tr("Setup Wizard"));//配置向导
    setSubTitle(tr("Welcome to use the laboratory instrument open sharing management system") + "[" + getDeviceVersion() + "]");//欢迎使用实验仪器开放共享管理系统
    langLabel->setText(tr("Setting interface language:"));//设置界面语言:
    timeLabel->setText(tr("Set time and date:"));//设置日期和时间:
    versionLabel->setText(tr("Communication protocol version selection:"));//通信协议版本选择：

    setButtonText(QWizard::NextButton, tr("Next >"));//下一步
    setButtonText(QWizard::BackButton, tr("< Back"));//上一步

    versionComboBox->clear();
    QString info = QString(tr("Protocol Version 5"));//协议版本%5
    versionComboBox->addItem(info);
    SetShowTime(now);
}

QString LanguagePage::getDeviceVersion(void)
{
    QString version;

    std::ifstream fin{"/etc/Version", std::ios::in};
    char line[128] = {0};
    fin.read(line, sizeof(line));
    char var[128] = {0};
    sscanf(line, "%*[a-zA-Z0-9]-%*[a-zA-Z0-9]-%[^_]", var);
    fin.close();
    version.append(var);

    return version;
}

