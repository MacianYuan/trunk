#include <QtSql/QtSql>
#include <deviceinfo/version.h>
#include <controller/Context/AdminAction.h>
#include <tools/log.h>
#include <build/QT/include/QtGui/QMessageBox>
#include <deviceinfo/enum.h>
#include <controller/Context/ServiceEvent.h>
#include <controller/Base/NormalTools.h>
#include "controller/Context/WindowContext.h"
#include "controller/Context/InstrumentStatus.h"
#include "controller/Context/UserInformation.h"
#include "controller/UI/EntrustBeginDialog.h"
#include "controller/UI/AdminDialog.h"
#include "controller/UI/QSqlQueryCheckboxModel.h"
#include "ui_EntrustBeginDialog.h"
#include "controller/Base/GlobalData.h"
#include <tools/IniParse.h>
static const int RowsPerPage = 12;

EntrustBeginDialog::EntrustBeginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::EntrustBeginDialog) {
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_DeleteOnClose);

    IniParse osla(OSLA_CONFIG);
    GlobalData::currentLocale = osla.GetString("Global.Language").c_str();
    if (GlobalData::currentLocale.isEmpty()) {
        GlobalData::currentLocale = "en_US";
    }
    if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0){
         model = new QSqlQueryCheckboxModel(this, 0, "开始委托");
    }
    else{
         model = new QSqlQueryCheckboxModel(this, 0, "Entrust begin");
    }

    connect(ui->prevButton, SIGNAL(clicked()), this, SLOT(onPrevButtonClicked()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(onNextButtonClicked()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->beginButton, SIGNAL(clicked()), this, SLOT(startSession()));
    connect(ui->selectDontOverButton, SIGNAL(clicked()), this, SLOT(selectDontOverClicked()));
    connect(ui->entrustTableView, SIGNAL(clicked(QModelIndex)), this, SLOT(chooseClick(QModelIndex)));

    // 设置单行选中、最后一列拉伸、表头不高亮、无边框等
    ui->entrustTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->entrustTableView->horizontalHeader()->setStretchLastSection(true);
    ui->entrustTableView->horizontalHeader()->setHighlightSections(false);
    ui->entrustTableView->verticalHeader()->setVisible(false);
    ui->entrustTableView->setShowGrid(true);
    ui->entrustTableView->setFrameShape(QFrame::NoFrame);
    ui->entrustTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->entrustTableView->setModel(model);
    ui->entrustTableView->setColumnWidth(1, 300);
}

EntrustBeginDialog::~EntrustBeginDialog() {
    delete ui;
}

void EntrustBeginDialog::showEvent(QShowEvent *) {
    pageIndex = 0;

    std::list<int> status;
    status.push_back((int) EntrustStatus::DONT_DO);
    status.push_back((int) EntrustStatus::DONT_OVER);
    SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
    pageCount = sqlWorker->countValidEntrustList(status, 2) / RowsPerPage;
    showEntrust();
}

void EntrustBeginDialog::showEntrust() {
    ui->prevButton->setEnabled(pageIndex > 0);
    ui->nextButton->setEnabled(pageIndex < pageCount);
    ui->pageLabel->setText(tr("%1/%2").arg(pageIndex + 1).arg(pageCount + 1));

    std::list<int> status;
    status.push_back((int) EntrustStatus::DONT_DO);
    status.push_back((int) EntrustStatus::DONT_OVER);

    SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
    biotech::osla::EntrustList l = sqlWorker->getEntrustList(pageIndex * RowsPerPage, RowsPerPage, status, 2);
    model->updateData(l);

    selectDontOverClicked();
}

void EntrustBeginDialog::onPrevButtonClicked() {
    --pageIndex;
    showEntrust();
}

void EntrustBeginDialog::onNextButtonClicked() {
    ++pageIndex;
    showEntrust();
}

void EntrustBeginDialog::startSession() {
    DEBUG_LOG("");

    do {
        std::set<int> &checkedEntrust = model->getCheckedEntrust();
        DEBUG_LOG("checked Entrust size is " << checkedEntrust.size());
        if (checkedEntrust.size() == 0) {
            //tr("注意"),tr("未选中任何委托项目，是否继续实验？"),
            QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                                      tr("No commissioned item is selected.\nWhether to continue the experiment?"),
                                                      QMessageBox::Yes | QMessageBox::No, (QWidget *) (parent()));
            messageBox->setDefaultButton(QMessageBox::No);
            messageBox->setAttribute(Qt::WA_DeleteOnClose);

            if (messageBox->exec() != QMessageBox::Yes) { // 如果不继续，则直接退出
                break;
            }
        }

        InstrumentStatus *instrumentStatus = context->getInstrumentStatus();
        if (true == instrumentStatus->GetInUseStatus() && true == IsContinueUseInstrument()) {
            break;
        }
        UserInformation *userInformation = context->getUserInformation();
        User &newUser = userInformation->GetNewUser();

        if (false == instrumentStatus->InstrumentStatusCheck(newUser)) {
            break;
        }

        SessionNode node = {biotech::osla::SESSION_TYPE_ENTRUST,
                            InstrumentStatus::ENTRUST, biotech::osla::USER_ACTION_START_ENTRUST};

        if (false == sessionManage->StartSession(node)) {
            break;
        }

        NTG()->SetDeviceWorkStatusLed(true);

        SqlWorker *sqlWorker = SqlWorker::GetSqlWorkByThread();
        quint64 lastId = sqlWorker->getLastId("select ID from TSession order by id desc limit 1");
        for (long long v : checkedEntrust) {
            biotech::osla::EntrustEvent entrustEvent;
            entrustEvent.set_entrust_id(v);
            entrustEvent.set_status(EntrustStatus::DONT_OVER);
            entrustEvent.set_is_doing(1);
            entrustEvent.set_session_id(lastId);
            SE_G->AddEntrustEvent(entrustEvent);
            biotech::osla::Entrust entrust;
            entrust.set_id(v);
            entrust.set_status(EntrustStatus::DONT_OVER);
            entrust.set_is_doing(1);
            sqlWorker->updateEntrustStatus(entrust);
        }
    } while (false);

    close();
}

void EntrustBeginDialog::init(WindowContext *ctx, SessionManage *se) {
    context = ctx;
    sessionManage = se;
}

bool EntrustBeginDialog::IsContinueUseInstrument() {
    DEBUG_LOG("");
    UserInformation *userInformation = context->getUserInformation();
    User &newUser = userInformation->GetNewUser();
    //tr("注意"),tr("仪器正在使用中，是否结束当前实验？"),
    QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                              tr("The instrument is in use,shall we continue the experiment"),
                                              QMessageBox::Yes | QMessageBox::No, (QWidget *) (parent()));
    messageBox->setDefaultButton(QMessageBox::No);
    messageBox->setAttribute(Qt::WA_DeleteOnClose);

    if (messageBox->exec() != QMessageBox::Yes) { // 如果不继续，则直接退出
        SE_G->AddUserEvent(newUser, biotech::osla::USER_ACTION_NULL_OP, biotech::osla::USER_ACTION_NULL_RESULT);
        return true;
    }

    // 如果继续，先结束当前使用
    sessionManage->EndSession();

    return false;
}

void EntrustBeginDialog::chooseClick(QModelIndex index) {
    if (model->data(index, Qt::CheckStateRole) == Qt::Unchecked) {
        model->setData(index, Qt::Checked, Qt::CheckStateRole);
    } else if (model->data(index, Qt::CheckStateRole) == Qt::Checked) {
        model->setData(index, Qt::Unchecked | Qt::PartiallyChecked, Qt::CheckStateRole);
    }
}

void EntrustBeginDialog::selectDontOverClicked()
{
    QModelIndex parent;

    for (int row = 0; row < model->rowCount(parent); row++)
    {
        int status;
        QModelIndex index = model->index(row, 3);
        if (model->getStatus(index, &status))
        {
            if (status == EntrustStatus::DONT_OVER)
            {
                model->setData(index, Qt::Checked, Qt::CheckStateRole);
            }
        }
    }
}
