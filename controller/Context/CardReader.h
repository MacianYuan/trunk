#ifndef CARDREADER_H
#define CARDREADER_H

#include <QtCore/QObject>


class CardReader : public QObject
{
    Q_OBJECT
public:
    explicit CardReader(QObject *parent = 0);
    void setGuardInterval(uint msecs);
    void setSerialEndian(QSysInfo::Endian endian);

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void cardDetected(uint serial);

public slots:
    void open();
    void close();

private slots:
    void seekCard();

private:
    int timerId {0};
    uint count {0};
    uint guardCount {50};
    uint serial {0};
    unsigned char serialBytes[5];
    QSysInfo::Endian serialEndian;
};

#endif // CARDREADER_H
