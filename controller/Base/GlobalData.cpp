
#include "controller/Base/GlobalData.h"

GlobalData *GlobalData::m_Instance = nullptr;
std::mutex GlobalData::m_Mutex;

GlobalData *GlobalData::GetInstance() {
    if (nullptr == m_Instance) {
        std::lock_guard<std::mutex> gurad(m_Mutex);
        if (nullptr == m_Instance) {
            m_Instance = new GlobalData;
        }
    }
    return m_Instance;
}

void GlobalData::Destory() {
    if (nullptr != m_Instance) {
        std::lock_guard<std::mutex> gurad(m_Mutex);
        if (nullptr != m_Instance) {
            delete m_Instance;
            m_Instance = nullptr;
        }
    }
}

QObject *GlobalData::GetControlData(ControlDataType type) {
    QObject *ptr = nullptr;
    auto iter = m_GlobalObject.find(type);
    if (m_GlobalObject.end() != iter) {
        ptr = iter->second;
    }
    return ptr;
}

GlobalData::GlobalData() {
//    SqlWorker* sqlWork = new SqlWorker();
//    ZY_ASSERT(NULL != sqlWork, "create new SqlMainWorker failed");
//    m_GlobalObject.insert(std::make_pair(SQL_CONTROL, sqlWork));
}

GlobalData::~GlobalData() {
    for (auto v : m_GlobalObject) {
        if (nullptr != v.second) {
            delete v.second;
        }
    }
}

IniParse osla(OSLA_CONFIG);
//QTranslator *GlobalData::appTranslator = NULL  ;
QString GlobalData::currentLocale = osla.GetString("Global.Language").c_str();

quint64 GlobalData::id = 0;
int  GlobalData::user_id= 0;
int  GlobalData::group_id= 0;
char* GlobalData::card_serial = NULL;
int GlobalData::card_type= 0;
int GlobalData::action_type= 0;
int GlobalData::action_result= 0;
int GlobalData::create_time= 0;
int GlobalData::instrument_id= 0;
