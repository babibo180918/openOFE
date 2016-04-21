/****
 * utility.c - implement utilities functions frequently used in common C/C++ program.
 * 		@Author: Leo Nguyen. All right reserved
 * 		@Date: Oct, 08th 2015
 */

#include "utility.h"

#include <sstream>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#else
#include <dirent.h>
#include <unistd.h>
#include <sys/param.h>
#include <libgen.h>
#endif

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
		return false;
	if(strlen(path) == 1){
	} else if(path[strlen(path)-1] == '/'){
		path[strlen(path)-1]='\0';
	}
	lstat(path, &buf);
	if(S_ISREG(buf.st_mode)){
		return IS_FILE;
	} else if(S_ISDIR(buf.st_mode)){
		return IS_DIRECTORY;
	} else {
		return false;
	}
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

	while (std::getline(ss, level, DIR_DELIMIT_CHAR)) {
		current_level += level;
		current_level += DIR_DELIMIT_CHAR;

#if defined _WIN32
		if (!folder_exists(current_level.c_str()) && _mkdir(current_level.c_str()) != 0)
		return -1;

#else
		printf("current_level.c_str(): %s\n", current_level.c_str());
		if (!folder_exists(current_level.c_str())
				&& mkdir(current_level.c_str(), 0777) != 0)
			return -1;
#endif
	}
	return 0;
}

char *get_extension(const char *filePath){
	char path[MAX_PATH_LEN];
	char *ptr = NULL;
	strcpy(path, filePath);

	for(int i=strlen(path)-1; i>=0; i--){
		if(path[i]=='.'){
			return path +i+1;
		}
		if(filePath[i]==DIR_DELIMIT_CHAR)
			return NULL;
	}

//	ptr = strrchr(path, '.');
//	if(ptr==NULL){
//		return NULL;
//	} else {
//		return ptr+1;
//	}
}

char *del_extension(char *filePath){
	for(int i=strlen(filePath)-1; i>=0; i--){
		if(filePath[i]=='.'){
			filePath[i] = (char)NULL;
			return filePath;
		}
		if(filePath[i]==DIR_DELIMIT_CHAR)
			return filePath;
	}
}

void split_file_path(const char *filePath, char *dir, char *filename){
	char *realPath = realpath(filePath, NULL);
	if(realPath == NULL) return;
	if(is_file_directory(realPath) == IS_FILE){
		char *ptr = strrchr(realPath, DIR_DELIMIT_CHAR);
		strcpy(filename, ptr+1);
		*ptr = (char)NULL;
		strcpy(dir, realPath);
	} else {
		strcpy(dir, realPath);
	}
}

bool del_file(const char *filename){
	return true;
}

bool del_directory(const char *path){
	switch(is_file_directory(path)){
	case IS_FILE:
		if(!remove(path)) return true;
		else return false;
		break;
	case IS_DIRECTORY:
#ifdef WIN32_
#else
		DIR*			dp;
		struct dirent	*ep;
		char			buf[512];
		dp = opendir(path);

		while((ep=readdir(dp)) != NULL){
			if(!strcmp(ep->d_name,".") || !strcmp(ep->d_name, ".."))
				continue;
			sprintf(buf, "%s/%s", path, ep->d_name);
			switch(is_file_directory(buf)){
			case IS_FILE:
				if(remove(buf)) return false;
				break;
			case IS_DIRECTORY:
				del_directory(buf);
			}
		}
		closedir(dp);
		rmdir(path);
#endif
		break;
	default:
		return false;
		break;
	}
	return false;
}

void dir_traverse(const char *path, char ***files, int &size){
	char buf[MAX_PATH_LEN];
	switch(is_file_directory(path)){
	case IS_FILE:
		size++;
		*files = (char**)realloc(*files,size*sizeof(char*));
		*((*files)+size-1) = (char*)malloc(MAX_PATH_LEN);
		strcpy(*((*files)+size-1), path);
		break;
	case IS_DIRECTORY:
#ifdef WIN32_
#else
		DIR*			dp;
		struct dirent	*ep;
		dp = opendir(path);

		while((ep=readdir(dp)) != NULL){
			if(!strcmp(ep->d_name,".") || !strcmp(ep->d_name, ".."))
				continue;
			sprintf(buf, "%s/%s", path, ep->d_name);
			switch(is_file_directory(buf)){
			case IS_FILE:
				size++;
				*files = (char**)realloc(*files,size*sizeof(char*));
				(*files)[size-1] = (char*)malloc(MAX_PATH_LEN);
				strcpy((*files)[size-1], buf);
				break;
			case IS_DIRECTORY:
				dir_traverse(buf, files, size);
			}
		}
		closedir(dp);
#endif
		break;
	default:
		break;
	}
}
