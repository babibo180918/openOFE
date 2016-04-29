/****
 * TestApp.h - testing app's Base class definition.
 * 		@Author: Leo Nguyen. All right reserved
 * 		@Date: April, 21st 2016
 */

#ifndef CORE_TEST_APP_H_
#define CORE_TEST_APP_H_

#include "/Users/nhanndt/Works/C_SOURCE/openOFE/utility/utility.h"
#include <string>

class TestApp {
public:
    TestApp(char *input, char *output){
        strcpy(this->input, input);
        strcpy(this->output, output);
        if (is_file_directory(this->input) == IS_FILE) {
            this->inMode = IS_FILE;
        } else if (is_file_directory(this->input) == IS_DIRECTORY){
            this->inMode = IS_DIRECTORY;
        } else {
            this->inMode = 0;
        }
        if (is_file_directory(this->output) == IS_FILE) {
            this->outMode = IS_FILE;
        } else if (is_file_directory(this->output) == IS_DIRECTORY){
            this->outMode = IS_DIRECTORY;
        } else {
            this->outMode = 0;
        }
    }
    virtual ~TestApp(){}
    virtual void start() = 0;
    virtual void stop() = 0;
    
protected:
    char input[MAX_PATH_LEN];
    char output[MAX_PATH_LEN];
    int inMode;
    int outMode;
};

#endif // CORE_TEST_APP_H_