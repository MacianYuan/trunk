#include <QtGui/QVBoxLayout>
#include <QtGui/QAbstractButton>
#include "DownloadPage.h"

DownloadPage::DownloadPage(QWidget* parent) : QWizardPage(parent)
{
    complete = false;
    label = new QLabel;
    information = new QTextBrowser;
    progressBar = new QProgressBar;
    progressBar->setRange(0, 0);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(progressBar);
    layout->addWidget(information);
    setLayout(layout);
}

void DownloadPage::initializePage()
{
    setTitle(tr("Data download"));//数据下载
    setSubTitle(tr("Download the relevant data from the server"));//从服务器下载相关数据

    label->setText(tr("Get started downloading..."));//开始下载
    information->clear();
    information->append(tr("Downloading"));//开始下载

    setButtonText(QWizard::NextButton, tr("Next >"));//下一步
    setButtonText(QWizard::BackButton, tr("< Back"));//上一步

    //QWizard* selfWizard = wizard();
    //selfWizard->setButton(QWizard::BackButton, NULL);

    //completeChanged();
}

bool DownloadPage::isComplete() const
{
    return complete;
}

void DownloadPage::cleanupPage()
{
    emit beforeBack();
}
