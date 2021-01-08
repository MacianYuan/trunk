/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *pushButtonSpeakerOpen;
    QPushButton *pushButtonScreenCalibration;
    QPushButton *pushButtonScreenTest;
    QPushButton *pushButtonSpeakerClose;
    QPushButton *pushButtonBeepOpen;
    QPushButton *pushButtonBeepClose;
    QPushButton *pushButtonCardClose;
    QPushButton *pushButtonCardOpen;
    QSlider *horizontalSliderSoundSize;
    QSplitter *splitter;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QPushButton *pushButtonEthTest;
    QPushButton *pushButtonWifiTest;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *labelSDCardStatus;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(680, 480);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButtonSpeakerOpen = new QPushButton(centralWidget);
        pushButtonSpeakerOpen->setObjectName(QString::fromUtf8("pushButtonSpeakerOpen"));
        pushButtonSpeakerOpen->setGeometry(QRect(216, 128, 80, 30));
        QFont font;
        font.setPointSize(12);
        pushButtonSpeakerOpen->setFont(font);
        pushButtonScreenCalibration = new QPushButton(centralWidget);
        pushButtonScreenCalibration->setObjectName(QString::fromUtf8("pushButtonScreenCalibration"));
        pushButtonScreenCalibration->setGeometry(QRect(160, 342, 81, 31));
        pushButtonScreenCalibration->setFont(font);
        pushButtonScreenTest = new QPushButton(centralWidget);
        pushButtonScreenTest->setObjectName(QString::fromUtf8("pushButtonScreenTest"));
        pushButtonScreenTest->setGeometry(QRect(286, 340, 81, 31));
        pushButtonScreenTest->setFont(font);
        pushButtonSpeakerClose = new QPushButton(centralWidget);
        pushButtonSpeakerClose->setObjectName(QString::fromUtf8("pushButtonSpeakerClose"));
        pushButtonSpeakerClose->setGeometry(QRect(336, 128, 80, 30));
        pushButtonSpeakerClose->setFont(font);
        pushButtonBeepOpen = new QPushButton(centralWidget);
        pushButtonBeepOpen->setObjectName(QString::fromUtf8("pushButtonBeepOpen"));
        pushButtonBeepOpen->setGeometry(QRect(216, 178, 80, 30));
        pushButtonBeepOpen->setFont(font);
        pushButtonBeepClose = new QPushButton(centralWidget);
        pushButtonBeepClose->setObjectName(QString::fromUtf8("pushButtonBeepClose"));
        pushButtonBeepClose->setGeometry(QRect(336, 178, 80, 30));
        pushButtonBeepClose->setFont(font);
        pushButtonCardClose = new QPushButton(centralWidget);
        pushButtonCardClose->setObjectName(QString::fromUtf8("pushButtonCardClose"));
        pushButtonCardClose->setGeometry(QRect(336, 228, 80, 30));
        pushButtonCardClose->setFont(font);
        pushButtonCardOpen = new QPushButton(centralWidget);
        pushButtonCardOpen->setObjectName(QString::fromUtf8("pushButtonCardOpen"));
        pushButtonCardOpen->setGeometry(QRect(216, 228, 80, 30));
        pushButtonCardOpen->setFont(font);
        horizontalSliderSoundSize = new QSlider(centralWidget);
        horizontalSliderSoundSize->setObjectName(QString::fromUtf8("horizontalSliderSoundSize"));
        horizontalSliderSoundSize->setGeometry(QRect(436, 138, 160, 19));
        horizontalSliderSoundSize->setMaximum(5);
        horizontalSliderSoundSize->setOrientation(Qt::Horizontal);
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(105, 118, 96, 141));
        splitter->setOrientation(Qt::Vertical);
        label = new QLabel(splitter);
        label->setObjectName(QString::fromUtf8("label"));
        label->setFont(font);
        splitter->addWidget(label);
        label_2 = new QLabel(splitter);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        splitter->addWidget(label_2);
        label_3 = new QLabel(splitter);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);
        splitter->addWidget(label_3);
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(180, 30, 141, 31));
        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(180, 69, 141, 31));
        pushButtonEthTest = new QPushButton(centralWidget);
        pushButtonEthTest->setObjectName(QString::fromUtf8("pushButtonEthTest"));
        pushButtonEthTest->setGeometry(QRect(370, 30, 101, 31));
        pushButtonWifiTest = new QPushButton(centralWidget);
        pushButtonWifiTest->setObjectName(QString::fromUtf8("pushButtonWifiTest"));
        pushButtonWifiTest->setGeometry(QRect(370, 70, 101, 31));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(100, 40, 71, 20));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(100, 80, 71, 21));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(120, 290, 54, 21));
        labelSDCardStatus = new QLabel(centralWidget);
        labelSDCardStatus->setObjectName(QString::fromUtf8("labelSDCardStatus"));
        labelSDCardStatus->setGeometry(QRect(220, 290, 191, 21));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 680, 23));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        QWidget::setTabOrder(pushButtonSpeakerOpen, pushButtonScreenCalibration);
        QWidget::setTabOrder(pushButtonScreenCalibration, pushButtonBeepClose);
        QWidget::setTabOrder(pushButtonBeepClose, pushButtonBeepOpen);
        QWidget::setTabOrder(pushButtonBeepOpen, pushButtonSpeakerClose);
        QWidget::setTabOrder(pushButtonSpeakerClose, pushButtonScreenTest);
        QWidget::setTabOrder(pushButtonScreenTest, pushButtonCardClose);
        QWidget::setTabOrder(pushButtonCardClose, pushButtonCardOpen);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        pushButtonSpeakerOpen->setText(QApplication::translate("MainWindow", "\345\274\200", 0, QApplication::UnicodeUTF8));
        pushButtonScreenCalibration->setText(QApplication::translate("MainWindow", "\345\261\217\345\271\225\346\240\241\345\207\206", 0, QApplication::UnicodeUTF8));
        pushButtonScreenTest->setText(QApplication::translate("MainWindow", "\345\261\217\345\271\225\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        pushButtonSpeakerClose->setText(QApplication::translate("MainWindow", "\345\205\263", 0, QApplication::UnicodeUTF8));
        pushButtonBeepOpen->setText(QApplication::translate("MainWindow", "\345\274\200", 0, QApplication::UnicodeUTF8));
        pushButtonBeepClose->setText(QApplication::translate("MainWindow", "\345\205\263", 0, QApplication::UnicodeUTF8));
        pushButtonCardClose->setText(QApplication::translate("MainWindow", "\345\205\263", 0, QApplication::UnicodeUTF8));
        pushButtonCardOpen->setText(QApplication::translate("MainWindow", "\345\274\200", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "\345\226\207\345\217\255\346\265\213\350\257\225\357\274\232", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "\350\234\202\351\270\243\345\231\250\346\265\213\350\257\225\357\274\232", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "\345\210\267\345\215\241\346\265\213\350\257\225\357\274\232", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QApplication::translate("MainWindow", "HelloZhytek", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setText(QApplication::translate("MainWindow", "HelloZhytek", 0, QApplication::UnicodeUTF8));
        pushButtonEthTest->setText(QApplication::translate("MainWindow", "ETH\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        pushButtonWifiTest->setText(QApplication::translate("MainWindow", "Wifi\346\265\213\350\257\225", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "wifi\345\220\215\347\247\260\357\274\232", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "wifi\345\257\206\347\240\201\357\274\232", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "SD\345\215\241\357\274\232", 0, QApplication::UnicodeUTF8));
        labelSDCardStatus->setText(QApplication::translate("MainWindow", "\346\214\202\350\275\275\346\210\220\345\212\237", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
