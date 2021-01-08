#ifndef USERQUALIFICATION_H
#define USERQUALIFICATION_H

class IniParse;

class UserQualification
{
public:
    UserQualification();
    ~UserQualification();
    void writeUserQualification(IniParse &osla);
    bool readUserQualification(IniParse &osla);
    bool getUnauthorizedUserInWorkTime();
    bool getUnauthorizedUserNotInWorkTime();
    bool getNormalUserInWorkTime();
    bool getNormalUserNotInWorkTime();
    bool getSeniorUserInWorkTime();
    bool getSeniorUserNotInWorkTime();
    inline void setUnauthorizedUserInWorkTime(bool flag)
    {
        unauthorizedUserInWorkTime =  flag;
    }
    inline void setUnauthorizedUserNotInWorkTime(bool flag)
    {
        unauthorizedUserNotInWorkTime =  flag;
    }
    inline void setNormalUserInWorkTime(bool flag)
    {
        normalUserInWorkTime = flag;
    }
    inline void setNormalUserNotInWorkTime(bool flag)
    {
        normalUserNotInWorkTime = flag;
    }
    inline void setSeniorUserInWorkTime(bool flag)
    {
        seniorUserInWorkTime = flag;
    }
    inline void setSeniorUserNotInWorkTime(bool flag)
    {
        seniorUserNotInWorkTime = flag;
    }
private:
    bool unauthorizedUserInWorkTime;
    bool unauthorizedUserNotInWorkTime;
    bool normalUserInWorkTime;
    bool normalUserNotInWorkTime;
    bool seniorUserInWorkTime;
    bool seniorUserNotInWorkTime;
};

#endif // USERQUALIFICATION_H
