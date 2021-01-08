/********************************************************************************
** Form generated from reading UI file 'AdminDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMINDIALOG_H
#define UI_ADMINDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdminDialog
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QToolButton *trainingButton;
    QToolButton *maintainButton;
    QToolButton *assistButton;
    QToolButton *educationButton;
    QToolButton *shutdownButton;
    QToolButton *recordButton;
    QToolButton *settingsButton;
    QToolButton *projectStartButton;
    QGroupBox *userInfoGroupBox;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QLabel *userPhotoLabel;
    QLabel *userInfoLabel;

    void setupUi(QDialog *AdminDialog)
    {
        if (AdminDialog->objectName().isEmpty())
            AdminDialog->setObjectName(QString::fromUtf8("AdminDialog"));
        AdminDialog->setWindowModality(Qt::WindowModal);
        AdminDialog->resize(606, 253);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AdminDialog->sizePolicy().hasHeightForWidth());
        AdminDialog->setSizePolicy(sizePolicy);
        AdminDialog->setMinimumSize(QSize(456, 253));
        AdminDialog->setMaximumSize(QSize(640, 253));
        AdminDialog->setContextMenuPolicy(Qt::NoContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/controller.ico"), QSize(), QIcon::Normal, QIcon::Off);
        AdminDialog->setWindowIcon(icon);
        AdminDialog->setStyleSheet(QString::fromUtf8("font-size: 20px;"));
        AdminDialog->setSizeGripEnabled(false);
        layoutWidget = new QWidget(AdminDialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 20, 439, 215));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setSpacing(11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        trainingButton = new QToolButton(layoutWidget);
        trainingButton->setObjectName(QString::fromUtf8("trainingButton"));
        trainingButton->setMinimumSize(QSize(101, 96));
        trainingButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/btn_training.png"), QSize(), QIcon::Normal, QIcon::Off);
        trainingButton->setIcon(icon1);
        trainingButton->setIconSize(QSize(64, 64));
        trainingButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(trainingButton, 0, 0, 1, 1);

        maintainButton = new QToolButton(layoutWidget);
        maintainButton->setObjectName(QString::fromUtf8("maintainButton"));
        maintainButton->setMinimumSize(QSize(101, 96));
        maintainButton->setFocusPolicy(Qt::NoFocus);
        maintainButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/btn_maintain.png"), QSize(), QIcon::Normal, QIcon::Off);
        maintainButton->setIcon(icon2);
        maintainButton->setIconSize(QSize(64, 64));
        maintainButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(maintainButton, 0, 2, 1, 1);

        assistButton = new QToolButton(layoutWidget);
        assistButton->setObjectName(QString::fromUtf8("assistButton"));
        assistButton->setMinimumSize(QSize(101, 96));
        assistButton->setFocusPolicy(Qt::NoFocus);
        assistButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/btn_assist.png"), QSize(), QIcon::Normal, QIcon::Off);
        assistButton->setIcon(icon3);
        assistButton->setIconSize(QSize(64, 64));
        assistButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(assistButton, 0, 3, 1, 1);

        educationButton = new QToolButton(layoutWidget);
        educationButton->setObjectName(QString::fromUtf8("educationButton"));
        educationButton->setMinimumSize(QSize(101, 96));
        educationButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/education.png"), QSize(), QIcon::Normal, QIcon::Off);
        educationButton->setIcon(icon4);
        educationButton->setIconSize(QSize(64, 64));
        educationButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(educationButton, 0, 1, 1, 1);

        shutdownButton = new QToolButton(layoutWidget);
        shutdownButton->setObjectName(QString::fromUtf8("shutdownButton"));
        shutdownButton->setMinimumSize(QSize(101, 96));
        shutdownButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/btn_shutdown.png"), QSize(), QIcon::Normal, QIcon::On);
        shutdownButton->setIcon(icon5);
        shutdownButton->setIconSize(QSize(64, 64));
        shutdownButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(shutdownButton, 1, 1, 1, 1);

        recordButton = new QToolButton(layoutWidget);
        recordButton->setObjectName(QString::fromUtf8("recordButton"));
        recordButton->setMinimumSize(QSize(101, 96));
        recordButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/btn_table.png"), QSize(), QIcon::Normal, QIcon::On);
        recordButton->setIcon(icon6);
        recordButton->setIconSize(QSize(64, 64));
        recordButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(recordButton, 1, 2, 1, 1);

        settingsButton = new QToolButton(layoutWidget);
        settingsButton->setObjectName(QString::fromUtf8("settingsButton"));
        settingsButton->setMinimumSize(QSize(101, 96));
        settingsButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/btn_settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        settingsButton->setIcon(icon7);
        settingsButton->setIconSize(QSize(64, 64));
        settingsButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        settingsButton->setAutoRaise(false);

        gridLayout->addWidget(settingsButton, 1, 3, 1, 1);

        projectStartButton = new QToolButton(layoutWidget);
        projectStartButton->setObjectName(QString::fromUtf8("projectStartButton"));
        projectStartButton->setMinimumSize(QSize(101, 96));
        projectStartButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/project.png"), QSize(), QIcon::Normal, QIcon::Off);
        projectStartButton->setIcon(icon8);
        projectStartButton->setIconSize(QSize(64, 64));
        projectStartButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

        gridLayout->addWidget(projectStartButton, 1, 0, 1, 1);

        userInfoGroupBox = new QGroupBox(AdminDialog);
        userInfoGroupBox->setObjectName(QString::fromUtf8("userInfoGroupBox"));
        userInfoGroupBox->setGeometry(QRect(450, 12, 150, 220));
        userInfoGroupBox->setMinimumSize(QSize(150, 220));
        userInfoGroupBox->setMaximumSize(QSize(150, 225));
        userInfoGroupBox->setAutoFillBackground(false);
        userInfoGroupBox->setFlat(false);
        layoutWidget1 = new QWidget(userInfoGroupBox);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 30, 132, 195));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        userPhotoLabel = new QLabel(layoutWidget1);
        userPhotoLabel->setObjectName(QString::fromUtf8("userPhotoLabel"));
        userPhotoLabel->setMinimumSize(QSize(130, 161));
        userPhotoLabel->setMaximumSize(QSize(130, 180));
        userPhotoLabel->setFrameShape(QFrame::NoFrame);
        userPhotoLabel->setFrameShadow(QFrame::Plain);
        userPhotoLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/user_default.png")));
        userPhotoLabel->setScaledContents(false);
        userPhotoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(userPhotoLabel);

        userInfoLabel = new QLabel(layoutWidget1);
        userInfoLabel->setObjectName(QString::fromUtf8("userInfoLabel"));
        userInfoLabel->setMaximumSize(QSize(130, 16777215));
        userInfoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(userInfoLabel);


        retranslateUi(AdminDialog);

        QMetaObject::connectSlotsByName(AdminDialog);
    } // setupUi

    void retranslateUi(QDialog *AdminDialog)
    {
        AdminDialog->setWindowTitle(QApplication::translate("AdminDialog", "\344\273\252\345\231\250\347\256\241\347\220\206\345\221\230\351\235\242\346\235\277", 0, QApplication::UnicodeUTF8));
        trainingButton->setText(QApplication::translate("AdminDialog", "Training", 0, QApplication::UnicodeUTF8));
        maintainButton->setText(QApplication::translate("AdminDialog", "Maintain", 0, QApplication::UnicodeUTF8));
        assistButton->setText(QApplication::translate("AdminDialog", "By Admin", 0, QApplication::UnicodeUTF8));
        educationButton->setText(QApplication::translate("AdminDialog", "Teaching", 0, QApplication::UnicodeUTF8));
        shutdownButton->setText(QApplication::translate("AdminDialog", "End", 0, QApplication::UnicodeUTF8));
        recordButton->setText(QApplication::translate("AdminDialog", "Record", 0, QApplication::UnicodeUTF8));
        settingsButton->setText(QApplication::translate("AdminDialog", "Setting", 0, QApplication::UnicodeUTF8));
        projectStartButton->setText(QApplication::translate("AdminDialog", "Project", 0, QApplication::UnicodeUTF8));
        userInfoGroupBox->setTitle(QApplication::translate("AdminDialog", "Active User", 0, QApplication::UnicodeUTF8));
        userPhotoLabel->setText(QString());
        userInfoLabel->setText(QApplication::translate("AdminDialog", "Username", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AdminDialog: public Ui_AdminDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMINDIALOG_H
