
#ifndef NETCONNECTIONMANAGE_H
#define NETCONNECTIONMANAGE_H

#include <QtCore/QThread>
#include <mutex>

class NetConnectionManage : public QThread
{
    Q_OBJECT
public:
    explicit NetConnectionManage(QObject *parent = NULL);
    ~NetConnectionManage();

    void run();

    /// true : 查找wifi热点
    /// false: 停止查找wifi热点
    void WifiScan(bool status);

public:
    enum NET_STATUS {
        NET_STATUS_NONE, NET_STATUS_ONLINE, NET_STATUS_OFFLINE
    };

    enum NET_TYPE {
        NET_TYPE_NONE, NET_TYPE_WIFI, NET_TYPE_ETH
    };

private:
    NET_TYPE GetNetType();
    NET_STATUS GetWifiStatus();
    NET_STATUS GetEthStatus();
    void RunEthMode();
    void RunWlanMode();
    void RunScanWifiAP();
    void RunModeConvert();
    void selectBestAP();
private:
    std::mutex m_Mutex;
    bool m_ScanWifiAP;
    NET_TYPE m_NetType;
	int m_10MinCount;
	time_t m_10MinStart;
};

#endif //NETCONNECTIONMANAGE_H
