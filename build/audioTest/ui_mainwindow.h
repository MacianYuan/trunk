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
#include <QtGui/QListView>
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
    QSlider *horizontalSliderA;
    QListView *listView;
    QSplitter *splitter;
    QPushButton *pushButtonBefore;
    QPushButton *pushButtonPlay;
    QPushButton *pushButtonPause;
    QPushButton *pushButtonAfter;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 480);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalSliderA = new QSlider(centralWidget);
        horizontalSliderA->setObjectName(QString::fromUtf8("horizontalSliderA"));
        horizontalSliderA->setGeometry(QRect(50, 320, 541, 22));
        horizontalSliderA->setOrientation(Qt::Horizontal);
        listView = new QListView(centralWidget);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(40, 40, 571, 251));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(60, 370, 531, 32));
        splitter->setOrientation(Qt::Horizontal);
        pushButtonBefore = new QPushButton(splitter);
        pushButtonBefore->setObjectName(QString::fromUtf8("pushButtonBefore"));
        splitter->addWidget(pushButtonBefore);
        pushButtonPlay = new QPushButton(splitter);
        pushButtonPlay->setObjectName(QString::fromUtf8("pushButtonPlay"));
        splitter->addWidget(pushButtonPlay);
        pushButtonPause = new QPushButton(splitter);
        pushButtonPause->setObjectName(QString::fromUtf8("pushButtonPause"));
        splitter->addWidget(pushButtonPause);
        pushButtonAfter = new QPushButton(splitter);
        pushButtonAfter->setObjectName(QString::fromUtf8("pushButtonAfter"));
        splitter->addWidget(pushButtonAfter);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 640, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        pushButtonBefore->setText(QApplication::translate("MainWindow", "\344\270\212\344\270\200\351\246\226", 0, QApplication::UnicodeUTF8));
        pushButtonPlay->setText(QApplication::translate("MainWindow", "\346\222\255  \346\224\276", 0, QApplication::UnicodeUTF8));
        pushButtonPause->setText(QApplication::translate("MainWindow", "\346\232\202  \345\201\234", 0, QApplication::UnicodeUTF8));
        pushButtonAfter->setText(QApplication::translate("MainWindow", "\344\270\213\344\270\200\351\246\226", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
