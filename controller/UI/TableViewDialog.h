#ifndef TABLEVIEWDIALOG_H
#define TABLEVIEWDIALOG_H

#include <QtGui/QDialog>
#include <QtSql/QSqlDatabase>

class QSqlQueryModel;

namespace Ui {
class TableViewDialog;
}

class TableViewDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TableViewDialog(QWidget *parent = 0);
    ~TableViewDialog();
    enum TableView {
        USEREVENTTABLEVIEW,
        SESSIONTABLEVIEW,
        SYSTEMEVENTTABLEVIEW
    };

protected:
    void showEvent(QShowEvent *);

private slots:
    void switchTab(int index);
    void showTab(int index);
    void onPrevButtonClicked();
    void onNextButtonClicked();

private:
    Ui::TableViewDialog *ui;
    QSqlDatabase db;
    QSqlQueryModel *model;
    int pageIndex;
    int pageCount;
};

#endif // TABLEVIEWDIALOG_H
