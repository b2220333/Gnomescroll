#pragma once

#include <stdlib.h>
#include <limits.h>

int seed_max = INT_MAX;

#include <c_lib/t_map/t_map.h>

//inline float fabs(float x){
    ////return (float&) ((unsigned int&)x)&0x7fffffff ;
    ////return ((unsigned int*)(&x))&0x7fffffff;
//}
inline float fabs(float x)
{
    int y = (int&)x & 0x7FFFFFFF;
    return (float&)y;
}

typedef union f3 {
    float xyz[3];
    float x, y, z;
} float3;

typedef union f4 {
    float xyzw[4];
    float x, y, z, w;
} float4;

const float GRAD3[][3] = {
    {1,1,0},{-1,1,0},{1,-1,0},{-1,-1,0}, 
    {1,0,1},{-1,0,1},{1,0,-1},{-1,0,-1}, 
    {0,1,1},{0,-1,1},{0,1,-1},{0,-1,-1},
    {1,0,-1},{-1,0,-1},{0,-1,1},{0,1,1}};

#define fastfloor(n) (int)(n) - (((n) < 0.0f) & ((n) != (int)(n)))


#define PERM_SIZE 256
// At the possible cost of unaligned access, we use char instead of
// int here to try to ensure that this table fits in L1 cache
//const unsigned char PERM[] = {
unsigned char PERM[] = {
  151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
  36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,
  234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
  88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
  134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
  230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161,
  1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135, 130,
  116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250,
  124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227,
  47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44,
  154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98,
  108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251, 34,
  242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14,
  239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121,
  50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243,
  141, 128, 195, 78, 66, 215, 61, 156, 180, 151, 160, 137, 91, 90, 15, 131,
  13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37,
  240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252,
  219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125,
  136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158,
  231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245,
  40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187,
  208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198,
  173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147, 118, 126,
  255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42, 223,
  183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167,
  43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185,
  112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179,
  162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181, 199,
  106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254, 138, 236,
  205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156,
  180
};

float noisemap[xmax*ymax*zmax];

void seed_noise(int seed);
void set_terrain_height(int x, int y, int z, int baseline, int maxheight, int tile);
void set_terrain_density(int x, int y, int z, float threshold, int tile);
void clear_noisemap();
void invert_map(int x, int y, int z, int tile);

int _oct = 1;
float _per = 0.6f;
float _amp = 1.0f;
float _lac = 2.0f;
float _freq = 1.0f;

void set_noise_parameters(int octaves, float persistence, float amplitude, float lacunarity, float frequency);

