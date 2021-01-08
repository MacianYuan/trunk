#include <QtGui/QVBoxLayout>
#include <QtGui/QWSServer>
#include <QtGui/QLabel>
#include <tools/log.h>
#include "controller/UI/MessageBox.h"

static const int AutoCloseTimeOut = 5 * 1000;

MessageBox::MessageBox(QWidget *parent) :
        QFrame(parent) {
    setFrameStyle(QFrame::Box | QFrame::Sunken);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    label = new QLabel(this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->setMargin(20);
    layout->setSpacing(0);

    setLayout(layout);
    QFont font;
    font.setPixelSize(20);
    setFont(font);
}

void MessageBox::resizeEvent(QResizeEvent *) {
    move((parentWidget()->width() - frameRect().width()) / 2,
         (parentWidget()->height() - frameRect().height()) / 2);
}

void MessageBox::timerEvent(QTimerEvent *timerEvent) {
    if (timerEvent->timerId() == autoCloseTimerId) {
        killTimer(autoCloseTimerId);
        autoCloseTimerId = 0;
        close();
    }
}

void MessageBox::showText(const QString text) {
    QWSServer::screenSaverActivate(false);

    if (isVisible()) {
        hide();
        label->clear();
    }

    if (0 != autoCloseTimerId) {
        printf("call close event for message box\n");
        killTimer(autoCloseTimerId);
        autoCloseTimerId = 0;
        //close();
    }
    label->setText(text);

    show();
    autoCloseTimerId = startTimer(AutoCloseTimeOut);
}

void MessageBox::showText(const QString text, int interval) {
    QWSServer::screenSaverActivate(false);

    if (isVisible()) {
        hide();
        label->clear();
    }
    label->setText(text);
    show();

    if (interval == 0) {
        if (autoCloseTimerId) {
            killTimer(autoCloseTimerId);
            autoCloseTimerId = 0;
        }
    } else {
        autoCloseTimerId = startTimer(interval);
    }
}
