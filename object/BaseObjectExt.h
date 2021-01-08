//
// Created by 何继胜 on 2017/4/12.
//

#ifndef OSLA_NEW_BASEOBJECTEXT_H
#define OSLA_NEW_BASEOBJECTEXT_H

#include <functional>
#include <QtCore/QByteArray>
#include "object/BaseObject.h"

class BaseObjectExt : public BaseObject {
public:
    BaseObjectExt(std::string name = "BaseObjectExt");
    virtual ~BaseObjectExt();

    void createResponseCmd(bool status, BaseObject* org, BaseObject* in);

    template<class T, class F>
    bool ParseMessageType(const QByteArray& message, F fun) {
        bool status = false;
        do {
            T val;
            if (!val.ParseFromArray(message.data(), message.size())) {
                break;
            }
            status = fun(val);
        } while (false);

        return status;
    }

};

#endif //OSLA_NEW_BASEOBJECTEXT_H


