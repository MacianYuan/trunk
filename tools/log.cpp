//
// Created by 何继胜 on 2017/4/12.
//

#include "tools/log.h"

#include <log4cplus/ndc.h>
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>

using namespace log4cplus;
using namespace log4cplus::helpers;

bool LoggerControl::init(std::string fileName, int size, int num) {
    SharedAppenderPtr _append(new RollingFileAppender(fileName, size, num));
    _append->setName("QT");
    std::string pattern("%D{%m/%d/%y %H:%M:%S} : %m [%F:%L]%n");
    _append->setLayout(std::auto_ptr<Layout>(new PatternLayout(pattern)));
    Logger::getRoot().addAppender(_append);
    return true;
}

Logger LoggerControl::getRoot() {
    static Logger root = Logger::getRoot();
    return root;
}

LoggerControl& LoggerControl::getInstance() {
    static LoggerControl info;
    return info;
}
