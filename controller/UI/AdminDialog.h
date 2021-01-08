#ifndef ADMINDIALOG_H
#define ADMINDIALOG_H

#include <QtGui/QDialog>
#include <deviceinfo/typedef.h>

namespace Ui {
class AdminDialog;
}

class AdminDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AdminDialog(QWidget *parent = 0);
    ~AdminDialog();
    void setCurrentUserInfo(const User &user);
    void clearCurrentUserInfo();
    void setUsing(bool flag);
    enum AdminDialogCode {
        START_TRAINING = 2,
        START_MAINTENANCE = 3,
        START_ASSISTANCE = 4,
        END_SESSION = 5,
        CONFIG_SYSTEM = 6,
        VIEW_RECORDS = 7,
        START_PROJECT = 8, 
        START_EDUCATION = 9, 
    };

protected:
    void showEvent(QShowEvent *);



private:
    Ui::AdminDialog *ui;
    //正在实验标志
    bool isUsing;
};

#endif // ADMINDIALOG_H
