#include <QtSql/QtSql>
#include <deviceinfo/version.h>
#include <controller/Context/AdminAction.h>
#include <tools/log.h>
#include <build/QT/include/QtGui/QMessageBox>
#include <deviceinfo/enum.h>
#include <controller/Context/ServiceEvent.h>
#include <controller/Base/NormalTools.h>
#include <build/QT/include/QtGui/QAbstractItemView>
#include "controller/Context/WindowContext.h"
#include "controller/Context/InstrumentStatus.h"
#include "controller/Context/UserInformation.h"
#include "controller/UI/EntrustEndDialog.h"
#include "controller/UI/AdminDialog.h"
#include "controller/UI/QSqlQueryCheckboxModel.h"
#include "ui_EntrustEndDialog.h"

#include "controller/Base/GlobalData.h"
#include <tools/IniParse.h>
static const int RowsPerPage = 12;

EntrustEndDialog::EntrustEndDialog(QWidget *parent) : QDialog(parent), ui(new Ui::EntrustEndDialog) {
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    setAttribute(Qt::WA_DeleteOnClose);

    IniParse osla(OSLA_CONFIG);
    GlobalData::currentLocale = osla.GetString("Global.Language").c_str();
    if (GlobalData::currentLocale.isEmpty()) {
        GlobalData::currentLocale = "en_US";
    }
    if(QString::compare(GlobalData::currentLocale,"zh_CN") == 0){
        model = new QSqlQueryCheckboxModel(this, 0, "结束委托");
    }
    else{
         model = new QSqlQueryCheckboxModel(this, 0, "Entrust end");
    }

    connect(ui->prevButton, SIGNAL(clicked()), this, SLOT(onPrevButtonClicked()));
    connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(onNextButtonClicked()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->endButton, SIGNAL(clicked()), this, SLOT(endSession()));
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

EntrustEndDialog::~EntrustEndDialog() {
    DEBUG_LOG("");
    delete ui;
}

void EntrustEndDialog::showEvent(QShowEvent *) {
    DEBUG_LOG("");
    pageIndex = 0;
    std::list<int> status;
    status.push_back((int) EntrustStatus::DONT_DO);
    status.push_back((int) EntrustStatus::DONT_OVER);
    /// 取实验中，并且项目未结束／未执行的项目
    SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();
    pageCount = sqlWorker->countValidEntrustList(status, 1) / RowsPerPage;
    showEntrust();
    DEBUG_LOG("");
}

void EntrustEndDialog::showEntrust() {
    DEBUG_LOG("");
    ui->prevButton->setEnabled(pageIndex > 0);
    ui->nextButton->setEnabled(pageIndex < pageCount);
    ui->pageLabel->setText(tr("%1/%2").arg(pageIndex + 1).arg(pageCount + 1));

    SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();
    std::list<int> status;
    status.push_back((int) EntrustStatus::DONT_DO);
    status.push_back((int) EntrustStatus::DONT_OVER);
    biotech::osla::EntrustList l = sqlWorker->getEntrustList(pageIndex * RowsPerPage, RowsPerPage, status, 1);
    nowDoEntrustList.MergeFrom(l);
    model->updateData(l);
    DEBUG_LOG("");
}

void EntrustEndDialog::onPrevButtonClicked() {
    --pageIndex;
    showEntrust();
}

void EntrustEndDialog::onNextButtonClicked() {
    ++pageIndex;
    showEntrust();
}

void EntrustEndDialog::endSession() {
    DEBUG_LOG("");

    do {
        std::set<int> &checkedEntrust = model->getCheckedEntrust();
        DEBUG_LOG("checked Entrust size is " << checkedEntrust.size());
        if (checkedEntrust.size() == 0) {
            //tr("注意"),tr("未选中任何委托项目，是否结束实验？"),
            QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                                      tr("No commissioned item is selected.\nWhether to continue the experiment?"),
                                                      QMessageBox::Yes | QMessageBox::No, (QWidget *) (parent()));
            messageBox->setDefaultButton(QMessageBox::No);
            messageBox->setAttribute(Qt::WA_DeleteOnClose);

            if (messageBox->exec() != QMessageBox::Yes) {
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

        SqlWorker* sqlWorker = SqlWorker::GetSqlWorkByThread();
        quint64 lastId = sqlWorker->getLastId("select ID from TSession order by id desc limit 1");

        DEBUG_LOG("");
        sessionManage->EndSession();

        for (long long v : checkedEntrust) {
            biotech::osla::EntrustEvent entrustEvent;
            entrustEvent.set_entrust_id(v);
            entrustEvent.set_status(EntrustStatus::HAVA_OVER);
            entrustEvent.set_is_doing(0);
            entrustEvent.set_session_id(lastId);
            SE_G->AddEntrustEvent(entrustEvent);

            biotech::osla::Entrust entrust;
            entrust.set_id(v);
            entrust.set_status(EntrustStatus::HAVA_OVER);
            entrust.set_is_doing(0);
            sqlWorker->updateEntrustStatus(entrust);
        }

        for (int i = 0; i < nowDoEntrustList.entrust_size(); ++i) {
            const biotech::osla::Entrust &entrust = nowDoEntrustList.entrust(i);
            if (checkedEntrust.end() != checkedEntrust.find(entrust.id())) {
                continue;
            }
            biotech::osla::EntrustEvent entrustEvent;
            entrustEvent.set_entrust_id(entrust.id());
            entrustEvent.set_status(EntrustStatus::DONT_OVER);
            entrustEvent.set_is_doing(0);
            entrustEvent.set_session_id(lastId);
            SE_G->AddEntrustEvent(entrustEvent);

            biotech::osla::Entrust upEntrust;
            upEntrust.set_id(entrust.id());
            upEntrust.set_status(EntrustStatus::DONT_OVER);
            upEntrust.set_is_doing(0);
            sqlWorker->updateEntrustStatus(upEntrust);
        }
    } while (false);

    close();
}

void EntrustEndDialog::init(WindowContext *ctx, SessionManage *se) {
    context = ctx;
    sessionManage = se;
}

bool EntrustEndDialog::IsContinueUseInstrument() {
    DEBUG_LOG("");
    UserInformation *userInformation = context->getUserInformation();
    User &newUser = userInformation->GetNewUser();
    //tr("注意"), tr("仪器正在使用中，是否结束当前实验？")
    QMessageBox *messageBox = new QMessageBox(QMessageBox::Question, tr("Notice"),
                                              tr("The instrument is in use,shall we continue the experiment?"),
                                              QMessageBox::Yes | QMessageBox::No, (QWidget *) (parent()));
    messageBox->setDefaultButton(QMessageBox::No);
    messageBox->setAttribute(Qt::WA_DeleteOnClose);

    if (messageBox->exec() != QMessageBox::Yes) { // 如果不继续，则直接退出
        SE_G->AddUserEvent(newUser, biotech::osla::USER_ACTION_NULL_OP, biotech::osla::USER_ACTION_NULL_RESULT);
        return true;
    }
    // 如果继续，先结束当前使用
    //DEBUG_LOG("");
    //sessionManage->EndSession();

    return false;
}

void EntrustEndDialog::chooseClick(QModelIndex index) {
    if (model->data(index, Qt::CheckStateRole) == Qt::Unchecked) {
        model->setData(index, Qt::Checked, Qt::CheckStateRole);
    } else if (model->data(index, Qt::CheckStateRole) == Qt::Checked) {
        model->setData(index, Qt::Unchecked | Qt::PartiallyChecked, Qt::CheckStateRole);
    }
}
