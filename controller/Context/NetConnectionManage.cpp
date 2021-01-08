#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtCore>
#include <deviceinfo/tq2440.h>
#include <tools/log.h>
#include <tools/cscanapresults.h>
#include <tools/cwlanconfigfile.h>
#include <deviceinfo/version.h>

#include "controller/Context/NetConnectionManage.h"

/// 有办法了，让着变成一个网络管理工具

NetConnectionManage::NetConnectionManage(QObject *parent /*= NULL*/)
{
    m_ScanWifiAP = false;
    m_NetType = NET_TYPE_NONE;
	m_10MinCount = 0;
	m_10MinStart = time(NULL);
}

NetConnectionManage::~NetConnectionManage()
{

}

void NetConnectionManage::run()
{
    while (true) {
        sleep(3);
        if (true == m_ScanWifiAP) {
            RunScanWifiAP();
            continue;
        }

        RunModeConvert();

        if (true == GetNetType()) {
            RunWlanMode();
            sleep(30);
        } else {
            RunEthMode();
            sleep(30);
        }
        sleep(2);
    }
}

void NetConnectionManage::WifiScan(bool status)
{
    m_ScanWifiAP = status;
}

NetConnectionManage::NET_TYPE NetConnectionManage::GetNetType()
{
    NET_TYPE type = NET_TYPE_ETH;

    QFile ntcf(TQ2440_NETTYPE_CONF);
    if (ntcf.open(QIODevice::ReadOnly)) {
        QTextStream stream(&ntcf);

        QStringList items;
        while (!stream.atEnd()) {
            items.append(stream.readLine());
        }

        foreach(QString item, items) {
            QStringList list = item.split("=");
            if (list[0] == "NET" && list[1] == "0") {
                type = NET_TYPE_WIFI;
                break;
            }
        }
        ntcf.close();
    } else {
        DEBUG_LOG("Failed to open" << TQ2440_NETTYPE_CONF);
    }

    return type;
}

NetConnectionManage::NET_STATUS NetConnectionManage::GetWifiStatus()
{
    NET_STATUS status = NET_STATUS_OFFLINE;
    QProcess process;
    process.start("wlan_status");
    process.waitForFinished();
    int code = process.exitCode();
    if (code > 0) {
        status = NET_STATUS_ONLINE;
    }
    return status;
}

NetConnectionManage::NET_STATUS NetConnectionManage::GetEthStatus()
{
    NET_STATUS status = NET_STATUS_OFFLINE;
    QProcess process;
    process.start("eth_status");
    process.waitForFinished();
    int code = process.exitCode();
    if (code > 0) {
        status = NET_STATUS_ONLINE;
    }
    return status;
}

void NetConnectionManage::RunEthMode()
{
    if (NET_STATUS_ONLINE != GetEthStatus()) {
        QProcess::execute("eth_ip");  /// 需要研究udhcpc的使用方式
    } else {
        QProcess::execute("eth_check_ip");
    }
}

static int readLoadWifiMode = 100;
static int MaxLoadWifiMode = 100 * 1000;
static int MinLoadWifiMode = 100;

void NetConnectionManage::RunWlanMode()
{
    if (NET_STATUS_ONLINE != GetWifiStatus()) {
		DEBUG_LOG("wifi is off line");
		if (m_10MinCount >= readLoadWifiMode) {
			DEBUG_LOG("unload wifi mod");
			QProcess::execute("wlan_mod_unload");
			DEBUG_LOG("load wifi mod");
			QProcess::execute("wlan_mod_load");
            readLoadWifiMode = readLoadWifiMode * 10;
            if (readLoadWifiMode > MaxLoadWifiMode) {
                readLoadWifiMode = MaxLoadWifiMode;
            }
		}
		m_10MinCount += 1;

        RunScanWifiAP();
        selectBestAP();
        QProcess::execute("wlan_connect");
        QProcess::execute("wlan_ip");
    } else {
        readLoadWifiMode = MinLoadWifiMode;
        QProcess::execute("wlan_check_ip");
		m_10MinCount = 0;
		m_10MinStart = time(NULL);
    }
}

void NetConnectionManage::RunScanWifiAP()
{
    QProcess::execute("wlan_scanf");
}

void NetConnectionManage::RunModeConvert()
{
    NET_TYPE type = GetNetType();
    if (m_NetType != type) {
        QProcess::execute("net_clear");
        m_NetType = type;
        if (NET_TYPE_ETH == m_NetType) {
            QProcess::execute("eth_up");
        } else if (NET_TYPE_WIFI == m_NetType) {
            QProcess::execute("wlan_up");
        }
    }
}

void NetConnectionManage::selectBestAP()
{
    DEBUG_LOG("AP:run selectBestAP");
    CScanAPResults apResults;
    apResults.setResultFile(WLAN_SCAN_FILE);

    CWlanConfigFile wlanConfigFile;
    wlanConfigFile.readConfigFile(WLAN_CONFIG);

    QString bestBssid = apResults.getAPbssid(wlanConfigFile.getSsid());
    if (!bestBssid.isEmpty() && (bestBssid != wlanConfigFile.getBssid()))
    {
        DEBUG_LOG("AP:best bssid is:" << bestBssid.toStdString() << " level:" << apResults.getAPLevel(wlanConfigFile.getSsid()));
        wlanConfigFile.setBssid(bestBssid);
        wlanConfigFile.writeConfigFile(WLAN_CONFIG);
        QProcess::execute("sync");
    }
}
