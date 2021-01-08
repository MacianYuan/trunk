#include <QtCore/QSignalMapper>
#include <QtGui/QWSInputMethod>
#include <QtGui/QMessageBox>

#include "imframe/imframe.h"
#include "imframe/InputWidgetV.h"
#include "ui_InputWidgetV.h"

InputWidgetV::InputWidgetV(QWSInputMethod *im)
        : QWidget(0, Qt::Tool | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint),
          ui(new Ui::InputWidgetV) {
    ui->setupUi(this);
    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(const QString &)), im, SLOT(sendContent(const QString &)));

    QList<QPushButton *> btn = this->findChildren<QPushButton *>();
    foreach (QPushButton *b, btn) {
        if (b->objectName() == "bthBackspace") {
            mapper->setMapping(b, "bs1");
            connect(b, SIGNAL(clicked()), mapper, SLOT(map()));
        }else if (b->objectName() == "bthda") {
            mapper->setMapping(b, "bthda");
            connect(b, SIGNAL(clicked()), mapper, SLOT(map()));
        } else {
            if (InputType::getUppper() == InputTypeUL::IS_UPPER) {     
                mapper->setMapping(b, b->text().at(0).toUpper());
                b->setText(b->text().toUpper());           
                connect(b, SIGNAL(clicked()), mapper, SLOT(map()));
            } else if (InputType::getUppper() == InputTypeUL::IS_LOWER) {
                mapper->setMapping(b, b->text().at(0).toLower());
                b->setText(b->text().toLower());
                connect(b, SIGNAL(clicked()), mapper, SLOT(map()));
            }
            qApp->processEvents();
        }
    }
}

InputWidgetV::~InputWidgetV() {
    delete ui;
}
