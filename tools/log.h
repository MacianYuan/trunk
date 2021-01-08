//
// Created by 何继胜 on 2017/4/12.
//

#ifndef OSLA_NEW_LOG_H
#define OSLA_NEW_LOG_H

#include <string>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;

#define DEBUG_LOG(p) LOG4CPLUS_DEBUG(LoggerControl::getInstance().getRoot(), __FUNCTION__ << " " << p)
#define INFO_LOG(p)  LOG4CPLUS_INFO(LoggerControl::getInstance().getRoot(), __FUNCTION__ << " " << p)
#define WARN_LOG(p)  LOG4CPLUS_WARN(LoggerControl::getInstance().getRoot(), __FUNCTION__ << " " << p)
#define ERROR_LOG(p) LOG4CPLUS_ERROR(LoggerControl::getInstance().getRoot(), __FUNCTION__ << " " << p)

class LoggerControl {
public:
    static LoggerControl &getInstance();
    bool init(std::string fileName, int size, int num);
    Logger getRoot();

private:
    LoggerControl() {};
    ~LoggerControl() {};
};


#endif //OSLA_NEW_LOG_H
