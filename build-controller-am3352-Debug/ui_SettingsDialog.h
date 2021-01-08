/********************************************************************************
** Form generated from reading UI file 'SettingsDialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStackedWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QToolButton *instrumentButton;
    QStackedWidget *stackedWidget;
    QWidget *instrumentPage;
    QGroupBox *statusGroupBox;
    QWidget *layoutWidget;
    QVBoxLayout *statusLayout;
    QRadioButton *normalButton;
    QRadioButton *faultyButton;
    QRadioButton *inactiveButton;
    QPushButton *saveStatusButton;
    QPushButton *exportButton;
    QPushButton *unbindButton;
    QWidget *netchoicePage;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout_2;
    QRadioButton *wlanButton;
    QLabel *wlanSsidLabel;
    QComboBox *wlanPskcomboBox;
    QLabel *wlanPskLabel;
    QLineEdit *wlanPskEdit;
    QRadioButton *HideWireless_radioButton;
    QPushButton *AddWlan_button;
    QRadioButton *lanButton;
    QLabel *wlanPskLabel_4;
    QFrame *line_4;
    QPushButton *saveNetchoiceButton;
    QLabel *wlanPskLabel_5;
    QLabel *wlanPskLabel_2;
    QWidget *networkPage;
    QGroupBox *localGroupBox;
    QWidget *layoutWidget1;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *autoButton;
    QRadioButton *manualButton;
    QGridLayout *manualLayout;
    QLabel *ipLabel;
    QLineEdit *ipEdit;
    QLabel *netmaskLabel;
    QLineEdit *netmaskEdit;
    QLabel *gatewayLabel;
    QLineEdit *gatewayEdit;
    QGroupBox *serverGroupBox;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout;
    QLabel *serverHostLabel;
    QLineEdit *serverHostEdit;
    QLabel *serverPortLabel;
    QLineEdit *serverPortEdit;
    QGroupBox *ntpGroupBox;
    QWidget *layoutWidget3;
    QGridLayout *ntpLayout;
    QLabel *ntpLabel;
    QLineEdit *ntpEdit;
    QPushButton *saveNetworkButton;
    QWidget *systemPage;
    QFrame *line_2;
    QLabel *energyLabel;
    QLabel *voiceLabel;
    QFrame *line_1;
    QPushButton *saveSystemButton;
    QWidget *layoutWidget4;
    QHBoxLayout *voiceLayout;
    QCheckBox *voiceCheckBox;
    QSpacerItem *horizontalSpacer;
    QLabel *volumeLabel;
    QSlider *volumeSlider;
    QLabel *volumeValueLabel;
    QFrame *line_3;
    QLabel *advLabel;
    QWidget *layoutWidget5;
    QGridLayout *gridLayout_3;
    QLabel *autoCloseLabel;
    QComboBox *autoCloseComboBox;
    QLabel *screenDimLabel;
    QComboBox *screenDimComboBox;
    QLabel *screenOffLabel;
    QComboBox *screenOffComboBox;
    QCheckBox *casCheckBox;
    QWidget *layoutWidget6;
    QHBoxLayout *horizontalLayout;
    QLabel *casAddrLabel;
    QSpinBox *casAddrSpinBox;
    QWidget *layoutWidget_2;
    QHBoxLayout *voiceLayout_2;
    QLabel *volumeLabelSwapCardInterval;
    QSlider *swapCardIntervalSlider;
    QLabel *swapCardIntervalValueLabel;
    QCheckBox *noBookingCanLoginCheckBox;
    QWidget *timePage;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_3;
    QComboBox *yearCombox;
    QComboBox *monthCombox;
    QComboBox *dayCombox;
    QComboBox *hourCombox;
    QComboBox *minuteCombox;
    QPushButton *saveTimeButton;
    QLabel *voiceLabel_2;
    QFrame *line_5;
    QWidget *advancePage;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_2;
    QPushButton *restartButton;
    QPushButton *tsCalButton;
    QPushButton *upgradeButton;
    QPushButton *copyLogButton;
    QSpacerItem *verticalSpacer;
    QPushButton *formatCardButton;
    QComboBox *languageCombox;
    QToolButton *networkButton;
    QToolButton *systemButton;
    QToolButton *closeButton;
    QFrame *buttonLine;
    QToolButton *aboutButton;
    QToolButton *netchoiceButton;
    QToolButton *timeButton;
    QToolButton *advanceButton;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(640, 480);
        SettingsDialog->setMaximumSize(QSize(640, 480));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/controller.ico"), QSize(), QIcon::Normal, QIcon::Off);
        SettingsDialog->setWindowIcon(icon);
        SettingsDialog->setStyleSheet(QString::fromUtf8("font-size: 20px;"));
        instrumentButton = new QToolButton(SettingsDialog);
        instrumentButton->setObjectName(QString::fromUtf8("instrumentButton"));
        instrumentButton->setGeometry(QRect(10, 10, 91, 51));
        instrumentButton->setFocusPolicy(Qt::NoFocus);
        instrumentButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/btn_instrument.png"), QSize(), QIcon::Normal, QIcon::Off);
        instrumentButton->setIcon(icon1);
        instrumentButton->setIconSize(QSize(32, 32));
        instrumentButton->setPopupMode(QToolButton::InstantPopup);
        instrumentButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        stackedWidget = new QStackedWidget(SettingsDialog);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(110, 10, 521, 461));
        stackedWidget->setFrameShape(QFrame::Box);
        stackedWidget->setFrameShadow(QFrame::Sunken);
        instrumentPage = new QWidget();
        instrumentPage->setObjectName(QString::fromUtf8("instrumentPage"));
        statusGroupBox = new QGroupBox(instrumentPage);
        statusGroupBox->setObjectName(QString::fromUtf8("statusGroupBox"));
        statusGroupBox->setGeometry(QRect(30, 20, 461, 181));
        layoutWidget = new QWidget(statusGroupBox);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 40, 154, 131));
        statusLayout = new QVBoxLayout(layoutWidget);
        statusLayout->setSpacing(12);
        statusLayout->setObjectName(QString::fromUtf8("statusLayout"));
        statusLayout->setContentsMargins(0, 0, 0, 0);
        normalButton = new QRadioButton(layoutWidget);
        normalButton->setObjectName(QString::fromUtf8("normalButton"));
        normalButton->setFocusPolicy(Qt::NoFocus);

        statusLayout->addWidget(normalButton);

        faultyButton = new QRadioButton(layoutWidget);
        faultyButton->setObjectName(QString::fromUtf8("faultyButton"));
        faultyButton->setFocusPolicy(Qt::NoFocus);

        statusLayout->addWidget(faultyButton);

        inactiveButton = new QRadioButton(layoutWidget);
        inactiveButton->setObjectName(QString::fromUtf8("inactiveButton"));
        inactiveButton->setFocusPolicy(Qt::NoFocus);

        statusLayout->addWidget(inactiveButton);

        saveStatusButton = new QPushButton(statusGroupBox);
        saveStatusButton->setObjectName(QString::fromUtf8("saveStatusButton"));
        saveStatusButton->setGeometry(QRect(340, 120, 101, 51));
        saveStatusButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/btn_save.png"), QSize(), QIcon::Normal, QIcon::Off);
        saveStatusButton->setIcon(icon2);
        saveStatusButton->setIconSize(QSize(32, 32));
        exportButton = new QPushButton(instrumentPage);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));
        exportButton->setGeometry(QRect(280, 380, 220, 50));
        exportButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/btn_export.png"), QSize(), QIcon::Normal, QIcon::Off);
        exportButton->setIcon(icon3);
        exportButton->setIconSize(QSize(32, 32));
        unbindButton = new QPushButton(instrumentPage);
        unbindButton->setObjectName(QString::fromUtf8("unbindButton"));
        unbindButton->setGeometry(QRect(20, 380, 220, 50));
        unbindButton->setFocusPolicy(Qt::NoFocus);
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/btn_reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        unbindButton->setIcon(icon4);
        unbindButton->setIconSize(QSize(32, 32));
        stackedWidget->addWidget(instrumentPage);
        netchoicePage = new QWidget();
        netchoicePage->setObjectName(QString::fromUtf8("netchoicePage"));
        verticalLayoutWidget = new QWidget(netchoicePage);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(1, 1, 260, 469));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        wlanButton = new QRadioButton(verticalLayoutWidget);
        wlanButton->setObjectName(QString::fromUtf8("wlanButton"));
        wlanButton->setFocusPolicy(Qt::StrongFocus);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, wlanButton);

        wlanSsidLabel = new QLabel(verticalLayoutWidget);
        wlanSsidLabel->setObjectName(QString::fromUtf8("wlanSsidLabel"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, wlanSsidLabel);

        wlanPskcomboBox = new QComboBox(verticalLayoutWidget);
        wlanPskcomboBox->setObjectName(QString::fromUtf8("wlanPskcomboBox"));
        wlanPskcomboBox->setMinimumSize(QSize(250, 0));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, wlanPskcomboBox);

        wlanPskLabel = new QLabel(verticalLayoutWidget);
        wlanPskLabel->setObjectName(QString::fromUtf8("wlanPskLabel"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, wlanPskLabel);

        wlanPskEdit = new QLineEdit(verticalLayoutWidget);
        wlanPskEdit->setObjectName(QString::fromUtf8("wlanPskEdit"));

        formLayout_2->setWidget(4, QFormLayout::FieldRole, wlanPskEdit);

        HideWireless_radioButton = new QRadioButton(verticalLayoutWidget);
        HideWireless_radioButton->setObjectName(QString::fromUtf8("HideWireless_radioButton"));

        formLayout_2->setWidget(5, QFormLayout::FieldRole, HideWireless_radioButton);

        AddWlan_button = new QPushButton(verticalLayoutWidget);
        AddWlan_button->setObjectName(QString::fromUtf8("AddWlan_button"));

        formLayout_2->setWidget(6, QFormLayout::FieldRole, AddWlan_button);

        lanButton = new QRadioButton(verticalLayoutWidget);
        lanButton->setObjectName(QString::fromUtf8("lanButton"));
        lanButton->setFocusPolicy(Qt::StrongFocus);

        formLayout_2->setWidget(8, QFormLayout::FieldRole, lanButton);

        wlanPskLabel_4 = new QLabel(verticalLayoutWidget);
        wlanPskLabel_4->setObjectName(QString::fromUtf8("wlanPskLabel_4"));

        formLayout_2->setWidget(11, QFormLayout::FieldRole, wlanPskLabel_4);

        line_4 = new QFrame(verticalLayoutWidget);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        formLayout_2->setWidget(12, QFormLayout::FieldRole, line_4);

        saveNetchoiceButton = new QPushButton(verticalLayoutWidget);
        saveNetchoiceButton->setObjectName(QString::fromUtf8("saveNetchoiceButton"));
        saveNetchoiceButton->setEnabled(true);
        saveNetchoiceButton->setFocusPolicy(Qt::NoFocus);
        saveNetchoiceButton->setIcon(icon2);
        saveNetchoiceButton->setIconSize(QSize(32, 32));

        formLayout_2->setWidget(14, QFormLayout::FieldRole, saveNetchoiceButton);

        wlanPskLabel_5 = new QLabel(verticalLayoutWidget);
        wlanPskLabel_5->setObjectName(QString::fromUtf8("wlanPskLabel_5"));

        formLayout_2->setWidget(15, QFormLayout::FieldRole, wlanPskLabel_5);

        wlanPskLabel_2 = new QLabel(verticalLayoutWidget);
        wlanPskLabel_2->setObjectName(QString::fromUtf8("wlanPskLabel_2"));

        formLayout_2->setWidget(7, QFormLayout::FieldRole, wlanPskLabel_2);


        verticalLayout_2->addLayout(formLayout_2);

        stackedWidget->addWidget(netchoicePage);
        networkPage = new QWidget();
        networkPage->setObjectName(QString::fromUtf8("networkPage"));
        localGroupBox = new QGroupBox(networkPage);
        localGroupBox->setObjectName(QString::fromUtf8("localGroupBox"));
        localGroupBox->setGeometry(QRect(20, 20, 291, 201));
        layoutWidget1 = new QWidget(localGroupBox);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(6, 30, 271, 165));
        verticalLayout = new QVBoxLayout(layoutWidget1);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        autoButton = new QRadioButton(layoutWidget1);
        autoButton->setObjectName(QString::fromUtf8("autoButton"));
        autoButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_2->addWidget(autoButton);

        manualButton = new QRadioButton(layoutWidget1);
        manualButton->setObjectName(QString::fromUtf8("manualButton"));
        manualButton->setFocusPolicy(Qt::NoFocus);

        horizontalLayout_2->addWidget(manualButton);


        verticalLayout->addLayout(horizontalLayout_2);

        manualLayout = new QGridLayout();
        manualLayout->setObjectName(QString::fromUtf8("manualLayout"));
        ipLabel = new QLabel(layoutWidget1);
        ipLabel->setObjectName(QString::fromUtf8("ipLabel"));
        ipLabel->setMinimumSize(QSize(80, 0));

        manualLayout->addWidget(ipLabel, 0, 0, 1, 1);

        ipEdit = new QLineEdit(layoutWidget1);
        ipEdit->setObjectName(QString::fromUtf8("ipEdit"));
        ipEdit->setMinimumSize(QSize(151, 0));
        ipEdit->setMaximumSize(QSize(151, 16777215));
        ipEdit->setFocusPolicy(Qt::ClickFocus);

        manualLayout->addWidget(ipEdit, 0, 1, 1, 1);

        netmaskLabel = new QLabel(layoutWidget1);
        netmaskLabel->setObjectName(QString::fromUtf8("netmaskLabel"));
        netmaskLabel->setMinimumSize(QSize(80, 0));

        manualLayout->addWidget(netmaskLabel, 1, 0, 1, 1);

        netmaskEdit = new QLineEdit(layoutWidget1);
        netmaskEdit->setObjectName(QString::fromUtf8("netmaskEdit"));
        netmaskEdit->setMinimumSize(QSize(151, 0));
        netmaskEdit->setMaximumSize(QSize(151, 16777215));
        netmaskEdit->setFocusPolicy(Qt::ClickFocus);
        netmaskEdit->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        manualLayout->addWidget(netmaskEdit, 1, 1, 1, 1);

        gatewayLabel = new QLabel(layoutWidget1);
        gatewayLabel->setObjectName(QString::fromUtf8("gatewayLabel"));
        gatewayLabel->setMinimumSize(QSize(80, 0));

        manualLayout->addWidget(gatewayLabel, 2, 0, 1, 1);

        gatewayEdit = new QLineEdit(layoutWidget1);
        gatewayEdit->setObjectName(QString::fromUtf8("gatewayEdit"));
        gatewayEdit->setMinimumSize(QSize(151, 0));
        gatewayEdit->setMaximumSize(QSize(151, 16777215));
        gatewayEdit->setFocusPolicy(Qt::ClickFocus);

        manualLayout->addWidget(gatewayEdit, 2, 1, 1, 1);


        verticalLayout->addLayout(manualLayout);

        serverGroupBox = new QGroupBox(networkPage);
        serverGroupBox->setObjectName(QString::fromUtf8("serverGroupBox"));
        serverGroupBox->setGeometry(QRect(20, 230, 291, 121));
        layoutWidget2 = new QWidget(serverGroupBox);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 30, 271, 82));
        gridLayout = new QGridLayout(layoutWidget2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        serverHostLabel = new QLabel(layoutWidget2);
        serverHostLabel->setObjectName(QString::fromUtf8("serverHostLabel"));
        serverHostLabel->setMinimumSize(QSize(80, 0));

        gridLayout->addWidget(serverHostLabel, 0, 0, 1, 1);

        serverHostEdit = new QLineEdit(layoutWidget2);
        serverHostEdit->setObjectName(QString::fromUtf8("serverHostEdit"));
        serverHostEdit->setMinimumSize(QSize(151, 0));
        serverHostEdit->setMaximumSize(QSize(151, 16777215));
        serverHostEdit->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(serverHostEdit, 0, 1, 1, 1);

        serverPortLabel = new QLabel(layoutWidget2);
        serverPortLabel->setObjectName(QString::fromUtf8("serverPortLabel"));
        serverPortLabel->setMinimumSize(QSize(80, 0));

        gridLayout->addWidget(serverPortLabel, 1, 0, 1, 1);

        serverPortEdit = new QLineEdit(layoutWidget2);
        serverPortEdit->setObjectName(QString::fromUtf8("serverPortEdit"));
        serverPortEdit->setMinimumSize(QSize(151, 0));
        serverPortEdit->setMaximumSize(QSize(151, 16777215));
        serverPortEdit->setFocusPolicy(Qt::ClickFocus);

        gridLayout->addWidget(serverPortEdit, 1, 1, 1, 1);

        ntpGroupBox = new QGroupBox(networkPage);
        ntpGroupBox->setObjectName(QString::fromUtf8("ntpGroupBox"));
        ntpGroupBox->setGeometry(QRect(20, 360, 291, 81));
        layoutWidget3 = new QWidget(ntpGroupBox);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(10, 30, 271, 41));
        ntpLayout = new QGridLayout(layoutWidget3);
        ntpLayout->setObjectName(QString::fromUtf8("ntpLayout"));
        ntpLayout->setContentsMargins(0, 0, 0, 0);
        ntpLabel = new QLabel(layoutWidget3);
        ntpLabel->setObjectName(QString::fromUtf8("ntpLabel"));
        ntpLabel->setMinimumSize(QSize(80, 0));

        ntpLayout->addWidget(ntpLabel, 0, 0, 1, 1);

        ntpEdit = new QLineEdit(layoutWidget3);
        ntpEdit->setObjectName(QString::fromUtf8("ntpEdit"));
        ntpEdit->setMinimumSize(QSize(151, 0));
        ntpEdit->setMaximumSize(QSize(151, 16777215));
        ntpEdit->setFocusPolicy(Qt::ClickFocus);

        ntpLayout->addWidget(ntpEdit, 0, 1, 1, 1);

        saveNetworkButton = new QPushButton(networkPage);
        saveNetworkButton->setObjectName(QString::fromUtf8("saveNetworkButton"));
        saveNetworkButton->setGeometry(QRect(380, 390, 101, 51));
        saveNetworkButton->setFocusPolicy(Qt::NoFocus);
        saveNetworkButton->setIcon(icon2);
        saveNetworkButton->setIconSize(QSize(32, 32));
        stackedWidget->addWidget(networkPage);
        systemPage = new QWidget();
        systemPage->setObjectName(QString::fromUtf8("systemPage"));
        line_2 = new QFrame(systemPage);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(180, 100, 311, 21));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        energyLabel = new QLabel(systemPage);
        energyLabel->setObjectName(QString::fromUtf8("energyLabel"));
        energyLabel->setGeometry(QRect(30, 90, 151, 31));
        voiceLabel = new QLabel(systemPage);
        voiceLabel->setObjectName(QString::fromUtf8("voiceLabel"));
        voiceLabel->setGeometry(QRect(30, 20, 51, 20));
        line_1 = new QFrame(systemPage);
        line_1->setObjectName(QString::fromUtf8("line_1"));
        line_1->setGeometry(QRect(100, 20, 391, 20));
        line_1->setFrameShape(QFrame::HLine);
        line_1->setFrameShadow(QFrame::Sunken);
        saveSystemButton = new QPushButton(systemPage);
        saveSystemButton->setObjectName(QString::fromUtf8("saveSystemButton"));
        saveSystemButton->setGeometry(QRect(380, 370, 101, 51));
        saveSystemButton->setFocusPolicy(Qt::NoFocus);
        saveSystemButton->setIcon(icon2);
        saveSystemButton->setIconSize(QSize(32, 32));
        layoutWidget4 = new QWidget(systemPage);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(49, 50, 491, 35));
        voiceLayout = new QHBoxLayout(layoutWidget4);
        voiceLayout->setObjectName(QString::fromUtf8("voiceLayout"));
        voiceLayout->setContentsMargins(0, 0, 0, 0);
        voiceCheckBox = new QCheckBox(layoutWidget4);
        voiceCheckBox->setObjectName(QString::fromUtf8("voiceCheckBox"));
        voiceCheckBox->setFocusPolicy(Qt::NoFocus);
        voiceCheckBox->setChecked(true);

        voiceLayout->addWidget(voiceCheckBox);

        horizontalSpacer = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        voiceLayout->addItem(horizontalSpacer);

        volumeLabel = new QLabel(layoutWidget4);
        volumeLabel->setObjectName(QString::fromUtf8("volumeLabel"));

        voiceLayout->addWidget(volumeLabel);

        volumeSlider = new QSlider(layoutWidget4);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(volumeSlider->sizePolicy().hasHeightForWidth());
        volumeSlider->setSizePolicy(sizePolicy);
        volumeSlider->setMinimumSize(QSize(200, 0));
        volumeSlider->setFocusPolicy(Qt::NoFocus);
        volumeSlider->setMaximum(100);
        volumeSlider->setSingleStep(10);
        volumeSlider->setPageStep(20);
        volumeSlider->setValue(50);
        volumeSlider->setOrientation(Qt::Horizontal);
        volumeSlider->setInvertedAppearance(false);
        volumeSlider->setTickPosition(QSlider::NoTicks);

        voiceLayout->addWidget(volumeSlider);

        volumeValueLabel = new QLabel(layoutWidget4);
        volumeValueLabel->setObjectName(QString::fromUtf8("volumeValueLabel"));

        voiceLayout->addWidget(volumeValueLabel);

        line_3 = new QFrame(systemPage);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(130, 260, 361, 20));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        advLabel = new QLabel(systemPage);
        advLabel->setObjectName(QString::fromUtf8("advLabel"));
        advLabel->setGeometry(QRect(30, 260, 111, 21));
        layoutWidget5 = new QWidget(systemPage);
        layoutWidget5->setObjectName(QString::fromUtf8("layoutWidget5"));
        layoutWidget5->setGeometry(QRect(53, 132, 281, 122));
        gridLayout_3 = new QGridLayout(layoutWidget5);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        autoCloseLabel = new QLabel(layoutWidget5);
        autoCloseLabel->setObjectName(QString::fromUtf8("autoCloseLabel"));

        gridLayout_3->addWidget(autoCloseLabel, 0, 0, 1, 1);

        autoCloseComboBox = new QComboBox(layoutWidget5);
        autoCloseComboBox->setObjectName(QString::fromUtf8("autoCloseComboBox"));
        autoCloseComboBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(autoCloseComboBox, 0, 1, 1, 1);

        screenDimLabel = new QLabel(layoutWidget5);
        screenDimLabel->setObjectName(QString::fromUtf8("screenDimLabel"));

        gridLayout_3->addWidget(screenDimLabel, 1, 0, 1, 1);

        screenDimComboBox = new QComboBox(layoutWidget5);
        screenDimComboBox->setObjectName(QString::fromUtf8("screenDimComboBox"));
        screenDimComboBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(screenDimComboBox, 1, 1, 1, 1);

        screenOffLabel = new QLabel(layoutWidget5);
        screenOffLabel->setObjectName(QString::fromUtf8("screenOffLabel"));

        gridLayout_3->addWidget(screenOffLabel, 2, 0, 1, 1);

        screenOffComboBox = new QComboBox(layoutWidget5);
        screenOffComboBox->setObjectName(QString::fromUtf8("screenOffComboBox"));
        screenOffComboBox->setFocusPolicy(Qt::NoFocus);

        gridLayout_3->addWidget(screenOffComboBox, 2, 1, 1, 1);

        casCheckBox = new QCheckBox(systemPage);
        casCheckBox->setObjectName(QString::fromUtf8("casCheckBox"));
        casCheckBox->setGeometry(QRect(10, 430, 501, 20));
        casCheckBox->setFocusPolicy(Qt::NoFocus);
        casCheckBox->setChecked(true);
        layoutWidget6 = new QWidget(systemPage);
        layoutWidget6->setObjectName(QString::fromUtf8("layoutWidget6"));
        layoutWidget6->setGeometry(QRect(48, 380, 261, 39));
        horizontalLayout = new QHBoxLayout(layoutWidget6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        casAddrLabel = new QLabel(layoutWidget6);
        casAddrLabel->setObjectName(QString::fromUtf8("casAddrLabel"));

        horizontalLayout->addWidget(casAddrLabel);

        casAddrSpinBox = new QSpinBox(layoutWidget6);
        casAddrSpinBox->setObjectName(QString::fromUtf8("casAddrSpinBox"));
        casAddrSpinBox->setFocusPolicy(Qt::NoFocus);

        horizontalLayout->addWidget(casAddrSpinBox);

        layoutWidget_2 = new QWidget(systemPage);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(50, 290, 431, 35));
        voiceLayout_2 = new QHBoxLayout(layoutWidget_2);
        voiceLayout_2->setObjectName(QString::fromUtf8("voiceLayout_2"));
        voiceLayout_2->setContentsMargins(0, 0, 0, 0);
        volumeLabelSwapCardInterval = new QLabel(layoutWidget_2);
        volumeLabelSwapCardInterval->setObjectName(QString::fromUtf8("volumeLabelSwapCardInterval"));

        voiceLayout_2->addWidget(volumeLabelSwapCardInterval);

        swapCardIntervalSlider = new QSlider(layoutWidget_2);
        swapCardIntervalSlider->setObjectName(QString::fromUtf8("swapCardIntervalSlider"));
        sizePolicy.setHeightForWidth(swapCardIntervalSlider->sizePolicy().hasHeightForWidth());
        swapCardIntervalSlider->setSizePolicy(sizePolicy);
        swapCardIntervalSlider->setMinimumSize(QSize(200, 0));
        swapCardIntervalSlider->setFocusPolicy(Qt::NoFocus);
        swapCardIntervalSlider->setMaximum(20);
        swapCardIntervalSlider->setSingleStep(1);
        swapCardIntervalSlider->setPageStep(5);
        swapCardIntervalSlider->setValue(5);
        swapCardIntervalSlider->setOrientation(Qt::Horizontal);
        swapCardIntervalSlider->setInvertedAppearance(false);
        swapCardIntervalSlider->setTickPosition(QSlider::NoTicks);

        voiceLayout_2->addWidget(swapCardIntervalSlider);

        swapCardIntervalValueLabel = new QLabel(layoutWidget_2);
        swapCardIntervalValueLabel->setObjectName(QString::fromUtf8("swapCardIntervalValueLabel"));

        voiceLayout_2->addWidget(swapCardIntervalValueLabel);

        noBookingCanLoginCheckBox = new QCheckBox(systemPage);
        noBookingCanLoginCheckBox->setObjectName(QString::fromUtf8("noBookingCanLoginCheckBox"));
        noBookingCanLoginCheckBox->setGeometry(QRect(50, 330, 421, 32));
        noBookingCanLoginCheckBox->setFocusPolicy(Qt::NoFocus);
        noBookingCanLoginCheckBox->setChecked(true);
        stackedWidget->addWidget(systemPage);
        timePage = new QWidget();
        timePage->setObjectName(QString::fromUtf8("timePage"));
        verticalLayoutWidget_2 = new QWidget(timePage);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(110, 60, 311, 211));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        yearCombox = new QComboBox(verticalLayoutWidget_2);
        yearCombox->setObjectName(QString::fromUtf8("yearCombox"));

        verticalLayout_3->addWidget(yearCombox);

        monthCombox = new QComboBox(verticalLayoutWidget_2);
        monthCombox->setObjectName(QString::fromUtf8("monthCombox"));

        verticalLayout_3->addWidget(monthCombox);

        dayCombox = new QComboBox(verticalLayoutWidget_2);
        dayCombox->setObjectName(QString::fromUtf8("dayCombox"));

        verticalLayout_3->addWidget(dayCombox);

        hourCombox = new QComboBox(verticalLayoutWidget_2);
        hourCombox->setObjectName(QString::fromUtf8("hourCombox"));

        verticalLayout_3->addWidget(hourCombox);

        minuteCombox = new QComboBox(verticalLayoutWidget_2);
        minuteCombox->setObjectName(QString::fromUtf8("minuteCombox"));

        verticalLayout_3->addWidget(minuteCombox);

        saveTimeButton = new QPushButton(timePage);
        saveTimeButton->setObjectName(QString::fromUtf8("saveTimeButton"));
        saveTimeButton->setGeometry(QRect(380, 380, 101, 51));
        saveTimeButton->setFocusPolicy(Qt::NoFocus);
        saveTimeButton->setIcon(icon2);
        saveTimeButton->setIconSize(QSize(32, 32));
        voiceLabel_2 = new QLabel(timePage);
        voiceLabel_2->setObjectName(QString::fromUtf8("voiceLabel_2"));
        voiceLabel_2->setGeometry(QRect(20, 20, 81, 20));
        line_5 = new QFrame(timePage);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setGeometry(QRect(110, 20, 371, 20));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);
        stackedWidget->addWidget(timePage);
        advancePage = new QWidget();
        advancePage->setObjectName(QString::fromUtf8("advancePage"));
        groupBox = new QGroupBox(advancePage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 501, 431));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setHorizontalSpacing(30);
        restartButton = new QPushButton(groupBox);
        restartButton->setObjectName(QString::fromUtf8("restartButton"));

        gridLayout_2->addWidget(restartButton, 0, 1, 1, 1);

        tsCalButton = new QPushButton(groupBox);
        tsCalButton->setObjectName(QString::fromUtf8("tsCalButton"));

        gridLayout_2->addWidget(tsCalButton, 1, 0, 1, 1);

        upgradeButton = new QPushButton(groupBox);
        upgradeButton->setObjectName(QString::fromUtf8("upgradeButton"));

        gridLayout_2->addWidget(upgradeButton, 0, 0, 1, 1);

        copyLogButton = new QPushButton(groupBox);
        copyLogButton->setObjectName(QString::fromUtf8("copyLogButton"));

        gridLayout_2->addWidget(copyLogButton, 1, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 3, 0, 1, 1);

        formatCardButton = new QPushButton(groupBox);
        formatCardButton->setObjectName(QString::fromUtf8("formatCardButton"));

        gridLayout_2->addWidget(formatCardButton, 2, 0, 1, 1);

        languageCombox = new QComboBox(groupBox);
        languageCombox->setObjectName(QString::fromUtf8("languageCombox"));

        gridLayout_2->addWidget(languageCombox, 2, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 0, 0, 1, 1);

        stackedWidget->addWidget(advancePage);
        networkButton = new QToolButton(SettingsDialog);
        networkButton->setObjectName(QString::fromUtf8("networkButton"));
        networkButton->setGeometry(QRect(10, 110, 91, 51));
        networkButton->setFocusPolicy(Qt::NoFocus);
        networkButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/btn_network.png"), QSize(), QIcon::Normal, QIcon::Off);
        networkButton->setIcon(icon5);
        networkButton->setIconSize(QSize(32, 32));
        networkButton->setPopupMode(QToolButton::InstantPopup);
        networkButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        systemButton = new QToolButton(SettingsDialog);
        systemButton->setObjectName(QString::fromUtf8("systemButton"));
        systemButton->setGeometry(QRect(10, 160, 91, 51));
        systemButton->setFocusPolicy(Qt::NoFocus);
        systemButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/btn_system.png"), QSize(), QIcon::Normal, QIcon::Off);
        systemButton->setIcon(icon6);
        systemButton->setIconSize(QSize(32, 32));
        systemButton->setPopupMode(QToolButton::InstantPopup);
        systemButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        closeButton = new QToolButton(SettingsDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setGeometry(QRect(5, 420, 101, 51));
        closeButton->setFocusPolicy(Qt::NoFocus);
        closeButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/btn_close.png"), QSize(), QIcon::Normal, QIcon::Off);
        closeButton->setIcon(icon7);
        closeButton->setIconSize(QSize(32, 32));
        closeButton->setPopupMode(QToolButton::InstantPopup);
        closeButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        buttonLine = new QFrame(SettingsDialog);
        buttonLine->setObjectName(QString::fromUtf8("buttonLine"));
        buttonLine->setGeometry(QRect(10, 350, 91, 21));
        buttonLine->setFrameShape(QFrame::HLine);
        buttonLine->setFrameShadow(QFrame::Sunken);
        aboutButton = new QToolButton(SettingsDialog);
        aboutButton->setObjectName(QString::fromUtf8("aboutButton"));
        aboutButton->setGeometry(QRect(5, 370, 101, 51));
        aboutButton->setFocusPolicy(Qt::NoFocus);
        aboutButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        aboutButton->setIcon(icon8);
        aboutButton->setIconSize(QSize(32, 32));
        aboutButton->setPopupMode(QToolButton::InstantPopup);
        aboutButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        netchoiceButton = new QToolButton(SettingsDialog);
        netchoiceButton->setObjectName(QString::fromUtf8("netchoiceButton"));
        netchoiceButton->setGeometry(QRect(10, 60, 91, 51));
        netchoiceButton->setFocusPolicy(Qt::NoFocus);
        netchoiceButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        netchoiceButton->setIcon(icon9);
        netchoiceButton->setIconSize(QSize(32, 32));
        netchoiceButton->setPopupMode(QToolButton::InstantPopup);
        netchoiceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        timeButton = new QToolButton(SettingsDialog);
        timeButton->setObjectName(QString::fromUtf8("timeButton"));
        timeButton->setGeometry(QRect(10, 210, 91, 51));
        timeButton->setFocusPolicy(Qt::NoFocus);
        timeButton->setStyleSheet(QString::fromUtf8(""));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/btn_time.png"), QSize(), QIcon::Normal, QIcon::Off);
        timeButton->setIcon(icon10);
        timeButton->setIconSize(QSize(32, 32));
        timeButton->setPopupMode(QToolButton::InstantPopup);
        timeButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        advanceButton = new QToolButton(SettingsDialog);
        advanceButton->setObjectName(QString::fromUtf8("advanceButton"));
        advanceButton->setGeometry(QRect(10, 260, 91, 51));
        advanceButton->setFocusPolicy(Qt::NoFocus);
        advanceButton->setStyleSheet(QString::fromUtf8(""));
        advanceButton->setIcon(icon8);
        advanceButton->setIconSize(QSize(32, 32));
        advanceButton->setPopupMode(QToolButton::InstantPopup);
        advanceButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        retranslateUi(SettingsDialog);

        stackedWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "\347\263\273\347\273\237\350\256\276\347\275\256\351\235\242\346\235\277", 0, QApplication::UnicodeUTF8));
        instrumentButton->setText(QApplication::translate("SettingsDialog", "Dev", 0, QApplication::UnicodeUTF8));
        statusGroupBox->setTitle(QApplication::translate("SettingsDialog", " Set device status", 0, QApplication::UnicodeUTF8));
        normalButton->setText(QApplication::translate("SettingsDialog", "Normal", 0, QApplication::UnicodeUTF8));
        faultyButton->setText(QApplication::translate("SettingsDialog", "Breakdown", 0, QApplication::UnicodeUTF8));
        inactiveButton->setText(QApplication::translate("SettingsDialog", "Unavaiable", 0, QApplication::UnicodeUTF8));
        saveStatusButton->setText(QApplication::translate("SettingsDialog", "Save", 0, QApplication::UnicodeUTF8));
        exportButton->setText(QApplication::translate("SettingsDialog", "Data export", 0, QApplication::UnicodeUTF8));
        unbindButton->setText(QApplication::translate("SettingsDialog", "Unbundling device", 0, QApplication::UnicodeUTF8));
        wlanButton->setText(QApplication::translate("SettingsDialog", "Wireless", 0, QApplication::UnicodeUTF8));
        wlanSsidLabel->setText(QApplication::translate("SettingsDialog", "SSID:", 0, QApplication::UnicodeUTF8));
        wlanPskLabel->setText(QApplication::translate("SettingsDialog", "Password:", 0, QApplication::UnicodeUTF8));
        HideWireless_radioButton->setText(QApplication::translate("SettingsDialog", "Hide Wireless", 0, QApplication::UnicodeUTF8));
        AddWlan_button->setText(QApplication::translate("SettingsDialog", "Add WLAN", 0, QApplication::UnicodeUTF8));
        lanButton->setText(QApplication::translate("SettingsDialog", "Wire network", 0, QApplication::UnicodeUTF8));
        wlanPskLabel_4->setText(QString());
        saveNetchoiceButton->setText(QApplication::translate("SettingsDialog", "Save", 0, QApplication::UnicodeUTF8));
        wlanPskLabel_5->setText(QString());
        wlanPskLabel_2->setText(QString());
        localGroupBox->setTitle(QApplication::translate("SettingsDialog", "Local", 0, QApplication::UnicodeUTF8));
        autoButton->setText(QApplication::translate("SettingsDialog", "Auto(DHCP)", 0, QApplication::UnicodeUTF8));
        manualButton->setText(QApplication::translate("SettingsDialog", "Manual", 0, QApplication::UnicodeUTF8));
        ipLabel->setText(QApplication::translate("SettingsDialog", "IP address:", 0, QApplication::UnicodeUTF8));
        ipEdit->setText(QString());
        netmaskLabel->setText(QApplication::translate("SettingsDialog", "Netmask:", 0, QApplication::UnicodeUTF8));
        netmaskEdit->setText(QString());
        gatewayLabel->setText(QApplication::translate("SettingsDialog", "Gateway:", 0, QApplication::UnicodeUTF8));
        serverGroupBox->setTitle(QApplication::translate("SettingsDialog", "Server", 0, QApplication::UnicodeUTF8));
        serverHostLabel->setText(QApplication::translate("SettingsDialog", "IP address:", 0, QApplication::UnicodeUTF8));
        serverPortLabel->setText(QApplication::translate("SettingsDialog", "Port:", 0, QApplication::UnicodeUTF8));
        ntpGroupBox->setTitle(QApplication::translate("SettingsDialog", "Server for time", 0, QApplication::UnicodeUTF8));
        ntpLabel->setText(QApplication::translate("SettingsDialog", "Host:", 0, QApplication::UnicodeUTF8));
        saveNetworkButton->setText(QApplication::translate("SettingsDialog", "Save", 0, QApplication::UnicodeUTF8));
        energyLabel->setText(QApplication::translate("SettingsDialog", "Safety&Energy", 0, QApplication::UnicodeUTF8));
        voiceLabel->setText(QApplication::translate("SettingsDialog", "Voice", 0, QApplication::UnicodeUTF8));
        saveSystemButton->setText(QApplication::translate("SettingsDialog", "Save", 0, QApplication::UnicodeUTF8));
        voiceCheckBox->setText(QApplication::translate("SettingsDialog", "Voice Prompt", 0, QApplication::UnicodeUTF8));
        volumeLabel->setText(QApplication::translate("SettingsDialog", "Volume", 0, QApplication::UnicodeUTF8));
        volumeValueLabel->setText(QApplication::translate("SettingsDialog", "50", 0, QApplication::UnicodeUTF8));
        advLabel->setText(QApplication::translate("SettingsDialog", "Advanced", 0, QApplication::UnicodeUTF8));
        autoCloseLabel->setText(QApplication::translate("SettingsDialog", "Close Dialog:", 0, QApplication::UnicodeUTF8));
        screenDimLabel->setText(QApplication::translate("SettingsDialog", "Screen Dimming:", 0, QApplication::UnicodeUTF8));
        screenOffLabel->setText(QApplication::translate("SettingsDialog", "Close Screen:", 0, QApplication::UnicodeUTF8));
        casCheckBox->setText(QApplication::translate("SettingsDialog", "Connect the EMP of Chinese Academy of Sciences", 0, QApplication::UnicodeUTF8));
        casAddrLabel->setText(QApplication::translate("SettingsDialog", "Terminal address:", 0, QApplication::UnicodeUTF8));
        volumeLabelSwapCardInterval->setText(QApplication::translate("SettingsDialog", "Swipe interval:", 0, QApplication::UnicodeUTF8));
        swapCardIntervalValueLabel->setText(QApplication::translate("SettingsDialog", "5", 0, QApplication::UnicodeUTF8));
        noBookingCanLoginCheckBox->setText(QApplication::translate("SettingsDialog", "Loose pattern", 0, QApplication::UnicodeUTF8));
        saveTimeButton->setText(QApplication::translate("SettingsDialog", "Save", 0, QApplication::UnicodeUTF8));
        voiceLabel_2->setText(QApplication::translate("SettingsDialog", "Timeset", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("SettingsDialog", "Advance", 0, QApplication::UnicodeUTF8));
        restartButton->setText(QApplication::translate("SettingsDialog", "Reboot", 0, QApplication::UnicodeUTF8));
        tsCalButton->setText(QApplication::translate("SettingsDialog", "Screen caliberation", 0, QApplication::UnicodeUTF8));
        upgradeButton->setText(QApplication::translate("SettingsDialog", "Upgrade", 0, QApplication::UnicodeUTF8));
        copyLogButton->setText(QApplication::translate("SettingsDialog", "Copy of log", 0, QApplication::UnicodeUTF8));
        formatCardButton->setText(QApplication::translate("SettingsDialog", "format SD card", 0, QApplication::UnicodeUTF8));
        networkButton->setText(QApplication::translate("SettingsDialog", "Net", 0, QApplication::UnicodeUTF8));
        systemButton->setText(QApplication::translate("SettingsDialog", "Sys", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("SettingsDialog", "Close", 0, QApplication::UnicodeUTF8));
        aboutButton->setText(QApplication::translate("SettingsDialog", "About", 0, QApplication::UnicodeUTF8));
        netchoiceButton->setText(QApplication::translate("SettingsDialog", "Net", 0, QApplication::UnicodeUTF8));
        timeButton->setText(QApplication::translate("SettingsDialog", "Time", 0, QApplication::UnicodeUTF8));
        advanceButton->setText(QApplication::translate("SettingsDialog", "Adv", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
