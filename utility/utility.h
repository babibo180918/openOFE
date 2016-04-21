/****
 * utility.h - define utilities functions frequently used in common C/C++ program.
 * 		@Author: Leo Nguyen. All right reserved
 * 		@Date: Oct, 08th 2015
 */

#ifndef __UTILITY_H_
#define __UTILITY_H_

#if defined _WIN32
#define DIR_DELIMIT_CHAR				'\\'
#define DIR_DELIMIT_CHARS			"\\"
#else
#define DIR_DELIMIT_CHAR				'/'
#define DIR_DELIMIT_CHARS			"/"
#endif
#ifndef MAX_PATH_LEN
#define MAX_PATH_LEN					4096
#endif
#ifndef MAX_FILE_LEN
#define MAX_FILE_LEN					255
#endif
#define IS_FILE						1
#define IS_DIRECTORY					2

// Directory
int 			is_file_directory(const char *pathname);
bool 			folder_exists(const char* path);
int 			mkdir_r(const char* path);
char 			*get_extension(const char *filePath);
char 			*del_extension(char *filePath);
void 			split_file_path(const char *filePath, char *dir, char *filename);
bool 			del_file(const char *filename);
bool 			del_directory(const char *path);
void			dir_traverse(const char *path, char ***files, int &size);

#endif // __UTILITY_H_
