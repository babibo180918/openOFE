/****
* Audio_TestApp.h - Audio Test app class.
* 		@Author: Leo Nguyen. All right reserved
* 		@Date: April, 21st 2016
*/

#ifndef CORE_AUDIO_TEST_APP_H_
#define CORE_AUDIO_TEST_APP_H_

#include "TestApp.h"

class Audio_TestApp : TestApp{
public:
    Audio_TestApp(char *input, char *output);
    ~Audio_TestApp();
    void start();
    void stop();
private:
    char **filenames;
    int filesize;
};

#endif // CORE_AUDIO_TEST_APP_H_