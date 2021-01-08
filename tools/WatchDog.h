//
// Created by 何继胜 on 2017/7/4.
//

#ifndef OSLA_NEW_WATCHDOG_H
#define OSLA_NEW_WATCHDOG_H


#include <mutex>

class WatchDog {
public:
    static WatchDog* GetInstance();
    static void DestroyInstance();

private:
    WatchDog();
    ~WatchDog();
    void Init();
    void Destroy();

public:
    void Feed();

private:
    int fd {0};

    static WatchDog* m_Instance;
    static std::mutex m_Mutex;
};


#endif //OSLA_NEW_WATCHDOG_H
