/****
 * utility.c - implement utilities functions frequently used in common C/C++ program.
 * 		@Author: Leo Nguyen. All right reserved
 * 		@Date: Oct, 08th 2015
 */

#include "utility.h"

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <dirent.h>
#include <unistd.h>
#include <sys/param.h>
#include <libgen.h>
#endif
#include <sstream>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

int is_file_directory(const char* pathname) {
	struct stat buf;
	char path[MAX_PATH_LEN];
	strcpy(path, pathname);
	if (path[strlen(path) - 1] == DIR_DELIMIT_CHAR)
		path[strlen(path) - 1] = '\0';

#if defined _WIN32
	if (_access(pathname, 0) == -1) {
		return false;
	}
	_finddatai64_t c_file;
	intptr_t file;
	int result;
	file = _findfirsti64(path, &c_file);
	if (file == -1L)
		result = -1;
	else {
		if (c_file.attrib & _A_SUBDIR)
			result = IS_DIRECTORY;
		else
			result = IS_FILE;
	}
	_findclose(file);
	return result;

#else
	int flag = false;
	if (access(pathname, F_OK))
		return true;
	lstat(path, &buf);
#endif
}

bool folder_exists(const char* path) {
	struct stat st;
	stat(path, &st);
	return st.st_mode & S_IFDIR;
}

int mkdir_r(const char* path) {

	std::string current_level = "";
	std::string level;
	std::stringstream ss(path);
	while (std::getline(ss, level, '\\')) {
		current_level += level;
		current_level += '\\';
#if defined _WIN32
		if (!folder_exists(current_level.c_str()) && _mkdir(current_level.c_str()) != 0)
			return -1;
#else
		if (!folder_exists(current_level.c_str()) && mkdir(current_level.c_str(), 0777) != 0)
			return -1;
#endif
	}
	return 0;
}
