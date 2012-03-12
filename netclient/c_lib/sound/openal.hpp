#pragma once

#ifdef USE_OPENAL

namespace OpenALSound
{

using Sound::Soundfile;

void init();
void close();

void set_volume(float vol);

void load_sound(Soundfile* snd);

int play_2d_sound(char* file);
int play_3d_sound(char* file, float x, float y, float z, float vx, float vy, float vz, float ox=0, float oy=0, float oz=-1);

void update_listener(float x, float y, float z, float vx, float vy, float vz, float fx, float fy, float fz, float ux, float uy, float uz);
void update();

int test();

void enumerate_devices();

}

#endif
