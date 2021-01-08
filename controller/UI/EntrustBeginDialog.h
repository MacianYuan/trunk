#ifndef ENTRUSTBEGINDIALOG_H
#define ENTRUSTBEGINDIALOG_H

#include <set>

#include <QtGui/QDialog>
#include <QtSql/QSqlDatabase>
#include "QSqlQueryCheckboxModel.h"
#include <sqlworker/sqlworker.h>
#include <controller/Context/WindowContext.h>

class QSqlQueryModel;
class WindowContext;
class SessionManage;

namespace Ui {
class EntrustBeginDialog;
}

class EntrustBeginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EntrustBeginDialog(QWidget *parent = 0);
    ~EntrustBeginDialog();
    void init(WindowContext* ctx, SessionManage* se);

protected:
    void showEvent(QShowEvent *);
    bool IsContinueUseInstrument();

private slots:
    void showEntrust();
    void onPrevButtonClicked();
    void onNextButtonClicked();
    void startSession();
    void chooseClick(QModelIndex);
    void selectDontOverClicked();
private:
    Ui::EntrustBeginDialog * ui;
    QSqlQueryCheckboxModel *model;
    WindowContext* context;
    SessionManage* sessionManage;
    int pageIndex;
    int pageCount;
};

#endif
