/****
 * PCMDecoder.h - Declaration of function to decode audio files in to PCM
 * 		@Author: Leo Nguyen. All right reserved
 * 		@Date: Oct, 08th 2015
 */

#ifndef CORE_AUDIO_PCM_DECODER_H_
#define CORE_AUDIO_PCM_DECODER_H_

#define MONO                    1
#define STEREO                  2
#define OUT_SAMPLE_FMT          AV_SAMPLE_FMT_S16
#define OUT_CODEC_NAME          "pcm_s16le"

#define SAMPLE_RATE_DEFAULT     16000
#define CHANNELS_DEFAULT        1
//
extern "C" {
#include "libswresample/swresample.h"
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
}

typedef struct tagAudioInfo{
    int duration;
    int64_t duration_base;
    int bit_rate;
    int sample_rate;
    int channels;
    char codec_name[32];
} AudioInfo;

typedef enum {
    D_SUCCESS = 0,
    D_FILE_OPEN_FAILED,
    D_NO_STREAM,
    D_NO_AUDIO_STREAM,
    D_CODEC_FIND_FAILED,
    D_CODEC_OPEN_FAILED,
    D_FRAME_ALLOC_FAILED,
    D_TIME_ERROR,
    D_RESAMPLER_INIT_FAILED,
    D_SEEK_FAILED
} DecodingResult;

class PCMDecoder {
public:
    PCMDecoder();
    ~PCMDecoder();
    PCMDecoder(char *filename);
    AudioInfo getAudioInfo();
    DecodingResult decodeFullPCM(int sp_rate, int channel, char **data, int &size);
    DecodingResult decodePCM(int sp_rate, int channel, char **data, int &size, int p_start = 0, int p_end = 100);
    DecodingResult decodePCM(int sp_rate, int channel, char **data, int &size, float start, float end);
    DecodingResult decodePCM(int sp_rate, int channel, char **data, int &size, int p_start, float duration);
private:
    void init();
    void ffmpeg_registration();
    //
    DecodingResult initResult;
    AudioInfo audioInfo;
    AVFormatContext *mAVFormatContext;
    AVCodecContext *mAVCodecContext;
    AVStream *mAVStream;
    AVFrame *frame;
    char *filename;
};

#endif // CORE_AUDIO_PCM_DECODER_H_
