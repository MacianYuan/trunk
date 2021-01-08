#include <QtGui/QVBoxLayout>
#include "InstrumentPage.h"


InstrumentPage::InstrumentPage(QWidget* parent)
    : QWizardPage(parent)
{
    label = new QLabel;
    listWidget = new QListWidget;
   // setTitle(tr("Instrument Selection"));//配置完成
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(listWidget);
    setLayout(layout);

    connect(listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(currentInstrumentChange()));
}

void InstrumentPage::initializePage()
{
    setTitle(tr("Instruments and associated"));//仪器关联
    setSubTitle(tr("Please select the target instrument you want to control:"));//请选择需要控制的目标仪器:

    setButtonText(QWizard::NextButton, tr("Next >"));//下一步
    setButtonText(QWizard::BackButton, tr("< Back"));//上一步

    if (listWidget->count() == 0) {
        label->setText(tr("There are no related instruments"));//没有可关联的仪器！
        //listWidget->setEnabled(false);
        return;
    }

    label->setText(tr("Correlating instruments(%1):").arg(listWidget->count()));//可关联的仪器（%1台）:
    listWidget->setCurrentRow(0);
    currentInstrumentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
    currentInstrumentName = listWidget->currentItem()->text();
}

void InstrumentPage::addInstrument(const QString& label, int data)
{
    QListWidgetItem* item = new QListWidgetItem();
    item->setText(label);
    item->setData(Qt::UserRole, data);
    listWidget->addItem(item);
}

int InstrumentPage::selectedInstrumentId() const
{
    return currentInstrumentId;
}

QString InstrumentPage::selectedInstrumentName() const
{
    return currentInstrumentName;
}

void InstrumentPage::currentInstrumentChange()
{
    if (listWidget->currentItem())
    {
        currentInstrumentId = listWidget->currentItem()->data(Qt::UserRole).toInt();
        currentInstrumentName = listWidget->currentItem()->text();
    }
}

bool InstrumentPage::isComplete() const
{
    if (listWidget->count() > 0)
    {
        return true;
    }

    return false;
}

void InstrumentPage::cleanupPage()
{
    listWidget->clear();
}

