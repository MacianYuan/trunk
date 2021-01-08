#include <QtGui/QVBoxLayout>
#include <QtGui/QWizard>
#include <QtGui/QAbstractButton>
#include "ConfirmPage.h"

ConfirmPage::ConfirmPage(QWidget* parent) : QWizardPage(parent)
{
    okLabel = new QLabel;
    okLabel->setPixmap(QPixmap(":/images/check.png"));
    okLabel->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addStretch();
    layout->addWidget(okLabel);
    setLayout(layout);
}

void ConfirmPage::initializePage()
{
    setTitle(tr("Setup completed"));//配置完成
    setSubTitle(tr("Configuration has been completed\nClick\"Finish\",exit the wizard and restart"));//配置已完成\n点击\"完成\"退出向导并启动

    setButtonText(QWizard::CancelButton, tr("Cancel"));//取消
    setButtonText(QWizard::FinishButton, tr("Finish"));//完成

    //QWizard* selfWizard = wizard();
    //selfWizard->button(WizardButton::
    //selfWizard->setButton(QWizard::BackButton, NULL);
}
