/****
 * Logger.h - define Logger class used to log messages.
 * 		@Author: Leo Nguyen. All right reserved
 * 		@Date: Oct, 08th 2015
 */
#pragma once
#ifndef __LOGGER_H_
#define __LOGGER_H_
#include "utility.h"
#include <stdio.h>

#define LOG_FOOTPRINT 		APP_NAME
#define MAX_MSGLEN			4096
#ifdef _WIN32
#define DEFAULT_LOG_DIR 		"C:\\Users\\Administrator\\Documents\\Log"
#else
#define DEFAULT_LOG_DIR 		"/tmp/Log"
#endif

#define KNOR  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

class Logger {
public:
	Logger();
	~Logger();

	enum LogType {INFO, ERROR, DEBUG};
	static void createLog(const char* path);
	static void o(const char* format, ...);
    static void w(const char* format, ...);
	static void e(const char* format, ...);
	static void d(const char* format, ...);
	static void i(const char* format, ...);
	static void destroy();

private:
	static FILE *fpLog;
	static void printLog(const char* logo, const char* msg, const char* color);
};

#endif // __LOGGER_H_
