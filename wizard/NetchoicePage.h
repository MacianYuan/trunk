
#ifndef NETCHOICEPAGE_H
#define NETCHOICEPAGE_H

#include <QtGui/QWizardPage>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>

class NetchoicePage : public QWizardPage
{
    Q_OBJECT
public:
    explicit NetchoicePage(QWidget* parent = 0);
    void initializePage();
    bool wlanSetToConfig();
    void wlan_delete();
    void initWlanInfo();

private slots:
    void modeToggled(bool checked);
    void wlan_edit(QString);
    void wlanFlash();
    void wlanConnecting();
    void onLockerOverEthCheckBoxStateChanged(int state);
private:
    QGroupBox* netchoiceConfig;
    QRadioButton* wlanButton;
    QRadioButton* lanButton;
    QLabel* wlanSsidLabel;
    QLabel* wlanPasswdLabel;
    QLineEdit* wlanPasswdEdit;
    QComboBox* wlanSsidCombox;
    QPushButton* wlanFlashButton;
    QPushButton* wlanConnectingButton;
    QCheckBox *lockerOverEthCheckBox;
    QString wlanSsid;
    int timerWlanScanId;
};

#endif //NETCHOICEPAGE_H
