/********************************************************************************
** Form generated from reading UI file 'InputWidgetNum.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INPUTWIDGETNUM_H
#define UI_INPUTWIDGETNUM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InputWidgetNum
{
public:
    QPushButton *btn0;
    QPushButton *btn1;
    QPushButton *btn6;
    QPushButton *btn7;
    QPushButton *btn9;
    QPushButton *btnBackspace;
    QPushButton *btn2;
    QPushButton *btn5;
    QPushButton *btn4;
    QPushButton *btn3;
    QPushButton *btnPoint;
    QPushButton *btn8;

    void setupUi(QWidget *InputWidgetNum)
    {
        if (InputWidgetNum->objectName().isEmpty())
            InputWidgetNum->setObjectName(QString::fromUtf8("InputWidgetNum"));
        InputWidgetNum->resize(151, 201);
        InputWidgetNum->setStyleSheet(QString::fromUtf8("font-size: 20px;"));
        btn0 = new QPushButton(InputWidgetNum);
        btn0->setObjectName(QString::fromUtf8("btn0"));
        btn0->setGeometry(QRect(0, 150, 51, 51));
        btn1 = new QPushButton(InputWidgetNum);
        btn1->setObjectName(QString::fromUtf8("btn1"));
        btn1->setGeometry(QRect(0, 100, 51, 51));
        btn6 = new QPushButton(InputWidgetNum);
        btn6->setObjectName(QString::fromUtf8("btn6"));
        btn6->setGeometry(QRect(100, 50, 51, 51));
        btn7 = new QPushButton(InputWidgetNum);
        btn7->setObjectName(QString::fromUtf8("btn7"));
        btn7->setGeometry(QRect(0, 0, 51, 51));
        btn9 = new QPushButton(InputWidgetNum);
        btn9->setObjectName(QString::fromUtf8("btn9"));
        btn9->setGeometry(QRect(100, 0, 51, 51));
        btnBackspace = new QPushButton(InputWidgetNum);
        btnBackspace->setObjectName(QString::fromUtf8("btnBackspace"));
        btnBackspace->setGeometry(QRect(100, 150, 51, 51));
        btn2 = new QPushButton(InputWidgetNum);
        btn2->setObjectName(QString::fromUtf8("btn2"));
        btn2->setGeometry(QRect(50, 100, 51, 51));
        btn5 = new QPushButton(InputWidgetNum);
        btn5->setObjectName(QString::fromUtf8("btn5"));
        btn5->setGeometry(QRect(50, 50, 51, 51));
        btn4 = new QPushButton(InputWidgetNum);
        btn4->setObjectName(QString::fromUtf8("btn4"));
        btn4->setGeometry(QRect(0, 50, 51, 51));
        btn3 = new QPushButton(InputWidgetNum);
        btn3->setObjectName(QString::fromUtf8("btn3"));
        btn3->setGeometry(QRect(100, 100, 51, 51));
        btnPoint = new QPushButton(InputWidgetNum);
        btnPoint->setObjectName(QString::fromUtf8("btnPoint"));
        btnPoint->setGeometry(QRect(50, 150, 51, 51));
        btn8 = new QPushButton(InputWidgetNum);
        btn8->setObjectName(QString::fromUtf8("btn8"));
        btn8->setGeometry(QRect(50, 0, 51, 51));

        retranslateUi(InputWidgetNum);

        QMetaObject::connectSlotsByName(InputWidgetNum);
    } // setupUi

    void retranslateUi(QWidget *InputWidgetNum)
    {
        btn0->setText(QApplication::translate("InputWidgetNum", "0", 0, QApplication::UnicodeUTF8));
        btn1->setText(QApplication::translate("InputWidgetNum", "1", 0, QApplication::UnicodeUTF8));
        btn6->setText(QApplication::translate("InputWidgetNum", "6", 0, QApplication::UnicodeUTF8));
        btn7->setText(QApplication::translate("InputWidgetNum", "7", 0, QApplication::UnicodeUTF8));
        btn9->setText(QApplication::translate("InputWidgetNum", "9", 0, QApplication::UnicodeUTF8));
        btnBackspace->setText(QApplication::translate("InputWidgetNum", "<", 0, QApplication::UnicodeUTF8));
        btn2->setText(QApplication::translate("InputWidgetNum", "2", 0, QApplication::UnicodeUTF8));
        btn5->setText(QApplication::translate("InputWidgetNum", "5", 0, QApplication::UnicodeUTF8));
        btn4->setText(QApplication::translate("InputWidgetNum", "4", 0, QApplication::UnicodeUTF8));
        btn3->setText(QApplication::translate("InputWidgetNum", "3", 0, QApplication::UnicodeUTF8));
        btnPoint->setText(QApplication::translate("InputWidgetNum", ".", 0, QApplication::UnicodeUTF8));
        btn8->setText(QApplication::translate("InputWidgetNum", "8", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(InputWidgetNum);
    } // retranslateUi

};

namespace Ui {
    class InputWidgetNum: public Ui_InputWidgetNum {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INPUTWIDGETNUM_H
