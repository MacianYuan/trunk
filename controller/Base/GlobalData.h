
#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <mutex>
#include <map>
#include <QtCore/QObject>
#include <QtCore/QTranslator>
#include "sqlworker/sqlworker.h"
#include "controller/Base/SmallDefine.h"
#include <tools/IniParse.h>
#include <deviceinfo/version.h>


enum ControlDataType {
    SQL_CONTROL,
    OSLA_CONTROL,
    CLIENT_CONTROL
};

class GlobalData
{
public:
    static GlobalData* GetInstance();
    static void Destory();
    static QString currentLocale;
    //static QTranslator *appTranslator;
    QObject* GetControlData(ControlDataType type);

    static quint64  id;
    static int  user_id;
    static int group_id;
    static char * card_serial;
    static int card_type;
    static int action_type;
    static int action_result;
    static int create_time;
    static int instrument_id;

private:
    GlobalData();
    ~GlobalData();

private:
    static std::mutex m_Mutex;
    static GlobalData* m_Instance;
    std::map<ControlDataType, QObject*> m_GlobalObject;

};

#endif //GLOBALDATA_H
