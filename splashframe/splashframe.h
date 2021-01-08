#ifndef SPLASHFRAME_H
#define SPLASHFRAME_H

#include <QtGui/QFrame>

class QLabel;
class QMovie;

class SplashFrame : public QFrame
{
    Q_OBJECT
public:
    explicit SplashFrame(QWidget *parent = 0);
    void showMessage(const QString &message);

protected:
    void resizeEvent(QResizeEvent *);

private:
    //QWidget *content;
    QMovie *movie;
    QLabel *icon;
    QLabel *text;
};

#endif // SPLASHFRAME_H
