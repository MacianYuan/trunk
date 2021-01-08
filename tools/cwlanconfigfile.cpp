#include <QStringList>
#include <QFile>
#include "cwlanconfigfile.h"


CWlanConfigFile::CWlanConfigFile() :
    validFlag(false)
{

}

bool CWlanConfigFile::isValid()
{
    return validFlag;
}

void CWlanConfigFile::readConfigFile(QString fileName)
{
    clear();

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    while (!file.atEnd())
    {
        QString line = file.readLine();
        processLine(line);
    }

    validFlag = true;
}

void CWlanConfigFile::writeConfigFile(QString fileName)
{
    if (!validFlag)
    {
        return;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        return;
    }

    file.write("ctrl_interface=/var/run/wpa_supplicant\n");
    file.write("network={\n");
    file.write("    key_mgmt=WPA-PSK\n");
    file.write("    scan_ssid=1\n");
    file.write("    proto=RSN\n");
    file.write("    ssid=\"");
    file.write(ssid.toAscii());
    file.write("\"\n");
    file.write("    psk=\"");
    file.write(password.toAscii());
    file.write("\"\n");
    if (!bssid.isEmpty())
    {
        file.write("    bssid=");
        file.write(bssid.toAscii());
        file.write("\n");
    }
    file.write("}\n");
}

QString CWlanConfigFile::getSsid()
{
    return ssid;
}

QString CWlanConfigFile::getBssid()
{
    return bssid;
}

QString CWlanConfigFile::getPassword()
{
    return password;
}

void CWlanConfigFile::setBssid(QString aBssid)
{
    validFlag = true;
    bssid = aBssid;
}

void CWlanConfigFile::setPassword(QString aPassword)
{
    validFlag = true;
    password = aPassword;
}

QString CWlanConfigFile::removeLevleForSsid(QString ssid)
{
    QStringList strList = ssid.split("]");
    if (strList.count() == 2)
    {
        return strList[1];
    }

    return ssid;
}

void CWlanConfigFile::setSsid(QString aSsid)
{
    validFlag = true;
    ssid = aSsid;
}

void CWlanConfigFile::clear()
{
    validFlag = false;
    ssid.clear();
    bssid.clear();
    password.clear();
}

void CWlanConfigFile::processLine(QString line)
{
    QStringList strList = line.split("=");
    if (strList.count() == 2)
    {
        if (strList[0].trimmed().toLower() == "ssid")
        {
            ssid = strList[1].trimmed().remove('"');
        }
        else if (strList[0].trimmed().toLower() == "psk")
        {
            password = strList[1].trimmed().remove('"');
        }
        else if (strList[0].trimmed().toLower() == "bssid")
        {
            bssid = strList[1].trimmed().remove('"');
        }
    }
}
