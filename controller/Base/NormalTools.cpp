#include <QtCore/QDateTime>
#include <QtCore/QProcess>
#include <QtCore/QFile>
#include <QtGui/QLCDNumber>

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h> // for opendir(), readdir(), closedir()
#include <sys/stat.h> // for stat()
#include <string.h>
#include <memory>
#include <deviceinfo/tq2440.h>
#include <deviceinfo/version.h>
#include <build/QT/include/QtGui/QApplication>
#include <sqlworker/sqlworker.h>
#include <tools/log.h>

#include "controller/Base/NormalTools.h"
#include "controller/Base/GlobalData.h"
#include "controller/UI/MessageBox.h"
#include "tools/IniParse.h"

NormalTools *NormalTools::m_Instance = NULL;
std::mutex NormalTools::m_Mutex;

NormalTools *NormalTools::GetInstance() {
    if (nullptr == m_Instance) {
        std::lock_guard<std::mutex> guard(m_Mutex);
        if (nullptr == m_Instance) {
            m_Instance = new NormalTools();
        }
    }
    return m_Instance;
}

void NormalTools::Destory() {
    if (nullptr != m_Instance) {
        std::lock_guard<std::mutex> guard(m_Mutex);
        if (nullptr != m_Instance) {
            delete m_Instance;
            m_Instance = nullptr;
        }
    }
}

void NormalTools::FlashCalendarInfo(QLabel *label) {
    // 刷新时钟
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QDate currentDate = currentDateTime.date();
    QString md = currentDateTime.toString("M.d ");
    int dow = currentDate.dayOfWeek();
    IniParse osla(OSLA_CONFIG);
    GlobalData::currentLocale = osla.GetString("Global.Language").c_str();
    if (GlobalData::currentLocale.isEmpty()) {
        GlobalData::currentLocale = "en_US";

    }
    QString dayArr[8];
    if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0){
        dayArr[0] = "";
        dayArr[1] = "星期一";
        dayArr[2] = "星期二";
        dayArr[3] = "星期三";
        dayArr[4] = "星期四";
        dayArr[5] = "星期五";
        dayArr[6] = "星期六";
        dayArr[7] = "星期日";
    }else{
        dayArr[0] = "";
        dayArr[1] = "Monday";
        dayArr[2] = "Tuesday";
        dayArr[3] = "Wednesday";
        dayArr[4] = "Thursday";
        dayArr[5] = "Friday";
        dayArr[6] = "Saturday";
        dayArr[7] = "Sun";
    }

    QString dayN = dayArr[dow];
    QString tm = currentDateTime.toString("hh:mm:ss");
    QString info = QString("%1 %2 %3").arg(md).arg(dayN).arg(tm);
    label->setText(info);
}

void NormalTools::PromptInfo(QObject *boxParent, QString info, QString sound, bool b, int usec) {
    if (true == b) {
        beep(200, usec);
    }

    if (info.length() > 0) {
        auto iter = m_BoxMap.find(boxParent);
        if (m_BoxMap.end() == iter) {
            MessageBox *ptr = new MessageBox((QWidget *) boxParent);
            m_BoxMap.insert(std::make_pair(boxParent, ptr));
            iter = m_BoxMap.find(boxParent);
        }
        iter->second->showText(info);
    }

    IniParse ini(CLIENT_CONFIG);
    if (sound.length() > 0 && true == ini.GetBool("Voice.Enabled")) {
        QStringList param;
        param << ("/usr/bin/" + sound);
        if (QProcess::NotRunning != m_SoundProcess->state()) {
            m_SoundProcess->kill();
            m_SoundProcess->waitForFinished(3000);
        }
        m_SoundProcess->start("/usr/bin/madplay", param);
    }
}

QString NormalTools::FormatTimeString(uint secs) {
    QString timeString;
    if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0){
        if (secs == 0) {
            timeString = tr("0秒");
            return timeString;
        }

        int days = secs / (24 * 3600);
        secs %= 24 * 3600;
        int hours = secs / 3600;
        secs %= 3600;
        int mins = secs / 60;
        secs %= 60;

        if (days == 0) {
            if (hours == 0) {
                if (mins == 0) {
                    timeString = tr("%1秒").arg(secs);//1秒
                } else {
                    if (secs == 0) {
                        timeString = tr("%1分钟").arg(mins);//1分钟
                    } else {
                        timeString = tr("%1分钟%2秒").arg(mins).arg(secs);//%1分%2秒
                    }
                }
            } else {
                timeString = tr("%1小时%2分钟").arg(hours).arg(mins);//%1小时%2分钟
            }
        } else {
            timeString = tr("%1天%2小时%3分钟").arg(days).arg(hours).arg(mins);//%1天%2小时%3分钟
        }
    }
    else{
        if (secs == 0) {
            timeString = tr("0Sec");
            return timeString;
        }

        int days = secs / (24 * 3600);
        secs %= 24 * 3600;
        int hours = secs / 3600;
        secs %= 3600;
        int mins = secs / 60;
        secs %= 60;

        if (days == 0) {
            if (hours == 0) {
                if (mins == 0) {
                    timeString = tr("%1Sec").arg(secs);//1秒
                } else {
                    if (secs == 0) {
                        timeString = tr("%1Min").arg(mins);//1分钟
                    } else {
                        timeString = tr("%1Min%2Sec").arg(mins).arg(secs);//%1分%2秒
                    }
                }
            } else {
                timeString = tr("%1Hour%2Min").arg(hours).arg(mins);//%1小时%2分钟
            }
        } else {
            timeString = tr("%1Day%2Hour%3Min").arg(days).arg(hours).arg(mins);//%1天%2小时%3分钟
        }
    }

    return timeString;
}


void NormalTools::SetTimerLCDText(uint secs, QFrame *frame) {
    QLCDNumber *lcd = (QLCDNumber *) frame;
    if (secs == 0) {
        lcd->display("0000:00:00");
        return;
    }

    int seconds = secs;
    int hours = seconds / 3600;
    seconds = seconds % 3600;
    int minutes = seconds / 60;
    seconds = seconds % 60;
    QString timerText;
    timerText.sprintf("%04d:%02d:%02d", hours, minutes, seconds);
    lcd->display(timerText);
}

bool NormalTools::IsWorkingTime(uint secs) {
    DEBUG_LOG("");
    QTime time = QDateTime::fromTime_t(secs).time();
    SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
    if (true == sqlWorker->isHoliday(QDate::currentDate())) {
        // 是节假日，不是工作时间
        return false;
    }

    for (int i = 0; i < m_WorkingTime.size(); i += 2) {
        DEBUG_LOG("work time : " << m_WorkingTime[i].toString("hh:mm").toStdString() <<
                                 ", " << m_WorkingTime[i + 1].toString("hh:mm").toStdString());
        if (time >= m_WorkingTime[i] && time < m_WorkingTime[i + 1]) {
            return true;
        }
    }
    DEBUG_LOG("");

    return false;
}

void NormalTools::StartNtpService() {
    QProcess::execute("killall ntpd; killall ntpdate");
    IniParse ini(OSLA_CONFIG);
    QString ntpHost(ini.GetString("Network.NtpHost").c_str());
    QString cmd = QString("ntpdate ") + ntpHost + QString(" &");
    system(cmd.toLatin1().data());
}

void NormalTools::TurnOffLeds() {
    /// 0表示关闭，1表示打开
    led1(0);
    led2(0);
    led3(0);
}

void NormalTools::SetControllerLed(bool on) {
    led1((true == on) ? 1 : 0);
}

void NormalTools::SetNetworkStatusLed(bool on) {
    led2((true == on) ? 1 : 0);
}

void NormalTools::SetDeviceWorkStatusLed(bool on) {
    led3((true == on) ? 1 : 0);
}

void NormalTools::SetPilotLamp(bool on) {
    pilotlamp(true == on ? 1 : 0);
}

NormalTools::NormalTools() : QObject(NULL) {
    m_SoundProcess = new QProcess();

    UpdateWorkingTime();
}

NormalTools::~NormalTools() {
    for (auto param: m_BoxMap) {
        delete param.second;
    }
}

void NormalTools::ClosePopupWindow() {
    while (QWidget *widget = QApplication::activeModalWidget()) {
        widget->close();
    }
}

void NormalTools::UpdateWorkingTime() {
    IniParse ini(OSLA_CONFIG);
    DEBUG_LOG("work time all: " << ini.GetString("Instrument.WorkingTime"));
    QStringList workingTimeString = QString(ini.GetString("Instrument.WorkingTime").c_str()).split(QRegExp(";|-"));
    DEBUG_LOG("work time array size is : " << workingTimeString.size());
    if (workingTimeString.isEmpty() || (workingTimeString.size() % 2) != 0) {
        workingTimeString.clear();
        workingTimeString << "08:00" << "12:00" << "14:00" << "18:00";
    }
    m_WorkingTime.clear();
    for (int i = 0; i < workingTimeString.size(); i++) {
        QTime time = QTime::fromString(workingTimeString[i], "hh:mm");
        m_WorkingTime.append(time);
        DEBUG_LOG("work time : " << workingTimeString[i].toStdString());
    }
}
