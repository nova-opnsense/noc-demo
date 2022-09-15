/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <syslog.h>
#include <string.h>

#define LOG_VERSION "0.1.0"

typedef struct
{
    va_list ap;
    const char *fmt;
    const char *file;
    struct tm *time;
    void *udata;
    int line;
    const char *func;
    int level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

enum
{
    LOGGER_TRACE,
    LOGGER_DEBUG,
    LOGGER_INFO,
    LOGGER_WARN,
    LOGGER_ERROR,
    LOGGER_FATAL
};

#if LOG_USE_FILENAME
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else
#define __FILENAME__ (__FILE__)
#endif

#if LOG_USE_STDERR
#define log_trace(...) log_log(LOGGER_TRACE, __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define log_debug(...) log_log(LOGGER_DEBUG, __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define log_info(...) log_log(LOGGER_INFO, __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define log_warn(...) log_log(LOGGER_WARN, __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define log_error(...) log_log(LOGGER_ERROR, __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#define log_fatal(...) log_log(LOGGER_FATAL, __FILENAME__, __LINE__, __func__, __VA_ARGS__)
#else
#define log_debug(...) syslog(LOG_DEBUG, __VA_ARGS__)
#define log_info(...) syslog(LOG_INFO, __VA_ARGS__)
#define log_warn(...) syslog(LOG_WARNING, __VA_ARGS__)
#define log_error(...) syslog(LOG_ERR, __VA_ARGS__)
#define log_crit(...) syslog(LOG_CRIT, __VA_ARGS__)
#endif

// #define log_emerg(...) syslog(LOG_EMERG, __VA_ARGS__)
// #define log_alert(...) syslog(LOG_ALERT, __VA_ARGS__)
// #define log_notice(...) syslog(LOG_NOTICE, __VA_ARGS__)

const char *log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE *fp, int level);

void log_init(char *tag, int log_level);

void log_log(int level, const char *file, int line, const char *func, const char *fmt, ...);

#endif
