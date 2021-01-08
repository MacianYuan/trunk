
#ifndef NORMALTOOLS_H
#define NORMALTOOLS_H

#include <mutex>
#include <map>

#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtGui/QLabel>
#include <QtGui/QFrame>
#include <QtCore/QList>
#include <QtCore/QTime>
#include <QtCore/QProcess>

#define PROMPTINFO NormalTools::GetInstance()->PromptInfo
#define NTG NormalTools::GetInstance

class MessageBox;

class NormalTools : public QObject
{
public:
    static NormalTools* GetInstance();
    static void Destory();
    void FlashCalendarInfo(QLabel *label);
    void PromptInfo(QObject* boxParent, QString info, QString sound = "", bool beep = false, int usec = 0);
    QString FormatTimeString(uint secs);
    void SetTimerLCDText(uint secs, QFrame* frame);
    bool IsWorkingTime(uint secs);
    void UpdateWorkingTime();
    void StartNtpService();

    /// 控制led及其其它灯
    void TurnOffLeds();
    void SetControllerLed(bool on);
    void SetNetworkStatusLed(bool on);
	void SetDeviceWorkStatusLed(bool on);
    void SetPilotLamp(bool on);
    void ClosePopupWindow();

protected:
    NormalTools();
    ~NormalTools();

private:
    QProcess* m_SoundProcess;
    std::map<QObject*, MessageBox*> m_BoxMap;
    QList<QTime> m_WorkingTime;

private:
    static NormalTools* m_Instance;
    static std::mutex m_Mutex;
};

#endif //NORMALTOOLS_H