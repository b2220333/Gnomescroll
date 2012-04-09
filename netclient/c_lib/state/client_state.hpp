#pragma once

#ifdef DC_CLIENT

//#include <c_lib/agent/player_agent.hpp>

//#include <c_lib/particles/particle_lib.hpp>

/*
#include <c_lib/agent/agent_list.hpp>
#include <c_lib/game/ctf.hpp>
#include <c_lib/voxel/voxel_render.hpp>

#include <c_lib/hud/text.hpp>
*/

#include <c_lib/items/items.hpp>
#include <c_lib/monsters/monsters.hpp>

#include <c_lib/game/ctf.hpp>
//#include <c_lib/particles/particle_lib.hpp>

//move
#include <c_lib/animations/hitscan.hpp>
#include <c_lib/animations/hitscan_laser.hpp>

class PlayerAgent_state; //forward declaration

namespace ClientState {

    extern PlayerAgent_state playerAgent_state;
    /* SINGLETONS
     * Make sure to extern declare* in cython_imports
     *          */

    extern class Agent_list* agent_list;

    extern class  ColoredMinivoxList* colored_minivox_list;

    //  property lists
    extern class  Voxel_render_list* voxel_render_list;
    extern class  Voxel_hitscan_list* voxel_hitscan_list;
    extern class  SpawnerList* spawner_list;
    extern class  OwnedList* owned_list;
    extern class  SpriteList* sprite_list;


    extern class  GameObject_list* object_list;

    //move into class
    //extern class  Animations::HitscanEffect_list* hitscan_effect_list;
    //extern class  Animations::HitscanLaserEffect_list* hitscan_laser_effect_list;   

    extern class  HitscanEffect_list* hitscan_effect_list;
    extern class  HitscanLaserEffect_list* hitscan_laser_effect_list;   

    extern class  CTF* ctf;

    extern char desired_name[PLAYER_NAME_MAX_LENGTH+1];
    extern int last_ping_time;
    extern int last_reliable_ping_time;
    
    void set_desired_name(char* name);
    int get_team_color(int team, unsigned char *r, unsigned char *g, unsigned char *b);
        
    void set_PlayerAgent_id(int id);
    void client_id_received(int client_id);

    void update();
    void init();
    void init_lists();
    void teardown();
    void teardown_lists();

    // connect callback
    void on_connect();
    void on_disconnect();

    void send_identify_packet(char* name);
    void update_camera();

}
#endif

