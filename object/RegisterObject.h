
#ifndef REGISTEROBJECTPOOL_H
#define REGISTEROBJECTPOOL_H

#include <memory>
#include <string>
#include <mutex>
#include <map>
#include <list>

#include "object/NonCopyAble.h"
#include "object/BaseObject.h"

using CreateObjectFun = std::function<std::shared_ptr<BaseObject>(std::string)>;

template<class T>
class CreateInstance 
{
public:
    static std::shared_ptr<BaseObject> create(std::string name) {
        return std::make_shared<T>(name);
    }
};

class RegisterObject : public NonCopyAble
{
public:
    static RegisterObject& GetInstance();
    static void DestoryInstance();

    template<class T>
    bool Register(std::string name) {
        CreateObjectFun fun = &CreateInstance<T>::create;
        m_CreateFun.insert(std::make_pair(name, fun));
        return true;
    }

    bool UnRegister(std::string name);
    std::shared_ptr<BaseObject> CreateObject(std::string name);
    void ReleaseObject(std::shared_ptr<BaseObject>& ptr);

private:
    RegisterObject();
    ~RegisterObject();

private:
    std::map<std::string, CreateObjectFun> m_CreateFun;
    std::map<std::string, std::list<std::shared_ptr<BaseObject>>> m_Object;

    static RegisterObject* m_Instance;
    static std::mutex m_Mutex;
};


#endif //REGISTEROBJECTPOOL_H