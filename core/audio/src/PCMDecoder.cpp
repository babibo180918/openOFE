/****
 * PCMDecoder.cpp - PCM decoding class implementation.
 * 		@Author: Leo Nguyen. All right reserved
 * 		@Date: April, 21st 2016
 */

#include "PCMDecoder.h"
#include "utility/Logger.h"


PCMDecoder::PCMDecoder():filename(NULL){init();}

PCMDecoder::PCMDecoder(char *filename):filename(filename){init();}

PCMDecoder::~PCMDecoder(){
    if(frame != NULL)
        avcodec_free_frame(&frame);
        //av_free(frame);
    if(mAVCodecContext != NULL);{
        avcodec_close(mAVCodecContext);
        //free(mAVCodecContext);
    }
    if(mAVFormatContext != NULL){
        avformat_close_input(&mAVFormatContext);
        //free(mAVFormatContext);
    }
    
};

void PCMDecoder::ffmpeg_registration(){
    static bool isRegisterCalled = false;
    if (!isRegisterCalled) {
        av_register_all();
        isRegisterCalled = true;
    }
}

void PCMDecoder::init(){
    ffmpeg_registration();
    mAVFormatContext = NULL;
    if(avformat_open_input(&mAVFormatContext, filename, NULL, NULL) != 0){
        initResult = D_FILE_OPEN_FAILED;
        return;
    }
    if (avformat_find_stream_info(mAVFormatContext, NULL) < 0)
    {
        avformat_close_input(&mAVFormatContext);
        initResult = D_NO_STREAM;
        return;
    }
    
    mAVStream = NULL;
    int nh = av_find_best_stream(mAVFormatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (!nh) {
        mAVStream = mAVFormatContext->streams[nh];
    } else {
        avformat_close_input(&mAVFormatContext);
        initResult = D_NO_AUDIO_STREAM;
        return;
    }
    
    mAVCodecContext = mAVStream->codec;
    mAVCodecContext->codec = avcodec_find_decoder(mAVCodecContext->codec_id);
    if (mAVCodecContext->codec == NULL){
        avformat_close_input(&mAVFormatContext);
        initResult = D_CODEC_FIND_FAILED;
        return;
    } else if (avcodec_open2(mAVCodecContext, mAVCodecContext->codec, NULL) != 0){
        avformat_close_input(&mAVFormatContext);
        initResult = D_CODEC_OPEN_FAILED;
        return;
    }
    frame = avcodec_alloc_frame();
    if (!frame){
        initResult = D_FRAME_ALLOC_FAILED;
        return;
    }
    initResult = D_SUCCESS;
    audioInfo = getAudioInfo();
}

AudioInfo PCMDecoder::getAudioInfo(){
    AudioInfo info;
    if(initResult == D_SUCCESS){
        if(mAVCodecContext->bit_rate!=0)
            info.bit_rate = mAVCodecContext->bit_rate;
        else if(mAVFormatContext->bit_rate!=0)
            info.bit_rate = mAVFormatContext->bit_rate;
        info.sample_rate = mAVCodecContext->sample_rate;
        info.channels = mAVCodecContext->channels;
        strcpy(info.codec_name, mAVCodecContext->codec->name);
        //
        if(mAVStream->duration > 0){
            info.duration_base = mAVStream->duration;
            info.duration = ((float)info.duration_base*mAVStream->time_base.num)*1000/mAVStream->time_base.den;
        } else if(mAVFormatContext->duration > 0){
            info.duration = (float)mAVFormatContext->duration*1000/AV_TIME_BASE;
            info.duration_base = (int64_t)(info.duration*mAVStream->time_base.den/(1000*mAVStream->time_base.num));
        } else {
            info.duration_base = -1;
            info.duration = -1;
        }
    }
    return info;
}

DecodingResult PCMDecoder::decodeFullPCM(int sp_rate, int channel, char **data, int &size){
    //
    if(initResult != D_SUCCESS)
        return initResult;
    //------------Define Resampler--------------//
    int frame_count = 0;
    uint64_t src_ch_layout, dst_ch_layout;
    int src_rate, dst_rate = sp_rate;
    int dst_nb_channels = channel ;
    if(dst_nb_channels == MONO){
        dst_ch_layout = AV_CH_LAYOUT_MONO;
    }else if(dst_nb_channels == STEREO){
        dst_ch_layout = AV_CH_LAYOUT_STEREO;
    }
    enum AVSampleFormat src_sample_fmt, dst_sample_fmt = OUT_SAMPLE_FMT;
    int in_byte_per_sample;
    int out_byte_per_sample = av_get_bytes_per_sample(dst_sample_fmt);
    SwrContext* swrContext = NULL;
    //------------source information--------------//
    src_rate = mAVCodecContext->sample_rate;
    src_ch_layout = mAVCodecContext->channel_layout;
    src_sample_fmt = mAVCodecContext->sample_fmt;
    in_byte_per_sample = av_get_bytes_per_sample(src_sample_fmt);
    //
    int ret;
    int dst_nb_samples, new_number_samples=0;
    int dst_buff_size, new_file_size=0;
    int pos = 0;
    
    if(src_ch_layout == 0){
        if(mAVCodecContext->channels == MONO)
            src_ch_layout = AV_CH_LAYOUT_MONO;
        else if(mAVCodecContext->channels == STEREO)
            src_ch_layout = AV_CH_LAYOUT_STEREO;
    }
    swrContext = swr_alloc_set_opts(swrContext,			// Resample Context option setting
                                    dst_ch_layout, // out channel layout
                                    dst_sample_fmt, // out sample format
                                    dst_rate, // out sample rate
                                    src_ch_layout, // in channel layout
                                    src_sample_fmt, // in sample format
                                    src_rate, // in sample rate
                                    0, // log offset
                                    NULL); // log context
    if(swr_init(swrContext) < 0)
    {
        swr_free(&swrContext);
        return D_RESAMPLER_INIT_FAILED;
    }
    if (!swrContext)
    {
        swr_free(&swrContext);
        return D_RESAMPLER_INIT_FAILED;
    }
    //------------------ Decoding -------------------//
    AVPacket packet;
    av_init_packet(&packet);
    while (av_read_frame(mAVFormatContext, &packet) == 0)
    {
        if (packet.stream_index == mAVStream->index)
        {
            int frameFinished = 0;
            if(avcodec_decode_audio4(mAVCodecContext, frame, &frameFinished, &packet) < 0){
                av_free_packet(&packet);
                continue;
            }
            if (frameFinished)
            {
                frame_count++;
                if ((mAVCodecContext->channel_layout != src_ch_layout && mAVCodecContext->channel_layout != 0) || mAVCodecContext->sample_rate != src_rate || mAVCodecContext->sample_fmt != src_sample_fmt) {
                    src_rate = mAVCodecContext->sample_rate;
                    src_ch_layout = mAVCodecContext->channel_layout;
                    src_sample_fmt = mAVCodecContext->sample_fmt;
                    in_byte_per_sample = av_get_bytes_per_sample(src_sample_fmt);
                    
                    if(src_ch_layout == 0){
                        if(mAVCodecContext->channels == MONO)
                            src_ch_layout = AV_CH_LAYOUT_MONO;
                        else if(mAVCodecContext->channels == STEREO)
                            src_ch_layout = AV_CH_LAYOUT_STEREO;
                    }
                    
                    swrContext = swr_alloc_set_opts(swrContext,			// Resample Context option Re-setting
                                                    dst_ch_layout, // out channel layout
                                                    dst_sample_fmt, // out sample format
                                                    dst_rate, // out sample rate
                                                    src_ch_layout, // in channel layout
                                                    src_sample_fmt, // in sample format
                                                    src_rate, // in sample rate
                                                    0, // log offset
                                                    NULL); // log context
                    if(swr_init(swrContext) < 0){
                        swr_free(&swrContext);
                        return D_RESAMPLER_INIT_FAILED;
                    }
                    if (!swrContext){
                        return D_RESAMPLER_INIT_FAILED;
                    }
                }
                
                dst_nb_samples = (int) ((float)dst_rate/(float)src_rate*(float)frame->nb_samples) + 1;
                int outbuff_size = av_samples_get_buffer_size(NULL, dst_nb_channels, dst_nb_samples, dst_sample_fmt, 0);
                uint8_t* out_buff = (uint8_t*)malloc(outbuff_size);
                const uint8_t** in = (const uint8_t**)frame->data;
                uint8_t* out[8] = {out_buff, NULL};
                //
                if((ret = swr_convert(swrContext, out, dst_nb_samples, in, frame->nb_samples)) < 0){
                    av_free_packet(&packet);
                    continue;
                }
                new_number_samples = new_number_samples + ret;
                dst_buff_size = ret*out_byte_per_sample*dst_nb_channels;
                
                new_file_size = new_file_size + dst_buff_size;
                *data = (char*)realloc(*data, new_file_size);
                memcpy((*data) + new_file_size - dst_buff_size, out[0], dst_buff_size);
                free(out_buff);
            }
        }
        av_free_packet(&packet);
    }
    //
    //
    if (mAVCodecContext->codec->capabilities & CODEC_CAP_DELAY){
        av_init_packet(&packet);
        // Decode all the remaining frames in the buffer, until the end is reached
        int frameFinished = 0;
        while (avcodec_decode_audio4(mAVCodecContext, frame, &frameFinished, &packet) >= 0 && frameFinished)
        {
            if ((mAVCodecContext->channel_layout != src_ch_layout && mAVCodecContext->channel_layout != 0) || mAVCodecContext->sample_rate != src_rate || mAVCodecContext->sample_fmt != src_sample_fmt) {
                src_rate = mAVCodecContext->sample_rate;
                src_ch_layout = mAVCodecContext->channel_layout;
                src_sample_fmt = mAVCodecContext->sample_fmt;
                in_byte_per_sample = av_get_bytes_per_sample(src_sample_fmt);
                
                if(src_ch_layout == 0){
                    if(mAVCodecContext->channels == MONO)
                        src_ch_layout = AV_CH_LAYOUT_MONO;
                    else if(mAVCodecContext->channels == STEREO)
                        src_ch_layout = AV_CH_LAYOUT_STEREO;
                    else
                        src_ch_layout = av_get_channel_layout_nb_channels(mAVCodecContext->channels);
                }
                
                swrContext = swr_alloc_set_opts(swrContext,			// Resample Context option Re-setting
                                                dst_ch_layout, // out channel layout
                                                dst_sample_fmt, // out sample format
                                                dst_rate, // out sample rate
                                                src_ch_layout, // in channel layout
                                                src_sample_fmt, // in sample format
                                                src_rate, // in sample rate
                                                0, // log offset
                                                NULL); // log context
                if(swr_init(swrContext) < 0)
                {
                    swr_free(&swrContext);
                    free(*data);
                    return D_RESAMPLER_INIT_FAILED;
                }
                if (!swrContext)
                {
                    free(*data);
                    return D_RESAMPLER_INIT_FAILED;
                }
                
            }
            
            dst_nb_samples = (int) ((float)dst_rate/(float)src_rate*(float)frame->nb_samples) + 1;
            int outbuff_size = av_samples_get_buffer_size(NULL, CHANNELS_DEFAULT, dst_nb_samples, dst_sample_fmt, 0);
            uint8_t* out_buff = (uint8_t*)malloc(outbuff_size);
            const uint8_t** in = (const uint8_t**)frame->data;
            uint8_t* out[8] = {out_buff, NULL};
            //
            if((ret = swr_convert(swrContext, out, dst_nb_samples, in, frame->nb_samples)) < 0){
                continue;
            }
            new_number_samples = new_number_samples + ret;
            dst_buff_size = ret*out_byte_per_sample*dst_nb_channels;
            new_file_size = new_file_size + dst_buff_size;
            *data = (char*)realloc(*data, new_file_size);
            memcpy((*data) + new_file_size - dst_buff_size, out[0], dst_buff_size);
            free(out_buff);
        }
        av_free_packet(&packet);
    }
    size = new_file_size;
    swr_free(&swrContext);
    
    //---------------------------------- Output -------------------------------------//
    Logger::i(" Decoding finished: %s\n", filename);
    Logger::i(" PCM size: %d\n", new_file_size);
    Logger::i(" Duration: %f (second)\n", (float)audioInfo.duration/1000);
    Logger::i(" Bit_rate: %d\n", audioInfo.bit_rate);
    Logger::i(" Channels: %d\n", audioInfo.channels);
    Logger::i(" Sampling rate: %d Hz\n", audioInfo.sample_rate);
    //-------------------------------------------------------------------------------//
    return D_SUCCESS;
}

DecodingResult PCMDecoder::decodePCM(int sp_rate, int channel, char **data, int &size, int p_start, int p_end){
    if(initResult != D_SUCCESS)
        return initResult;
    //------------Define Resampler--------------//
    int frame_count = 0;
    uint64_t src_ch_layout, dst_ch_layout;
    int src_rate, dst_rate = sp_rate;
    int dst_nb_channels = channel ;
    if(dst_nb_channels == MONO){
        dst_ch_layout = AV_CH_LAYOUT_MONO;
    }else if(dst_nb_channels == STEREO){
        dst_ch_layout = AV_CH_LAYOUT_STEREO;
    }
    enum AVSampleFormat src_sample_fmt, dst_sample_fmt = OUT_SAMPLE_FMT;
    int in_byte_per_sample;
    int out_byte_per_sample = av_get_bytes_per_sample(dst_sample_fmt);
    SwrContext* swrContext = NULL;
    //
    int64_t s_time_base = (int64_t)((float)audioInfo.duration*p_start*mAVStream->time_base.den/(100*1000*mAVStream->time_base.num));
    int64_t e_time_base = (int64_t)((float)audioInfo.duration*p_end*mAVStream->time_base.den/(100*1000*mAVStream->time_base.num));
    if(s_time_base < 0 || s_time_base >= audioInfo.duration_base){
        return D_TIME_ERROR;
    }
    if(e_time_base > audioInfo.duration_base)
        e_time_base = audioInfo.duration_base;
    //
    int ret;
    int dst_nb_samples, new_number_samples=0;
    float duration = ((float)(e_time_base-s_time_base))*mAVStream->time_base.num/mAVStream->time_base.den;
    int dst_buff_size, new_file_size = dst_rate*duration*out_byte_per_sample*channel;
    int pos = 0;
    //------------source information--------------//
    src_rate = mAVCodecContext->sample_rate;
    src_ch_layout = mAVCodecContext->channel_layout;
    src_sample_fmt = mAVCodecContext->sample_fmt;
    in_byte_per_sample = av_get_bytes_per_sample(src_sample_fmt);
    
    if(src_ch_layout == 0){
        if(mAVCodecContext->channels == MONO)
            src_ch_layout = AV_CH_LAYOUT_MONO;
        else if(mAVCodecContext->channels == STEREO)
            src_ch_layout = AV_CH_LAYOUT_STEREO;
    }
    swrContext = swr_alloc_set_opts(swrContext,			// Resample Context option setting
                                    dst_ch_layout, // out channel layout
                                    dst_sample_fmt, // out sample format
                                    dst_rate, // out sample rate
                                    src_ch_layout, // in channel layout
                                    src_sample_fmt, // in sample format
                                    src_rate, // in sample rate
                                    0, // log offset
                                    NULL); // log context
    if(swr_init(swrContext) < 0)
    {
        swr_free(&swrContext);
        return D_RESAMPLER_INIT_FAILED;
    }
    if (!swrContext)
    {
        swr_free(&swrContext);
        return D_RESAMPLER_INIT_FAILED;
    }
    //------------------ Decoding -------------------//
    int r = av_seek_frame(mAVFormatContext,mAVStream->index, s_time_base, AVSEEK_FLAG_ANY);
    if(r<0){
        swr_free(&swrContext);
        return D_SEEK_FAILED;
    }
    if(av_seek_frame(mAVFormatContext, mAVStream->index, s_time_base, AVSEEK_FLAG_BACKWARD) < 0){
        swr_free(&swrContext);
        return D_SEEK_FAILED;
    }
    // Read remain frames
    int64_t first_ts = 0, last_ts = 0;
    *data = (char *)malloc(new_file_size);
    AVPacket packet;
    av_init_packet(&packet);
    while (av_read_frame(mAVFormatContext, &packet) == 0)
    {
        if (packet.stream_index == mAVStream->index)
        {
            if (e_time_base < (packet.dts - mAVStream->start_time))
                break;
            int frameFinished = 0;
            if(avcodec_decode_audio4(mAVCodecContext, frame, &frameFinished, &packet) < 0){
                av_free_packet(&packet);
                continue;
            }
            if (frameFinished)
            {
                if(frame_count == 0)
                    first_ts = packet.dts;
                frame_count++;
                if ((mAVCodecContext->channel_layout != src_ch_layout && mAVCodecContext->channel_layout != 0) || mAVCodecContext->sample_rate != src_rate || mAVCodecContext->sample_fmt != src_sample_fmt) {
                    src_rate = mAVCodecContext->sample_rate;
                    src_ch_layout = mAVCodecContext->channel_layout;
                    src_sample_fmt = mAVCodecContext->sample_fmt;
                    in_byte_per_sample = av_get_bytes_per_sample(src_sample_fmt);
                    
                    if(src_ch_layout == 0){
                        if(mAVCodecContext->channels == MONO)
                            src_ch_layout = AV_CH_LAYOUT_MONO;
                        else if(mAVCodecContext->channels == STEREO)
                            src_ch_layout = AV_CH_LAYOUT_STEREO;
                    }
                    
                    swrContext = swr_alloc_set_opts(swrContext,			// Resample Context option Re-setting
                                                    dst_ch_layout, // out channel layout
                                                    dst_sample_fmt, // out sample format
                                                    dst_rate, // out sample rate
                                                    src_ch_layout, // in channel layout
                                                    src_sample_fmt, // in sample format
                                                    src_rate, // in sample rate
                                                    0, // log offset
                                                    NULL); // log context
                    if(swr_init(swrContext) < 0){
                        swr_free(&swrContext);
                        return D_RESAMPLER_INIT_FAILED;
                    }
                    if (!swrContext){
                        return D_RESAMPLER_INIT_FAILED;
                    }
                }
                
                dst_nb_samples = (int) ((float)dst_rate/(float)src_rate*(float)frame->nb_samples) + 1;
                int outbuff_size = av_samples_get_buffer_size(NULL, dst_nb_channels, dst_nb_samples, dst_sample_fmt, 0);
                uint8_t* out_buff = (uint8_t*)malloc(outbuff_size);
                const uint8_t** in = (const uint8_t**)frame->data;
                uint8_t* out[8] = {out_buff, NULL};
                //
                if((ret = swr_convert(swrContext, out, dst_nb_samples, in, frame->nb_samples)) < 0){
                    av_free_packet(&packet);
                    continue;
                }
                new_number_samples = new_number_samples + ret;
                dst_buff_size = ret*out_byte_per_sample*dst_nb_channels;
                
                
                if (pos + dst_buff_size > new_file_size){
                    memcpy(*data + pos, out[0], new_file_size - pos);
                    free(out_buff);
                    break;
                }else{
                    memcpy(*data + pos, out[0], dst_buff_size);
                }
                pos = pos + dst_buff_size;
                free(out_buff);
                last_ts = packet.dts;
            }
        }
        av_free_packet(&packet);
    }
    //
    //
    if (mAVCodecContext->codec->capabilities & CODEC_CAP_DELAY){
        av_init_packet(&packet);
        // Decode all the remaining frames in the buffer, until the end is reached
        int frameFinished = 0;
        while (avcodec_decode_audio4(mAVCodecContext, frame, &frameFinished, &packet) >= 0 && frameFinished)
        {
            if ((mAVCodecContext->channel_layout != src_ch_layout && mAVCodecContext->channel_layout != 0) || mAVCodecContext->sample_rate != src_rate || mAVCodecContext->sample_fmt != src_sample_fmt) {
                src_rate = mAVCodecContext->sample_rate;
                src_ch_layout = mAVCodecContext->channel_layout;
                src_sample_fmt = mAVCodecContext->sample_fmt;
                in_byte_per_sample = av_get_bytes_per_sample(src_sample_fmt);
                
                if(src_ch_layout == 0){
                    if(mAVCodecContext->channels == MONO)
                        src_ch_layout = AV_CH_LAYOUT_MONO;
                    else if(mAVCodecContext->channels == STEREO)
                        src_ch_layout = AV_CH_LAYOUT_STEREO;
                    else
                        src_ch_layout = av_get_channel_layout_nb_channels(mAVCodecContext->channels);
                }
                
                swrContext = swr_alloc_set_opts(swrContext,			// Resample Context option Re-setting
                                                dst_ch_layout, // out channel layout
                                                dst_sample_fmt, // out sample format
                                                dst_rate, // out sample rate
                                                src_ch_layout, // in channel layout
                                                src_sample_fmt, // in sample format
                                                src_rate, // in sample rate
                                                0, // log offset
                                                NULL); // log context
                if(swr_init(swrContext) < 0)
                {
                    swr_free(&swrContext);
                    free(*data);
                    return D_RESAMPLER_INIT_FAILED;
                }
                if (!swrContext)
                {
                    free(*data);
                    return D_RESAMPLER_INIT_FAILED;
                }
                
            }
            
            dst_nb_samples = (int) ((float)dst_rate/(float)src_rate*(float)frame->nb_samples) + 1;
            int outbuff_size = av_samples_get_buffer_size(NULL, CHANNELS_DEFAULT, dst_nb_samples, dst_sample_fmt, 0);
            uint8_t* out_buff = (uint8_t*)malloc(outbuff_size);
            const uint8_t** in = (const uint8_t**)frame->data;
            uint8_t* out[8] = {out_buff, NULL};
            //
            if((ret = swr_convert(swrContext, out, dst_nb_samples, in, frame->nb_samples)) < 0){
                continue;
            }
            new_number_samples = new_number_samples + ret;
            dst_buff_size = ret*out_byte_per_sample*dst_nb_channels;
            
            if (pos + dst_buff_size > new_file_size){
                memcpy(*data + pos, out[0], new_file_size - pos);
                free(out_buff);
                break;
            }else{
                memcpy(*data + pos, out[0], dst_buff_size);
            }
            pos = pos + dst_buff_size;
            free(out_buff);
            last_ts = packet.dts;
        }
        av_free_packet(&packet);
    }
    size = new_file_size;
    swr_free(&swrContext);
    
    Logger::i(" Decoding finished: %s\n", filename);
    Logger::i(" PCM size: %d\n", new_file_size);
    Logger::i(" Duration: %f (second)\n", (float)audioInfo.duration/1000);
    Logger::i(" Bit_rate: %d\n", audioInfo.bit_rate);
    Logger::i(" Channels: %d\n", audioInfo.channels);
    Logger::i(" Sampling rate: %d Hz\n", audioInfo.sample_rate);
    
    return D_SUCCESS;
}

DecodingResult PCMDecoder::decodePCM(int sp_rate, int channel, char **data, int &size, float start, float end){
    
    return D_SUCCESS;
}

DecodingResult decodePCM(int sp_rate, int channel, char **data, int &size, int p_start, float duration){
    return D_SUCCESS;
}

























