
#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include <QtGui/QWizardPage>
#include <QtGui/QGroupBox>
#include <QtGui/QRadioButton>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

class NetworkPage : public QWizardPage
{
    Q_OBJECT
public:
    explicit NetworkPage(QWidget* parent = 0);
    void initializePage();

private slots:
    void modeToggled(bool checked);

protected:
    bool isComplete() const;

private:
    QGroupBox* localConfig;
    QGroupBox* serverConfig;

    QRadioButton* autoButton;
    QRadioButton* manualButton;

    QLabel* ipLabel;
    QLabel* netmaskLabel;
    QLabel* gatewayLabel;
    QLabel* tcpHostLabel;
    QLabel* tcpPortLabel;
    QLabel* passwordLabel;


    QLineEdit* ipEdit;
    QLineEdit* netmaskEdit;
    QLineEdit* gatewayEdit;

    QLineEdit* tcpHostEdit;
    QLineEdit* tcpPortEdit;
    QLineEdit* passwordEdit;
};

#endif //NETWORKPAGE_H