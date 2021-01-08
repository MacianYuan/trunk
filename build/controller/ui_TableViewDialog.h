/********************************************************************************
** Form generated from reading UI file 'TableViewDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABLEVIEWDIALOG_H
#define UI_TABLEVIEWDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TableViewDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *userEventTab;
    QTableView *userEventTableView;
    QWidget *sessionTab;
    QTableView *sessionTableView;
    QWidget *systemEventTab;
    QTableView *systemEventTableView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *prevButton;
    QLabel *pageLabel;
    QPushButton *nextButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *closeButton;

    void setupUi(QDialog *TableViewDialog)
    {
        if (TableViewDialog->objectName().isEmpty())
            TableViewDialog->setObjectName(QString::fromUtf8("TableViewDialog"));
        TableViewDialog->resize(637, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/controller.ico"), QSize(), QIcon::Normal, QIcon::Off);
        TableViewDialog->setWindowIcon(icon);
        layoutWidget = new QWidget(TableViewDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(8, 9, 621, 461));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(layoutWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setFocusPolicy(Qt::NoFocus);
        tabWidget->setTabPosition(QTabWidget::North);
        tabWidget->setTabShape(QTabWidget::Triangular);
        tabWidget->setElideMode(Qt::ElideNone);
        tabWidget->setUsesScrollButtons(true);
        userEventTab = new QWidget();
        userEventTab->setObjectName(QString::fromUtf8("userEventTab"));
        userEventTableView = new QTableView(userEventTab);
        userEventTableView->setObjectName(QString::fromUtf8("userEventTableView"));
        userEventTableView->setGeometry(QRect(0, 0, 621, 391));
        userEventTableView->setFocusPolicy(Qt::NoFocus);
        userEventTableView->setFrameShape(QFrame::NoFrame);
        userEventTableView->setFrameShadow(QFrame::Plain);
        userEventTableView->setSelectionMode(QAbstractItemView::NoSelection);
        userEventTableView->horizontalHeader()->setHighlightSections(false);
        userEventTableView->verticalHeader()->setVisible(false);
        tabWidget->addTab(userEventTab, QString());
        sessionTab = new QWidget();
        sessionTab->setObjectName(QString::fromUtf8("sessionTab"));
        sessionTableView = new QTableView(sessionTab);
        sessionTableView->setObjectName(QString::fromUtf8("sessionTableView"));
        sessionTableView->setGeometry(QRect(0, 0, 621, 391));
        sessionTableView->setFocusPolicy(Qt::NoFocus);
        sessionTableView->setFrameShape(QFrame::NoFrame);
        sessionTableView->setFrameShadow(QFrame::Plain);
        sessionTableView->setSelectionMode(QAbstractItemView::NoSelection);
        sessionTableView->verticalHeader()->setVisible(false);
        tabWidget->addTab(sessionTab, QString());
        systemEventTab = new QWidget();
        systemEventTab->setObjectName(QString::fromUtf8("systemEventTab"));
        systemEventTableView = new QTableView(systemEventTab);
        systemEventTableView->setObjectName(QString::fromUtf8("systemEventTableView"));
        systemEventTableView->setGeometry(QRect(0, 0, 621, 391));
        systemEventTableView->setFocusPolicy(Qt::NoFocus);
        systemEventTableView->setFrameShape(QFrame::NoFrame);
        systemEventTableView->setFrameShadow(QFrame::Plain);
        systemEventTableView->setSelectionMode(QAbstractItemView::NoSelection);
        systemEventTableView->verticalHeader()->setVisible(false);
        tabWidget->addTab(systemEventTab, QString());

        verticalLayout->addWidget(tabWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        prevButton = new QPushButton(layoutWidget);
        prevButton->setObjectName(QString::fromUtf8("prevButton"));
        prevButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/arrow_left.png"), QSize(), QIcon::Normal, QIcon::Off);
        prevButton->setIcon(icon1);

        horizontalLayout->addWidget(prevButton);

        pageLabel = new QLabel(layoutWidget);
        pageLabel->setObjectName(QString::fromUtf8("pageLabel"));

        horizontalLayout->addWidget(pageLabel);

        nextButton = new QPushButton(layoutWidget);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/arrow_right.png"), QSize(), QIcon::Normal, QIcon::Off);
        nextButton->setIcon(icon2);

        horizontalLayout->addWidget(nextButton);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        closeButton = new QPushButton(layoutWidget);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/btn_close.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon3);

        horizontalLayout->addWidget(closeButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(TableViewDialog);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TableViewDialog);
    } // setupUi

    void retranslateUi(QDialog *TableViewDialog)
    {
        TableViewDialog->setWindowTitle(QApplication::translate("TableViewDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(userEventTab), QApplication::translate("TableViewDialog", " \345\210\267\345\215\241\350\256\260\345\275\225 ", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(sessionTab), QApplication::translate("TableViewDialog", " \344\275\277\347\224\250\350\256\260\345\275\225 ", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(systemEventTab), QApplication::translate("TableViewDialog", " \347\263\273\347\273\237\344\272\213\344\273\266 ", 0, QApplication::UnicodeUTF8));
        prevButton->setText(QApplication::translate("TableViewDialog", "\344\270\212\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        pageLabel->setText(QApplication::translate("TableViewDialog", "0/0", 0, QApplication::UnicodeUTF8));
        nextButton->setText(QApplication::translate("TableViewDialog", "\344\270\213\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("TableViewDialog", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class TableViewDialog: public Ui_TableViewDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABLEVIEWDIALOG_H
