#include <QtGui/QWSInputMethod>
#include <QtCore/QSignalMapper>

#include "imframe/InputWidgetNum.h"
#include "ui_InputWidgetNum.h"

InputWidgetNum::InputWidgetNum(QWSInputMethod *im)
        : QWidget(0, Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),
          ui(new Ui::InputWidgetNum) {
    ui->setupUi(this);

    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(const QString &)), im, SLOT(sendContent(const QString &)));

    mapper->setMapping(ui->btn0, "0");
    connect(ui->btn0, SIGNAL(clicked()), mapper, SLOT(map()));

    mapper->setMapping(ui->btn1, "1");
    connect(ui->btn1, SIGNAL(clicked()), mapper, SLOT(map()));

    mapper->setMapping(ui->btn2, "2");
    connect(ui->btn2, SIGNAL(clicked()), mapper, SLOT(map()));

    mapper->setMapping(ui->btn3, "3");
    connect(ui->btn3, SIGNAL(clicked()), mapper, SLOT(map()));

    mapper->setMapping(ui->btn4, "4");
    connect(ui->btn4, SIGNAL(clicked()), mapper, SLOT(map()));

    mapper->setMapping(ui->btn5, "5");
    connect(ui->btn5, SIGNAL(clicked()), mapper, SLOT(map()));

    mapper->setMapping(ui->btn6, "6");
    connect(ui->btn6, SIGNAL(clicked()), mapper, SLOT(map()));

    mapper->setMapping(ui->btn7, "7");
    connect(ui->btn7, SIGNAL(clicked()), mapper, SLOT(map()));

    mapper->setMapping(ui->btn8, "8");
    connect(ui->btn8, SIGNAL(clicked()), mapper, SLOT(map()));

    mapper->setMapping(ui->btn9, "9");
    connect(ui->btn9, SIGNAL(clicked()), mapper, SLOT(map()));

    //point
    mapper->setMapping(ui->btnPoint, ".");
    connect(ui->btnPoint, SIGNAL(clicked()), mapper, SLOT(map()));

    //Backspace
    mapper->setMapping(ui->btnBackspace, "bs");
    connect(ui->btnBackspace, SIGNAL(clicked()), mapper, SLOT(map()));
}

InputWidgetNum::~InputWidgetNum() {
    delete ui;
} 
