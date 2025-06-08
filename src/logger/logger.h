#if !defined(_LOGGER_H_)
#define _LOGGER_H_

#include <Arduino.h>

inline size_t LogInfof(const char *format, ...){
    #ifdef DEBUG_INFO
        va_list arg;
        va_list copy;
        va_start(arg, format);
        va_copy(copy, arg);
        size_t result = vprintf(format, arg);
        va_end(arg);
        return result;
    #endif
    return 0;
}

inline size_t LogWarnf(const char *format, ...){
    #ifdef DEBUG_WARN
        va_list arg;
        va_list copy;
        va_start(arg, format);
        va_copy(copy, arg);
        size_t result = vprintf(format, arg);
        va_end(arg);
        return result;
    #endif
    return 0;
}

inline size_t LogErrorf(const char *format, ...){
    #ifdef DEBUG_ERROR
        va_list arg;
        va_list copy;
        va_start(arg, format);
        va_copy(copy, arg);
        size_t result = vprintf(format, arg);
        va_end(arg);
        return result;
    #endif
    return 0;
}
#endif