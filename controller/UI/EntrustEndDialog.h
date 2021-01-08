#ifndef EntrustEndDialog_H
#define EntrustEndDialog_H

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
class EntrustEndDialog;
}

class EntrustEndDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EntrustEndDialog(QWidget *parent = 0);
    ~EntrustEndDialog();
    void init(WindowContext* ctx, SessionManage* se);

protected:
    void showEvent(QShowEvent *);
    bool IsContinueUseInstrument();

private slots:
    void showEntrust();
    void onPrevButtonClicked();
    void onNextButtonClicked();
    void endSession();
    void chooseClick(QModelIndex);

private:
    Ui::EntrustEndDialog * ui;
    QSqlQueryCheckboxModel *model;
    WindowContext* context;
    SessionManage* sessionManage;
    int pageIndex;
    int pageCount;
    biotech::osla::EntrustList nowDoEntrustList;
};

#endif
