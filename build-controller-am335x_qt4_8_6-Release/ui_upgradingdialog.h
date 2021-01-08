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
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(16);
        font.setBold(false);
        font.setItalic(false);
        font.setWeight(50);
        energyLabel->setFont(font);
        energyLabel->setStyleSheet(QString::fromUtf8("font: 16pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        energyLabel->setAlignment(Qt::AlignCenter);
        energyLabel->setWordWrap(true);

        retranslateUi(upgradingDialog);

        QMetaObject::connectSlotsByName(upgradingDialog);
    } // setupUi

    void retranslateUi(QDialog *upgradingDialog)
    {
        upgradingDialog->setWindowTitle(QApplication::translate("upgradingDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        energyLabel->setText(QApplication::translate("upgradingDialog", "Upgrading, automatic restart after the upgrade is done. Don't cut off the power!", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class upgradingDialog: public Ui_upgradingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPGRADINGDIALOG_H
