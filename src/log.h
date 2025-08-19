/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef LOG_H
#define LOG_H

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

#define LOG_VERSION "0.4.0"

#define LOG_USE_COLOR

#if !defined (PATH_TYPE)
#define PATH_TYPE FILENAME
#endif

#if defined(win32)
	#define __file__ __FILE__
	#define __BASE_FILE__ __file__
#endif

#define FILENAME(__file__) \
  (strrchr(__file__, '/') ? strrchr(__file__, '/') + 1 : __file__)

#define RELPATH(__file__)                        \
  (strchr(strchr(__file__, '/') + 1, '/') ? strchr(strchr(__file__, '/') + 1, '/') + 1 : __file__)

typedef struct {
  va_list ap;
  const char *fmt;
  const char *file;
  struct tm *time;
  void *udata;
  int line;
  int level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);

enum {
	LOG_TRACE,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL,
	LOG_VK_VL,
	LOG_TODO,
	LOG_DMT_INFO,
    LOG_DMT_LEAK
};

#define log_trace(...)       log_log(LOG_TRACE, PATH_TYPE(__BASE_FILE__), __LINE__, __VA_ARGS__)
#define log_debug(...)       log_log(LOG_DEBUG, PATH_TYPE(__BASE_FILE__), __LINE__, __VA_ARGS__)
#define log_info(...)        log_log(LOG_INFO,  PATH_TYPE(__BASE_FILE__), __LINE__, __VA_ARGS__)
#define log_warn(...)        log_log(LOG_WARN,  PATH_TYPE(__BASE_FILE__), __LINE__, __VA_ARGS__)
#define log_error(...)       log_log(LOG_ERROR, PATH_TYPE(__BASE_FILE__), __LINE__, __VA_ARGS__)
#define log_fatal(...)       log_log(LOG_FATAL, PATH_TYPE(__BASE_FILE__), __LINE__, __VA_ARGS__)
#define log_vk_vl(...)       log_log(LOG_VK_VL, PATH_TYPE(__BASE_FILE__), __LINE__, __VA_ARGS__)
#define log_todo(...)        log_log(LOG_TODO,  PATH_TYPE(__BASE_FILE__), __LINE__, __VA_ARGS__)
#define log_dmt_info(...)    log_log(LOG_DMT_INFO,   PATH_TYPE(__BASE_FILE__), __LINE__, __VA_ARGS__)
#define log_dmt_leak(...)    log_log(LOG_DMT_INFO,   PATH_TYPE(__BASE_FILE__), __LINE__, __VA_ARGS__)

#ifdef DEBUG
    #define log_assert(condition, fmt, ...) \
        do { \
            if (!(condition)) { \
                log_fatal(fmt, "%s", ##__VA_ARGS__, #condition); \
                abort(); \
            } \
        } while(0);
#else
    #define log_assert(condition, fmt, ...) ((void)0)
#endif

const char* log_level_string(int level);
void log_set_lock(log_LockFn fn, void *udata);
void log_set_level(int level);
void log_set_quiet(bool enable);
int log_add_callback(log_LogFn fn, void *udata, int level);
int log_add_fp(FILE *fp, int level);

void log_log(int level, const char *file, int line, const char *fmt, ...);

#endif
