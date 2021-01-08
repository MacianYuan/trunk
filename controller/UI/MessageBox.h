#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QtGui/QFrame>

class QLabel;

class MessageBox : public QFrame
{
    Q_OBJECT
public:
    explicit MessageBox(QWidget *parent = 0);
    void showText(const QString text);
    void showText(const QString text, int interval);

protected:
    void resizeEvent(QResizeEvent *);
    void timerEvent(QTimerEvent *timerEvent);

private:
    //QWidget *content;
    QLabel *label;
    int autoCloseTimerId;
};

#endif // MESSAGEBOX_H
