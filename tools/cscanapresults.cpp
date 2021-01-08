#include <QtAlgorithms>
#include "cscanapresults.h"

#define SSID_NO_LEVEL    -1000

bool levelLessThan(const SSID_INFO &s1, const SSID_INFO &s2)
{
    return s1.level > s2.level;
}

CScanAPResults::CScanAPResults()
{

}

bool CScanAPResults::setResultFile(QString fileName)
{
    clearAPList();

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    while (!file.atEnd())
    {
        QString line = file.readLine();
        line = line.trimmed();
        processFileLine(line);
        qSort(apList.begin(), apList.end(), levelLessThan);
    }

    return true;
}

int CScanAPResults::getAPLevel(QString ssid)
{
    foreach(SSID_INFO ssidInfo, apList)
    {
        if (ssidInfo.ssid == ssid)
        {
            return ssidInfo.level;
        }
    }

    return SSID_NO_LEVEL;
}

QString CScanAPResults::getAPbssid(QString ssid)
{
    foreach (SSID_INFO ssidInfo, apList) {
        if (ssidInfo.ssid == ssid)
        {
            return ssidInfo.bssid;
        }
    }

    return QString();
}

void CScanAPResults::clearAPList()
{
    apList.clear();
}

void CScanAPResults::processFileLine(QString line)
{
    QStringList strList;

    strList = line.split("\t");
    if (strList.count() == 5)
    {
        SSID_INFO ssidInfo;
        ssidInfo.bssid = strList[0];
        ssidInfo.freq = strList[1].toInt();
        ssidInfo.level = strList[2].toInt();
        ssidInfo.flags = strList[3];
        ssidInfo.ssid = strList[4];
        int level = getAPLevel(ssidInfo.ssid);
        if (level == SSID_NO_LEVEL)
        {
            apList.append(ssidInfo);
        }
        else if (level < ssidInfo.level)
        {
            for (int i = 0; i < apList.count(); i++)
            {
                if (apList[i].ssid == ssidInfo.ssid)
                {
                    apList.removeAt(i);
                    break;
                }
            }
            apList.append(ssidInfo);
        }
    }
}
