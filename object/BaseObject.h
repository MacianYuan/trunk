#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <string>
#include <memory>

class BaseObject : public std::enable_shared_from_this<BaseObject> {
public:
    BaseObject(std::string className = "Object");

    virtual ~BaseObject();

    void setContext(BaseObject *context);

    virtual bool createRequest(BaseObject *dataIn, BaseObject *dataOut) {
        dataIn = dataIn;
        dataOut = dataOut;
        return true;
    }

    virtual bool parseRequset(BaseObject *dataIn, BaseObject *dataOut) {
        dataIn = nullptr;
        dataOut = nullptr;
        return true;
    }

    virtual bool parseRespose(BaseObject *dataIn, BaseObject *dataOut) {
        dataIn = nullptr;
        dataOut = nullptr;
        return true;
    }

    virtual void reset() {}

    std::string &getClassName();

protected:
    std::string m_ClassName;
    BaseObject *m_Context;
};

#endif //BASEOBJECT_H
