
#ifndef CASCONNECTION_H
#define CASCONNECTION_H

#include <QtCore/QObject>

class QTcpSocket;

class CASConnection : public QObject
{
    Q_OBJECT
public:
    explicit CASConnection(QObject* parent = NULL);
    ~CASConnection();

    void SendLogin(uint cardId);
    void SendLogout(uint cardId);
    void SendHeartBeat(bool inUse);
    
public slots:
    void CASConnect();
    void CASReconnect();

private:
    QTcpSocket* casConnection;
};

#endif //CASCONNECTION_H