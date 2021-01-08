#include "upgradingdialog.h"
#include "ui_upgradingdialog.h"

UpgradingDialog::UpgradingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::upgradingDialog)
{
    ui->setupUi(this);

    //设置字体颜色和大小
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    ui->energyLabel->setPalette(pa);
    QFont font;
    font.setPointSize(48);
    ui->energyLabel->setFont(font);
}

UpgradingDialog::~UpgradingDialog()
{
    delete ui;
}
