/****
 * testapp.cpp - implement testing for utilities.
 * 		@Author: Leo Nguyen. All right reserved
 * 		@Date: April, 21st 2016
 */

#include "Logger.h"
#include "utility.h"

int main(int argc, char *argv[]){
    
    char **files = NULL;
    int size = 0;
    dir_traverse(argv[1], &files, size);
    for(int i=0; i<size; i++){
        printf("file: %s\n", *(files + i));
    }
	return 0;
}
