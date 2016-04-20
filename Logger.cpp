/****
 * Logger.cpp - implement Logger class used to log messages.
 * 		@Author: Leo Nguyen. All right reserved
 * 		@Date: Oct, 08th 2015
 */
#include "Logger.h"
#include <stdlib.h>
#include <stdio.h>
#include "time.h"
#include <stdarg.h>
#include <string.h>
FILE *Logger::fpLog = NULL;

Logger::Logger(){}
Logger::~Logger(){
	if(fpLog){
		fclose(fpLog);
		fpLog = NULL;
	}
}

void Logger::createLog(const char* path){
	if(fpLog){
		fclose(fpLog);
		fpLog = NULL;
	}
	if(!path || strlen(path) <=0){
		printLog("ERROR", "Failed to create Log file.\n");
		return;
	}

	if(!folder_exists(path)){
		if(!mkdir_r(path)==0){
			printLog("ERROR", "Failed to create Log file.\n");
		}
	}

	time_t t = time(NULL);
	struct tm* _tm = localtime(&t);
	char* filename = (char*)malloc(strlen(path) + 25);
	sprintf(filename, "%s%c%4d_%02d%02d.log",
			path,
			DIR_DELIMIT_CHAR,
			_tm->tm_year + 1900,
			_tm->tm_mon + 1,
			_tm->tm_mday);
	fpLog = fopen(filename, "r+");
	if(fpLog == NULL){
		fpLog = fopen(filename, "w");
		if(fpLog)
			printf("New log file created: %s\n", filename);
	} else {
		fseek(fpLog, 0, SEEK_END);
		printf("Log file is openned: %s\n", filename);
	}
}

void Logger::o(const char* format, ...) {
	va_list args;
	char    string[MAX_MSGLEN + 1];
	va_start(args, format);
	vsprintf(string, format, args);
	va_end(args);
	printLog("", string);
}

void Logger::i(const char* format, ...) {
	va_list args;
	char    string[MAX_MSGLEN + 1];
	va_start(args, format);
	vsprintf(string, format, args);
	va_end(args);
	printLog("INFO", string);
}

void Logger::e(const char* format, ...) {
	va_list args;
	char    string[MAX_MSGLEN + 1];
	va_start(args, format);
	vsprintf(string, format, args);
	va_end(args);
	printLog("ERROR", string);
}

void Logger::d(const char* format, ...) {
	va_list args;
	char    string[MAX_MSGLEN + 1];
	va_start(args, format);
	vsprintf(string, format, args);
	va_end(args);
	printLog("DEBUG", string);
}

void Logger::printLog(const char* logo, const char* msg) {
	//if (fpLog==NULL) {
	//	//createLog(DEFAULT_LOG_DIR);
	//	return;
	//}
	char    string[MAX_MSGLEN + 1];
	time_t t = time(NULL);
	struct tm* _tm = localtime(&t);
	sprintf(string, "%04d%02d%02d_%02d%02d%02d: %5s %s",
			_tm->tm_year + 1900,
			_tm->tm_mon + 1,
			_tm->tm_mday,
			_tm->tm_hour,
			_tm->tm_min,
			_tm->tm_sec,
			logo, msg);
	printf(string);
	if (!fpLog) {
		createLog(DEFAULT_LOG_DIR);
	}

	fseek(fpLog, 0, SEEK_END);
	fputs(string, fpLog);
}

void Logger::destroy() {
	if (fpLog) {
		fclose(fpLog);
		fpLog = NULL;
	}
}
