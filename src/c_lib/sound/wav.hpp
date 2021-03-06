/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#pragma once

#if DC_SERVER
# error Do not include this file in the server
#endif

#include <common/compat_al.h>

namespace Sound
{

const int MAX_WAV_BUFFERS = 64;
typedef struct WavFile
{
    short format;
    short channels;
    int sample_rate;
    int byte_rate;
    short bits_per_sample;
    float duration;
    size_t size;
    bool in_use;
} WavData;

ALenum get_openal_wav_format(WavData* data);

void print_wav_data(WavData* data);

// returns buffer id.  make sure to free *buffer after binding to an ALbuffer
int load_wav_file(const char* filename, unsigned char** buffer);

void release_wav_data(int buffer_id);
WavData* get_loaded_wav_data(int buffer_id);

void init_wav_buffers();
void teardown_wav_buffers();

}   // Sound
