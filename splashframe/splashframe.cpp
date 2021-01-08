#include <QtGui/QMovie>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>

#include "splashframe/splashframe.h"

SplashFrame::SplashFrame(QWidget *parent) : QFrame(parent) {
    setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Base);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    setWindowModality(Qt::WindowModal);

    icon = new QLabel;
    text = new QLabel;
    movie = new QMovie(":/images/wait.gif");
    icon->setMovie(movie);
    movie->start();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(icon);
    layout->addWidget(text);
    setLayout(layout);
}

void SplashFrame::showMessage(const QString &message) {
    text->setText(message);
    if (isHidden()) {
        show();
    }
}

void SplashFrame::resizeEvent(QResizeEvent *) {
    move((parentWidget()->width() - frameRect().width()) / 2, (parentWidget()->height() - frameRect().height()) / 2);
}
