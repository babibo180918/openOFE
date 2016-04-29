/****
 * Audio_TestApp.cpp - Audio Test App implementation.
 * 		@Author: Leo Nguyen. All right reserved
 * 		@Date: April, 21st 2016
 */

#include "Audio_TestApp.h"
#include "utility/Logger.h"
#include "utility/utility.h"
#include "PCMDecoder.h"

#ifdef __cplusplus
extern "C" {
#include <unistd.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/param.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <time.h>
}
#endif

#define LOG_PATH "/Users/nhanndt/Log"

Audio_TestApp::Audio_TestApp(char *input, char *output) : TestApp(input, output), filenames(NULL), filesize(0){
}

Audio_TestApp::~Audio_TestApp(){
    if(filenames != NULL){
        for(int i=0; i<filesize; i++)
            free(filenames[i]);
        free(filenames);
    }
}

void Audio_TestApp::start(){
    if(!inMode){
        Logger::e("Input error!\n");
        return;
    }
    if(!folder_exists(output)){
        if(mkdir_r(output)<0){
            Logger::e("Failed to create output directory.\n");
            return;
        }
    }
    char dir[MAX_PATH_LEN], name[MAX_FILE_LEN], ext[10];
    dir_traverse(input, &filenames, filesize);
    for (int i=0; i<filesize; i++){
        PCMDecoder *mPCMDecoder = new PCMDecoder(filenames[i]);
        char *data = NULL;
        int size = 0;
        mPCMDecoder->decodeFullPCM(16000, 1, &data, size);
        if(data){
            split_file_path(filenames[i], dir, name);
            del_extension(name);
            sprintf(dir,"%s/%s.pcm", output, name);
            printf("out file: %s\n", dir);
            FILE *out = fopen(dir,"w");
            if(out){
                fwrite(data, 1, size, out);
                fclose(out);
            } else Logger::d("Failed to write output file: %s\n", dir);
            free(data);
        }
        delete mPCMDecoder;
    }
}

void Audio_TestApp::stop(){
}

int main(int argc, char *argv[]){
    Logger::createLog(LOG_PATH);
    char input[256]="\0", output[256] = "\0";
    // Reading arguments
    if (argc != 5){
        Logger::e("Wrong syntax!\n");
        return -1;
    }
    for(int i=0; i<argc; i++){
        if(!strcmp(argv[i], "-i")){
            strcpy(input, argv[i+1]);
        }
        if(!strcmp(argv[i], "-o")){
            strcpy(output, argv[i+1]);
        }
    }
    //
    if(!strcmp(input, "\0") || !strcmp(output, "\0")){
        Logger::e("Wrong syntax!\n");
        return -1;
    }
    Audio_TestApp *app = new Audio_TestApp(input, output);
    app->start();
    app->stop();
    delete app;
}
