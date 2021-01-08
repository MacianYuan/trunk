
#ifndef SMALLDEFINE_H
#define SMALLDEFINE_H

#include <cstdio>

#define ZY_ASSERT(condition, fmt, ...)  {\
    if(false == (condition)) { \
        char format[4086] = {0}; \
        sprintf(format, "[%s-%d]:%s\n", __FUNCTION__, __LINE__, fmt); \
        printf(format, ##__VA_ARGS__); \
        throw; \
    } \
}

#endif //SMALLDEFINE_H