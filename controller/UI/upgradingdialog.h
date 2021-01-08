#ifndef UPGRADINGDIALOG_H
#define UPGRADINGDIALOG_H

#include <QDialog>

namespace Ui {
class upgradingDialog;
}

class UpgradingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpgradingDialog(QWidget *parent = 0);
    ~UpgradingDialog();

private:
    Ui::upgradingDialog *ui;
};

#endif // UPGRADINGDIALOG_H
