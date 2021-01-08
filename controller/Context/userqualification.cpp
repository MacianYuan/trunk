#include <QtCore>
#include "userqualification.h"
#include <tools/IniParse.h>
#include "tools/log.h"

UserQualification::UserQualification()
{
    unauthorizedUserInWorkTime = false;
    unauthorizedUserNotInWorkTime = false;
    normalUserInWorkTime = true;
    normalUserNotInWorkTime = false;
    seniorUserInWorkTime = true;
    seniorUserNotInWorkTime = true;
}

UserQualification::~UserQualification()
{

}

void UserQualification::writeUserQualification(IniParse &osla)
{
    osla.Set("UserQualification.unauthorizedUserInWorkTime", unauthorizedUserInWorkTime);
    osla.Set("UserQualification.unauthorizedUserNotInWorkTime", unauthorizedUserNotInWorkTime);
    osla.Set("UserQualification.normalUserInWorkTime", normalUserInWorkTime);
    osla.Set("UserQualification.normalUserNotInWorkTime", normalUserNotInWorkTime);
    osla.Set("UserQualification.seniorUserInWorkTime", seniorUserInWorkTime);
    osla.Set("UserQualification.seniorUserNotInWorkTime", seniorUserNotInWorkTime);
    osla.SaveFile();

    DEBUG_LOG("userQualification.getNormalUserInWorkTime:" << getNormalUserInWorkTime());
    DEBUG_LOG("userQualification.getNormalUserNotInWorkTime:" << getNormalUserNotInWorkTime());
    DEBUG_LOG("userQualification.getUnauthorizedUserInWorkTime:" << getUnauthorizedUserInWorkTime());
    DEBUG_LOG("userQualification.getUnauthorizedUserNotInWorkTime:" << getUnauthorizedUserNotInWorkTime());
    DEBUG_LOG("userQualification.getSeniorUserInWorkTime:" << getSeniorUserInWorkTime());
    DEBUG_LOG("userQualification.getSeniorUserNotInWorkTime:" << getSeniorUserNotInWorkTime());
    DEBUG_LOG("userQualification wrote!");
}

bool UserQualification::readUserQualification(IniParse &osla)
{
    std::string str = osla.GetString("UserQualification.unauthorizedUserInWorkTime", "");
    if (!str.empty())
    {
        unauthorizedUserInWorkTime = osla.GetBool("UserQualification.unauthorizedUserInWorkTime", false);
        unauthorizedUserNotInWorkTime = osla.GetBool("UserQualification.unauthorizedUserNotInWorkTime", false);
        normalUserInWorkTime = osla.GetBool("UserQualification.normalUserInWorkTime", true);
        normalUserNotInWorkTime = osla.GetBool("UserQualification.normalUserNotInWorkTime", false);
        seniorUserInWorkTime = osla.GetBool("UserQualification.seniorUserInWorkTime", true);
        seniorUserNotInWorkTime = osla.GetBool("UserQualification.seniorUserNotInWorkTime", true);
    }
    else
    {
        bool looseMode = osla.GetBool("Instrument.LooseMode", true);
        if (looseMode)
        {   //宽松模式
            unauthorizedUserInWorkTime = false;
            unauthorizedUserNotInWorkTime = false;
            normalUserInWorkTime = true;
            normalUserNotInWorkTime = false;
            seniorUserInWorkTime = true;
            seniorUserNotInWorkTime = true;
        }
        else
        {   //严格模式，所有用户在任何时间都要预约
            unauthorizedUserInWorkTime = false;
            unauthorizedUserNotInWorkTime = false;
            normalUserInWorkTime = false;
            normalUserNotInWorkTime = false;
            seniorUserInWorkTime = false;
            seniorUserNotInWorkTime = false;
        }
    }


    return true;
}

bool UserQualification::getUnauthorizedUserInWorkTime()
{
    return unauthorizedUserInWorkTime;
}

bool UserQualification::getUnauthorizedUserNotInWorkTime()
{
    return unauthorizedUserNotInWorkTime;
}

bool UserQualification::getNormalUserInWorkTime()
{
    return normalUserInWorkTime;
}

bool UserQualification::getNormalUserNotInWorkTime()
{
    return normalUserNotInWorkTime;
}

bool UserQualification::getSeniorUserInWorkTime()
{
    return seniorUserInWorkTime;
}

bool UserQualification::getSeniorUserNotInWorkTime()
{
    return seniorUserNotInWorkTime;
}
