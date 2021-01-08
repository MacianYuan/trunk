/********************************************************************************
** Form generated from reading UI file 'EntrustBeginDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTRUSTBEGINDIALOG_H
#define UI_ENTRUSTBEGINDIALOG_H

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
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EntrustBeginDialog
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_3;
    QTableView *entrustTableView;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *selectDontOverButton;
    QPushButton *prevButton;
    QLabel *pageLabel;
    QPushButton *nextButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *closeButton;
    QPushButton *beginButton;

    void setupUi(QDialog *EntrustBeginDialog)
    {
        if (EntrustBeginDialog->objectName().isEmpty())
            EntrustBeginDialog->setObjectName(QString::fromUtf8("EntrustBeginDialog"));
        EntrustBeginDialog->resize(637, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/controller.ico"), QSize(), QIcon::Normal, QIcon::Off);
        EntrustBeginDialog->setWindowIcon(icon);
        layoutWidget = new QWidget(EntrustBeginDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(8, 9, 621, 461));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(13, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);

        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);

        horizontalSpacer_3 = new QSpacerItem(13, 13, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_2);

        entrustTableView = new QTableView(layoutWidget);
        entrustTableView->setObjectName(QString::fromUtf8("entrustTableView"));

        verticalLayout->addWidget(entrustTableView);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(10, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        selectDontOverButton = new QPushButton(layoutWidget);
        selectDontOverButton->setObjectName(QString::fromUtf8("selectDontOverButton"));
        selectDontOverButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(selectDontOverButton);

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

        beginButton = new QPushButton(layoutWidget);
        beginButton->setObjectName(QString::fromUtf8("beginButton"));
        beginButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/btn_start.png"), QSize(), QIcon::Normal, QIcon::Off);
        beginButton->setIcon(icon4);

        horizontalLayout->addWidget(beginButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(EntrustBeginDialog);

        QMetaObject::connectSlotsByName(EntrustBeginDialog);
    } // setupUi

    void retranslateUi(QDialog *EntrustBeginDialog)
    {
        EntrustBeginDialog->setWindowTitle(QApplication::translate("EntrustBeginDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("EntrustBeginDialog", "Start by the order mode", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        selectDontOverButton->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        selectDontOverButton->setText(QApplication::translate("EntrustBeginDialog", "Selection incomplete", 0, QApplication::UnicodeUTF8));
        prevButton->setText(QApplication::translate("EntrustBeginDialog", "Back", 0, QApplication::UnicodeUTF8));
        pageLabel->setText(QApplication::translate("EntrustBeginDialog", "0/0", 0, QApplication::UnicodeUTF8));
        nextButton->setText(QApplication::translate("EntrustBeginDialog", "Next", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("EntrustBeginDialog", "Close", 0, QApplication::UnicodeUTF8));
        beginButton->setText(QApplication::translate("EntrustBeginDialog", "Start", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EntrustBeginDialog: public Ui_EntrustBeginDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTRUSTBEGINDIALOG_H
