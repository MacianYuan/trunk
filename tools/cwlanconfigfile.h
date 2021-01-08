#ifndef CWLANCONFIGFILE_H
#define CWLANCONFIGFILE_H

#include <QObject>

class CWlanConfigFile
{
public:
    explicit CWlanConfigFile();
    bool isValid(void);
    void readConfigFile(QString fileName);
    void writeConfigFile(QString fileName);
    QString getSsid(void);
    QString getBssid(void);
    QString getPassword(void);
    void setSsid(QString aSsid);
    void setBssid(QString aBssid);
    void setPassword(QString aPassword);
    static QString removeLevleForSsid(QString ssid);
private:
    void clear();
    void processLine(QString line);

    bool validFlag;
    QString ssid;
    QString bssid;
    QString password;
};

#endif // CWLANCONFIGFILE_H
