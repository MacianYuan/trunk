#ifndef ADDWLANDIALOG_H
#define ADDWLANDIALOG_H

#include <QDialog>

namespace Ui {
class AddWlanDialog;
}

class AddWlanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddWlanDialog(QWidget *parent = 0);
    ~AddWlanDialog();

signals:
    void hideSsidInputError(bool);
private:
    Ui::AddWlanDialog *ui;
    bool HideSSID ;
private slots:
    void savewlanconfig();
};

#endif // ADDWLANDIALOG_H
