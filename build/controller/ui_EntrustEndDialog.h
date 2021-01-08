/********************************************************************************
** Form generated from reading UI file 'EntrustEndDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENTRUSTENDDIALOG_H
#define UI_ENTRUSTENDDIALOG_H

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

class Ui_EntrustEndDialog
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
    QPushButton *prevButton;
    QLabel *pageLabel;
    QPushButton *nextButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *closeButton;
    QPushButton *endButton;

    void setupUi(QDialog *EntrustEndDialog)
    {
        if (EntrustEndDialog->objectName().isEmpty())
            EntrustEndDialog->setObjectName(QString::fromUtf8("EntrustEndDialog"));
        EntrustEndDialog->resize(637, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/controller.ico"), QSize(), QIcon::Normal, QIcon::Off);
        EntrustEndDialog->setWindowIcon(icon);
        layoutWidget = new QWidget(EntrustEndDialog);
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

        endButton = new QPushButton(layoutWidget);
        endButton->setObjectName(QString::fromUtf8("endButton"));
        endButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/btn_end.png"), QSize(), QIcon::Normal, QIcon::Off);
        endButton->setIcon(icon4);

        horizontalLayout->addWidget(endButton);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(EntrustEndDialog);

        QMetaObject::connectSlotsByName(EntrustEndDialog);
    } // setupUi

    void retranslateUi(QDialog *EntrustEndDialog)
    {
        EntrustEndDialog->setWindowTitle(QApplication::translate("EntrustEndDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("EntrustEndDialog", "\351\241\271\347\233\256\345\247\224\346\211\230\345\205\263\346\234\272", 0, QApplication::UnicodeUTF8));
        prevButton->setText(QApplication::translate("EntrustEndDialog", "\344\270\212\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        pageLabel->setText(QApplication::translate("EntrustEndDialog", "0/0", 0, QApplication::UnicodeUTF8));
        nextButton->setText(QApplication::translate("EntrustEndDialog", "\344\270\213\344\270\200\351\241\265", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("EntrustEndDialog", "\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
        endButton->setText(QApplication::translate("EntrustEndDialog", "\347\273\223\346\235\237", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EntrustEndDialog: public Ui_EntrustEndDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENTRUSTENDDIALOG_H
