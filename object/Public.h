
#ifndef PUBLIC_H
#define PUBLIC_H

#include <memory>
#include <string>

#include "object/BaseObject.h"
#include "object/RegisterObject.h"


template<class T>
class RegisterOperation
{
public:
    RegisterOperation(std::string n) : name(n) {
        RegisterObject::GetInstance().Register<T>(name);
    }

    ~RegisterOperation() {
        RegisterObject::GetInstance().UnRegister(name);
    }

private:
    std::string name;
};


#define DynClassRegister(clsName) RegisterOperation<clsName> dynClassRegister##clsName(std::string(#clsName))

#endif