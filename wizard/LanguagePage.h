
#ifndef LANGUAGEPAGE_H
#define LANGUAGEPAGE_H

#include <QtGui/QWizardPage>
#include <QtCore/QTranslator>
#include <QtGui/QLabel>
#include <QtCore/QDateTime>
#include <QtGui/QComboBox>
#include <QtGui/QTimeEdit>
#include <QtGui/QDateEdit>

class LanguagePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit LanguagePage(QWidget* parent = 0);
    ~LanguagePage();
    QString currentLanguage() const;
	int currentVersion();
    void SetSysTime();

private slots:
    void onTimerTimeout();
    void switchLanguage(int langIndex);
    void UpdateYearCombox(int index);
    void UpdateMonthCombox(int index);
    void UpdateDayCombox(int index);
    void UpdateHourCombox(int index);
    void UpdateMinuteCombox(int index);

private:
    void SetShowTime(QDateTime info);
    void ConnectTimeCombox();
    void DisConnectTimeCombox();

private:
    QString getDeviceVersion(void);
    QTranslator appTranslator;
    QLabel* langLabel;
    QComboBox* langComboBox;
    QLabel* timeLabel;
    QString currentLocale;
    QTimer* timer;
    QDateTime now;

    QComboBox* yearCombox;
    QComboBox* monthCombox;
    QComboBox* dayCombox;
    QComboBox* hourCombox;
    QComboBox* minuteCombox;

    QLabel* versionLabel;
    QComboBox* versionComboBox;

private:
    void retranslateUi();
};

#endif //LANGUAGEPAGE_H
