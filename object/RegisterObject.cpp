#include <iostream>
#include "object/RegisterObject.h"
#include "object/BaseObject.h"

RegisterObject* RegisterObject::m_Instance;
std::mutex RegisterObject::m_Mutex;

RegisterObject::RegisterObject()
{
    m_CreateFun.clear();
    m_Object.clear();
}

RegisterObject::~RegisterObject()
{
    m_CreateFun.clear();
    m_Object.clear();
}

RegisterObject& RegisterObject::GetInstance()
{
    if (nullptr == m_Instance) {
        std::lock_guard<std::mutex> guard(m_Mutex);
        if (nullptr == m_Instance) {
            m_Instance = new RegisterObject();
        }
    }
    return *m_Instance;
}

void RegisterObject::DestoryInstance()
{
    if (nullptr != m_Instance) {
        std::lock_guard<std::mutex> guard(m_Mutex);
        if (nullptr != m_Instance) {
            delete m_Instance;
            m_Instance = nullptr;
        }
    }
}

bool RegisterObject::UnRegister(std::string name)
{
    auto iter = m_CreateFun.find(name);
    if (m_CreateFun.end() != iter) {
        m_CreateFun.erase(iter);
    }
    return true;
}

std::shared_ptr<BaseObject> RegisterObject::CreateObject(std::string name)
{
    std::shared_ptr<BaseObject> ptr;

    //创建BaseObject子类对象
    auto createObjectFun = [this](std::string& name) -> std::shared_ptr<BaseObject> {
        std::shared_ptr<BaseObject> p;
        auto iter = m_CreateFun.find(name);
        if (m_CreateFun.end() != iter) {
            auto fun = iter->second;
            p = fun(name);
        }
        return p;
    };

    //查找BaseObject子类对象是否存在，若不存在，则创建一个.存在，则返回这个对象
    std::lock_guard<std::mutex> guard(m_Mutex);
    auto existIter = m_Object.find(name);
    if (m_Object.end() == existIter) {
        ptr = createObjectFun(name);
    } else {
        auto& objectList = existIter->second;
        if (true != objectList.empty()) {
            ptr = objectList.front();
            objectList.pop_front();
        } else {
            ptr = createObjectFun(name);
        }
    }

    return ptr;
}

void RegisterObject::ReleaseObject(std::shared_ptr<BaseObject>& ptr)
{
    if (nullptr == ptr) {
        return;
    }

    std::lock_guard<std::mutex> guard(m_Mutex);

    ptr->reset();
    std::string& name = ptr->getClassName();

    auto existIter = m_Object.find(name);
    if (m_Object.end() == existIter) {
        std::list<std::shared_ptr<BaseObject>> objectList;
        objectList.push_back(ptr);
        m_Object.insert(std::make_pair(name, objectList));
    } else {
        auto& objectList = existIter->second;
        objectList.push_back(ptr);
    }
}

class SignalRegisterObjectPoolSwap {
public:
    SignalRegisterObjectPoolSwap() {
        RegisterObject::GetInstance();
    }
    ~SignalRegisterObjectPoolSwap() {
        RegisterObject::DestoryInstance();
    }
};
SignalRegisterObjectPoolSwap _SignalRegisterObjectPoolSwap;
