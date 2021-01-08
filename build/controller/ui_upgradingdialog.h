/********************************************************************************
** Form generated from reading UI file 'upgradingdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UPGRADINGDIALOG_H
#define UI_UPGRADINGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_upgradingDialog
{
public:
    QLabel *energyLabel;

    void setupUi(QDialog *upgradingDialog)
    {
        if (upgradingDialog->objectName().isEmpty())
            upgradingDialog->setObjectName(QString::fromUtf8("upgradingDialog"));
        upgradingDialog->resize(640, 480);
        energyLabel = new QLabel(upgradingDialog);
        energyLabel->setObjectName(QString::fromUtf8("energyLabel"));
        energyLabel->setGeometry(QRect(20, 10, 591, 361));
        QFont font;
        font.setFamily(QString::fromUtf8("Noto Sans CJK SC"));
        font.setPointSize(36);
        font.setBold(true);
        font.setWeight(75);
        energyLabel->setFont(font);
        energyLabel->setAlignment(Qt::AlignCenter);
        energyLabel->setWordWrap(true);

        retranslateUi(upgradingDialog);

        QMetaObject::connectSlotsByName(upgradingDialog);
    } // setupUi

    void retranslateUi(QDialog *upgradingDialog)
    {
        upgradingDialog->setWindowTitle(QApplication::translate("upgradingDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        energyLabel->setText(QApplication::translate("upgradingDialog", "\346\255\243\345\234\250\346\233\264\346\226\260\344\270\255\357\274\214\346\233\264\346\226\260\345\256\214\346\210\220\344\274\232\350\207\252\345\212\250\351\207\215\345\220\257\343\200\202\350\257\267\345\213\277\345\210\207\346\226\255\347\224\265\346\272\220...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class upgradingDialog: public Ui_upgradingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPGRADINGDIALOG_H
