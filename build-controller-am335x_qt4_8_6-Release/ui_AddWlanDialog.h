/********************************************************************************
** Form generated from reading UI file 'AddWlanDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDWLANDIALOG_H
#define UI_ADDWLANDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_AddWlanDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *NetworkName;
    QLineEdit *lineEdit_ssid;
    QLabel *Password;
    QLineEdit *lineEdit_password;
    QLabel *Bssid;
    QLineEdit *lineEdit_bssid;

    void setupUi(QDialog *AddWlanDialog)
    {
        if (AddWlanDialog->objectName().isEmpty())
            AddWlanDialog->setObjectName(QString::fromUtf8("AddWlanDialog"));
        AddWlanDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(AddWlanDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(0, 260, 181, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        NetworkName = new QLabel(AddWlanDialog);
        NetworkName->setObjectName(QString::fromUtf8("NetworkName"));
        NetworkName->setGeometry(QRect(20, 20, 381, 17));
        lineEdit_ssid = new QLineEdit(AddWlanDialog);
        lineEdit_ssid->setObjectName(QString::fromUtf8("lineEdit_ssid"));
        lineEdit_ssid->setGeometry(QRect(20, 40, 161, 25));
        Password = new QLabel(AddWlanDialog);
        Password->setObjectName(QString::fromUtf8("Password"));
        Password->setGeometry(QRect(20, 80, 381, 17));
        lineEdit_password = new QLineEdit(AddWlanDialog);
        lineEdit_password->setObjectName(QString::fromUtf8("lineEdit_password"));
        lineEdit_password->setGeometry(QRect(20, 100, 161, 25));
        Bssid = new QLabel(AddWlanDialog);
        Bssid->setObjectName(QString::fromUtf8("Bssid"));
        Bssid->setGeometry(QRect(20, 140, 381, 17));
        lineEdit_bssid = new QLineEdit(AddWlanDialog);
        lineEdit_bssid->setObjectName(QString::fromUtf8("lineEdit_bssid"));
        lineEdit_bssid->setGeometry(QRect(20, 160, 161, 25));

        retranslateUi(AddWlanDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AddWlanDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AddWlanDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AddWlanDialog);
    } // setupUi

    void retranslateUi(QDialog *AddWlanDialog)
    {
        AddWlanDialog->setWindowTitle(QApplication::translate("AddWlanDialog", "Add WLAN ", 0, QApplication::UnicodeUTF8));
        NetworkName->setText(QApplication::translate("AddWlanDialog", "Network name", 0, QApplication::UnicodeUTF8));
        Password->setText(QApplication::translate("AddWlanDialog", "Password", 0, QApplication::UnicodeUTF8));
        Bssid->setText(QApplication::translate("AddWlanDialog", "Bssid", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AddWlanDialog: public Ui_AddWlanDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDWLANDIALOG_H
