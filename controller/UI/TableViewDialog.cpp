#include <QtSql/QtSql>
#include <deviceinfo/version.h>
#include "controller/UI/TableViewDialog.h"
#include "ui_TableViewDialog.h"


static const int RowsPerPage = 12;

TableViewDialog::TableViewDialog(QWidget* parent) : QDialog(parent), ui(new Ui::TableViewDialog)
{
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_DeleteOnClose);

    db = QSqlDatabase::addDatabase(DATABASE_DRIVER, "TableViewDialog");
    db.setDatabaseName(DATABASE_NAME);

    if (!db.open()) {
        reject();
    }
    model = new QSqlQueryModel(this);
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(switchTab(int)));
    connect(ui->prevButton, SIGNAL(clicked()), this, SLOT(onPrevButtonClicked()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(onNextButtonClicked()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

TableViewDialog::~TableViewDialog()
{
    model->clear();
    if (db.isOpen()) {
        db.close();
    }
    delete ui;
}

void TableViewDialog::showEvent(QShowEvent*)
{
    switchTab(USEREVENTTABLEVIEW);
}

void TableViewDialog::switchTab(int index)
{
    pageIndex = 0;
    QSqlQuery query(db);
    QString queryString;

    switch (index) {
        case USEREVENTTABLEVIEW:
            queryString = "select count(*) from TUserEvent";
            break;
        case SESSIONTABLEVIEW:
            queryString = "select count(*) from TSession";
            break;
        case SYSTEMEVENTTABLEVIEW:
            queryString = "select count(*) from TSystemEvent";
            break;
    }
    query.exec(queryString);
    query.first();
    pageCount = query.value(0).toInt() / RowsPerPage;
    query.finish();

    showTab(index);
}

void TableViewDialog::showTab(int index)
{
    ui->prevButton->setEnabled(pageIndex > 0);
    ui->nextButton->setEnabled(pageIndex < pageCount);
    ui->pageLabel->setText(tr("%1/%2").arg(pageIndex + 1).arg(pageCount + 1));

    switch (index) {
        case USEREVENTTABLEVIEW: {
            model->clear();
            model->setQuery(QString("SELECT strftime('%m/%d %H:%M:%S', ue.CreateTime, 'unixepoch', 'localtime'),"
                                    "       (u.FirstName || u.LastName) AS UserName,"
                                    "       ue.CardSerial,"
                                     "      t1.Text,"
                                     "      t2.Text,"
                                     "      t3.Text"
                                     " FROM TUserEvent ue,"
                                     "      TUser u,"
                                     "      TEnumText t1,"
                                     "      TEnumText t2,"
                                     "      TEnumText t3 "
                                     "WHERE u.id = ue.UserID AND "
                                     "      t1.id = ue.CardType AND "
                                     "      t2.ID = ue.ActionType AND "
                                     "      t3.ID = ue.ActionResult "
                                     "ORDER BY ue.CreateTime DESC "
                                     "LIMIT %1, %2").arg(pageIndex * RowsPerPage).arg(RowsPerPage), db);
            model->setHeaderData(0, Qt::Horizontal, tr("Charge time"));//刷卡时间
            model->setHeaderData(1, Qt::Horizontal, tr("Charge time"));//刷卡时间
            model->setHeaderData(2, Qt::Horizontal, tr("Card Number"));//卡号
            model->setHeaderData(3, Qt::Horizontal, tr("Card type"));//卡类型
            model->setHeaderData(4, Qt::Horizontal, tr("Operation"));//操作
            model->setHeaderData(5, Qt::Horizontal, tr("Result"));//v
            ui->userEventTableView->setModel(model);
            ui->userEventTableView->resizeColumnsToContents();
            break;
        }

        case SESSIONTABLEVIEW: {
            model->clear();
            model->setQuery(QString("SELECT (u.FirstName || u.LastName) AS UserName,"
                                    "        strftime('%m/%d %H:%M:%S', s.StartTime, 'unixepoch', 'localtime'),"
                                    "        strftime('%m/%d %H:%M:%S', s.EndTime, 'unixepoch', 'localtime'),"
                                    "        t1.Text"
                                    "  FROM TSession s,"
                                    "       TUser u,"
                                    "       TEnumText t1"
                                    " WHERE u.ID = s.UserID and t1.ID = s.Type "
                                    " ORDER BY s.StartTime DESC LIMIT %1, %2").arg(pageIndex * RowsPerPage).arg(RowsPerPage), db);
            model->setHeaderData(0, Qt::Horizontal, tr("User name"));//用户名
            model->setHeaderData(1, Qt::Horizontal, tr("Strat time"));//开始时间
            model->setHeaderData(2, Qt::Horizontal, tr("Finish time"));//结束时间
            model->setHeaderData(3, Qt::Horizontal, tr("Type"));//类型
            ui->sessionTableView->setModel(model);
            ui->sessionTableView->resizeColumnsToContents();
            break;
        }

        case SYSTEMEVENTTABLEVIEW: {
            model->clear();

            model->setQuery(QString("SELECT strftime('%m/%d %H:%M:%S', CreateTime, 'unixepoch', 'localtime')," 
                                    "       t1.Text," 
				                    "       EventContent "
                                    "       FROM TSystemEvent se, TEnumText t1 " 
				                    " WHERE t1.ID = se.EventType " 
				                    " ORDER BY CreateTime DESC LIMIT %1, %2").arg(pageIndex * RowsPerPage).arg(RowsPerPage), db);
            model->setHeaderData(0, Qt::Horizontal, tr("Time"));
            model->setHeaderData(1, Qt::Horizontal, tr("Event type"));//事件类型
            model->setHeaderData(2, Qt::Horizontal, tr("Event content"));//事件内容
            ui->systemEventTableView->setModel(model);
            ui->systemEventTableView->resizeColumnsToContents();
            break;
        }
    }
}

void TableViewDialog::onPrevButtonClicked()
{
    --pageIndex;
    showTab(ui->tabWidget->currentIndex());
}

void TableViewDialog::onNextButtonClicked()
{
    ++pageIndex;
    showTab(ui->tabWidget->currentIndex());
}
