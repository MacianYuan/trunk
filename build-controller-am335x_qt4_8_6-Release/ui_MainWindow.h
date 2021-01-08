/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
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
#include <QtGui/QLCDNumber>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QLabel *currentUserLabel;
    QLabel *instrumentNameLabel;
    QLCDNumber *timerLCD;
    QLabel *datetimeLabel;
    QLabel *timerLabel;
    QLabel *currentStateLabel;
    QLabel *adminInfoLabel;
    QLabel *currentBookingCountDownLabel;
    QLabel *currentBookingTimeLabel;
    QLabel *currentBookingUserLabel;
    QLabel *networkStateLabel;
    QLabel *currentUserInfoLabel;
    QLabel *bookingLabel;
    QToolButton *moreBookingsButton;
    QLabel *bgImage;
    QLabel *statusbarLabel;
    QLabel *warningLabel;
    QLabel *label;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(640, 480);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/controller.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setStyleSheet(QString::fromUtf8("font-size: 20px;"));
        currentUserLabel = new QLabel(MainWindow);
        currentUserLabel->setObjectName(QString::fromUtf8("currentUserLabel"));
        currentUserLabel->setGeometry(QRect(430, 120, 131, 31));
        currentUserLabel->setStyleSheet(QString::fromUtf8("color: rgb(200, 200, 200);"));
        currentUserLabel->setAlignment(Qt::AlignCenter);
        instrumentNameLabel = new QLabel(MainWindow);
        instrumentNameLabel->setObjectName(QString::fromUtf8("instrumentNameLabel"));
        instrumentNameLabel->setGeometry(QRect(40, 110, 301, 71));
        instrumentNameLabel->setAutoFillBackground(false);
        instrumentNameLabel->setStyleSheet(QString::fromUtf8("color: rgb(0, 85, 255);\n"
"font-size: 26px;"));
        instrumentNameLabel->setAlignment(Qt::AlignCenter);
        instrumentNameLabel->setWordWrap(true);
        timerLCD = new QLCDNumber(MainWindow);
        timerLCD->setObjectName(QString::fromUtf8("timerLCD"));
        timerLCD->setGeometry(QRect(360, 260, 271, 51));
        timerLCD->setStyleSheet(QString::fromUtf8("color: rgb(200, 200, 200);"));
        timerLCD->setFrameShape(QFrame::NoFrame);
        timerLCD->setFrameShadow(QFrame::Plain);
        timerLCD->setDigitCount(10);
        timerLCD->setSegmentStyle(QLCDNumber::Flat);
        datetimeLabel = new QLabel(MainWindow);
        datetimeLabel->setObjectName(QString::fromUtf8("datetimeLabel"));
        datetimeLabel->setGeometry(QRect(339, 440, 261, 31));
        datetimeLabel->setStyleSheet(QString::fromUtf8("font-size: 18px;"));
        datetimeLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timerLabel = new QLabel(MainWindow);
        timerLabel->setObjectName(QString::fromUtf8("timerLabel"));
        timerLabel->setGeometry(QRect(460, 230, 81, 31));
        timerLabel->setStyleSheet(QString::fromUtf8("color: rgb(200, 200, 200);"));
        timerLabel->setAlignment(Qt::AlignCenter);
        currentStateLabel = new QLabel(MainWindow);
        currentStateLabel->setObjectName(QString::fromUtf8("currentStateLabel"));
        currentStateLabel->setGeometry(QRect(40, 190, 301, 31));
        currentStateLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);\n"
"font-size: 26px;"));
        currentStateLabel->setAlignment(Qt::AlignCenter);
        adminInfoLabel = new QLabel(MainWindow);
        adminInfoLabel->setObjectName(QString::fromUtf8("adminInfoLabel"));
        adminInfoLabel->setGeometry(QRect(40, 240, 301, 51));
        adminInfoLabel->setAlignment(Qt::AlignCenter);
        currentBookingCountDownLabel = new QLabel(MainWindow);
        currentBookingCountDownLabel->setObjectName(QString::fromUtf8("currentBookingCountDownLabel"));
        currentBookingCountDownLabel->setGeometry(QRect(150, 350, 341, 21));
        currentBookingCountDownLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        currentBookingTimeLabel = new QLabel(MainWindow);
        currentBookingTimeLabel->setObjectName(QString::fromUtf8("currentBookingTimeLabel"));
        currentBookingTimeLabel->setGeometry(QRect(150, 376, 341, 20));
        currentBookingUserLabel = new QLabel(MainWindow);
        currentBookingUserLabel->setObjectName(QString::fromUtf8("currentBookingUserLabel"));
        currentBookingUserLabel->setGeometry(QRect(150, 400, 341, 21));
        networkStateLabel = new QLabel(MainWindow);
        networkStateLabel->setObjectName(QString::fromUtf8("networkStateLabel"));
        networkStateLabel->setGeometry(QRect(600, 440, 31, 31));
        networkStateLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/network-offline.png")));
        networkStateLabel->setScaledContents(false);
        networkStateLabel->setAlignment(Qt::AlignCenter);
        currentUserInfoLabel = new QLabel(MainWindow);
        currentUserInfoLabel->setObjectName(QString::fromUtf8("currentUserInfoLabel"));
        currentUserInfoLabel->setGeometry(QRect(359, 160, 271, 61));
        currentUserInfoLabel->setStyleSheet(QString::fromUtf8("color: rgb(200, 200, 200);\n"
"font-size: 26px;"));
        currentUserInfoLabel->setAlignment(Qt::AlignCenter);
        bookingLabel = new QLabel(MainWindow);
        bookingLabel->setObjectName(QString::fromUtf8("bookingLabel"));
        bookingLabel->setGeometry(QRect(0, 340, 101, 81));
        bookingLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);\n"
""));
        bookingLabel->setAlignment(Qt::AlignCenter);
        bookingLabel->setWordWrap(true);
        moreBookingsButton = new QToolButton(MainWindow);
        moreBookingsButton->setObjectName(QString::fromUtf8("moreBookingsButton"));
        moreBookingsButton->setGeometry(QRect(550, 350, 81, 71));
        moreBookingsButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/btn_schedule.png"), QSize(), QIcon::Normal, QIcon::Off);
        moreBookingsButton->setIcon(icon1);
        moreBookingsButton->setIconSize(QSize(64, 64));
        moreBookingsButton->setAutoRaise(true);
        bgImage = new QLabel(MainWindow);
        bgImage->setObjectName(QString::fromUtf8("bgImage"));
        bgImage->setGeometry(QRect(0, 0, 640, 480));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(bgImage->sizePolicy().hasHeightForWidth());
        bgImage->setSizePolicy(sizePolicy);
        bgImage->setStyleSheet(QString::fromUtf8("background-image: url(:/images/bg.png);"));
        statusbarLabel = new QLabel(MainWindow);
        statusbarLabel->setObjectName(QString::fromUtf8("statusbarLabel"));
        statusbarLabel->setGeometry(QRect(10, 440, 411, 31));
        statusbarLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);\n"
"font-size: 12px;"));
        warningLabel = new QLabel(MainWindow);
        warningLabel->setObjectName(QString::fromUtf8("warningLabel"));
        warningLabel->setGeometry(QRect(400, 40, 121, 61));
        warningLabel->setLayoutDirection(Qt::LeftToRight);
        warningLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 0, 0);"));
        warningLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        warningLabel->setWordWrap(true);
        label = new QLabel(MainWindow);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 640, 90));
        label->setStyleSheet(QString::fromUtf8("font: 75 20pt \"YaHei Consolas Hybrid\";"));
        bgImage->raise();
        currentUserLabel->raise();
        instrumentNameLabel->raise();
        timerLCD->raise();
        datetimeLabel->raise();
        timerLabel->raise();
        currentStateLabel->raise();
        adminInfoLabel->raise();
        currentBookingCountDownLabel->raise();
        currentBookingTimeLabel->raise();
        currentBookingUserLabel->raise();
        networkStateLabel->raise();
        currentUserInfoLabel->raise();
        bookingLabel->raise();
        moreBookingsButton->raise();
        statusbarLabel->raise();
        warningLabel->raise();
        label->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\344\273\252\345\231\250\346\216\247\345\210\266\345\231\250", 0, QApplication::UnicodeUTF8));
        currentUserLabel->setText(QApplication::translate("MainWindow", "Active user", 0, QApplication::UnicodeUTF8));
        instrumentNameLabel->setText(QApplication::translate("MainWindow", "Device name", 0, QApplication::UnicodeUTF8));
        datetimeLabel->setText(QApplication::translate("MainWindow", "Data/Time", 0, QApplication::UnicodeUTF8));
        timerLabel->setText(QApplication::translate("MainWindow", "Time", 0, QApplication::UnicodeUTF8));
        currentStateLabel->setText(QApplication::translate("MainWindow", "Device status", 0, QApplication::UnicodeUTF8));
        adminInfoLabel->setText(QApplication::translate("MainWindow", "Adminstrator\n"
"Information", 0, QApplication::UnicodeUTF8));
        currentBookingCountDownLabel->setText(QString());
        currentBookingTimeLabel->setText(QString());
        currentBookingUserLabel->setText(QString());
        networkStateLabel->setText(QString());
        currentUserInfoLabel->setText(QApplication::translate("MainWindow", "None", 0, QApplication::UnicodeUTF8));
        bookingLabel->setText(QApplication::translate("MainWindow", "The recent Booking", 0, QApplication::UnicodeUTF8));
        moreBookingsButton->setText(QApplication::translate("MainWindow", "...", 0, QApplication::UnicodeUTF8));
        bgImage->setText(QString());
        statusbarLabel->setText(QApplication::translate("MainWindow", "If you have any questions,please contact the instrument administrator.", 0, QApplication::UnicodeUTF8));
        warningLabel->setText(QApplication::translate("MainWindow", "Temp/Humi", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
