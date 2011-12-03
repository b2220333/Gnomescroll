#pragma once

#include <compat.h>
#ifdef DC_CLIENT
    #include <compat_gl.h>
#endif

#include <c_lib/voxel/common.h>
#include <physics/common.h>
#include <ray_trace/ray_trace.h>
#include <t_map/t_map.hpp>
#include <t_map/t_properties.h>
#include <t_map/t_vbo.h>

#define MINIVOX_MAX 4096
#define MINIVOX_TTL 100
#define MINIVOX_DAMP 0.5f
#define MINIVOX_TYPE 6

/* voxel properties */
float minivox_size = 0.05f;
#define MINIVOX_R 136
#define MINIVOX_G 27
#define MINIVOX_B 224
#define MINIVOX_A 255

#define MINIVOX_DRAW_MODE_TEXTURED 1

class Minivox {
    private:
        float theta;
        float phi;
        void orient_vectors();

    public:
        Particle2 particle;

        float size;

        Vector vec_x;
        Vector vec_y;
        Vector vec_z;
        
        Voxel vox;

        void set_color(unsigned char r, unsigned char g, unsigned char b);
        void set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
        void set_orientation(float theta, float phi);
        void set_ttl(int ttl) {
            particle.ttl = ttl;
        }

        int draw_mode;
        float pix_margin;
        float tx,ty;
        int texture_pixel_width;
        void set_texture(int tex_id);
        void set_texture(int tex_id, int pixels_wide);
        
        void draw();
        void draw_textured();
        void tick();
        Minivox(int id);
        Minivox(int id, float x, float y, float z, float vx, float vy, float vz);
};

#include <c_lib/template/object_list.hpp>

class Minivox_list: public Object_list<Minivox, MINIVOX_MAX>
{
    private:
        const char* name() { return "Minivox"; }
    public:
        void draw();
        void tick();

        float _s;
        void set_size(float s) {
            _s = minivox_size;
            minivox_size = s;
        }
        void unset_size() {
            minivox_size = _s;
        }

        Minivox_list() : _s(minivox_size) {}
};
    
