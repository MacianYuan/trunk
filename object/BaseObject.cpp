
#include "object/BaseObject.h"

BaseObject::BaseObject(std::string className /*= "Object"*/)
{
    m_ClassName = className;
}

BaseObject::~BaseObject()
{

}

std::string& BaseObject::getClassName()
{
    return m_ClassName;
}

void BaseObject::setContext(BaseObject *context) {
    this->m_Context = context;
}

