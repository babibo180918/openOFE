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

class Logger {
public:
	Logger();
	~Logger();

	enum LogType {INFO, ERROR, DEBUG};
	static void createLog(const char* path);
	static void o(const char* format, ...);
	static void e(const char* format, ...);
	static void d(const char* format, ...);
	static void i(const char* format, ...);
	static void destroy();

private:
	static FILE *fpLog;
	static void printLog(const char* logo, const char* msg);
};

#endif // __LOGGER_H_
