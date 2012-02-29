#pragma once

#include <physics/common.hpp>
#include <physics/verlet.hpp>


/* These should be pulled from a dat or something */
#define GRENADE_MAX 1024
#define GRENADE_TTL 100
#define GRENADE_DAMP 0.5f
#define GRENADE_TYPE 1
#define GRENADE_TEXTURE_ID 5
#define GRENADE_TEXTURE_SCALE 1.0f

#define GRENADE_BLOCK_DESTROY_RADIUS 2
#define GRENADE_DAMAGE_RADIUS 4.5f
#define GRENADE_SPLASH_DAMAGE 65
#define GRENADE_BLOCK_DAMAGE 32

using Verlet::VerletParticle;

class grenade_StoC;     // forward decl

class Grenade: public Particle
{
    private:
        int bounce_count;
    public:
        int owner;  // agent_id

        void draw();
        void tick();

        void explode();

        int block_damage(int dist);
        void damage_blocks();

        void create_message(grenade_StoC* msg);

        Grenade(int id);
        Grenade(int id, float x, float y, float z, float ix, float iy, float iz);
        ~Grenade();
};

#include <c_lib/template/object_list.hpp>
class Grenade_list: public Object_list<Grenade, GRENADE_MAX>
{
    private:
        const char* name() { return "Grenade"; }
    public:
        void draw();
        void tick() GNOMESCROLL_API;
};
