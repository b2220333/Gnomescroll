#pragma once

#ifndef DC_CLIENT
    #define DC_CLIENT 1
#endif

#ifdef DC_SERVER
dont_include_this_file_in_server
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
 
#include <string.h>
#include <math.h>

#ifdef _WIN32
    #include "windows.h"
    #undef interface
#endif

#ifdef __APPLE__
#include <common/osx.hpp>
#endif

/* Common headers */
#include <common/version.h>
#include <common/defines.h>
#include <common/compat_gl.h>
#include <common/compat_al.h>
#include <common/macros.hpp>
#include <common/crash_report/stack_trace.hpp>

/* Compression */
#include <common/compression/miniz.c>

/* Templates */
 
#include <common/template/object_list.hpp>
#include <common/template/dynamic_object_list.hpp>
#include <common/template/multi_object_list.hpp>
#include <common/template/dynamic_multi_object_list.hpp>
#include <common/template/simple_object_list.hpp>
#include <common/template/object_pool.hpp>

/* configuration options */
#include <options/option_macros.hpp>
#include <options/client_options.cpp>

/* Network */
#include <net_lib/_include.hpp>

//utility
#include <common/gl_assert.hpp>
#include <common/macros.hpp>
#include <common/common.cpp>
#include <common/files.cpp>

// logging
#include <common/logger.cpp>

// time 
#include <common/time/physics_timer.cpp>

/* Vectors, Ray Tracers, Physics */
#include <physics/vec3.hpp>
#include <physics/vec4.hpp>
#include <physics/mat3.hpp>
#include <physics/mat4.hpp>
#include <physics/affine.hpp>

#include <physics/color_matrix.cpp>
#include <physics/ray_trace/ray_trace.cpp>
#include <physics/ray_trace/hitscan.cpp>
#include <physics/ray_trace/handlers.cpp>
#include <physics/common.cpp>
#include <physics/verlet.cpp>
#include <physics/verlet_particle.cpp>
#include <physics/motion.cpp>
#include <physics/quadrant.cpp>
/* SDL */
#include <SDL/shader_loader.cpp>
#include <SDL/texture_loader.cpp>
#include <SDL/texture_sheet_loader.cpp>
#include <SDL/draw_functions.cpp>
#include <SDL/particle_functions.c>
#include <SDL/SDL_functions.c>

#include <SDL/mesh_loader/obj_loader.cpp>

#include <SDL/md5_loader/_include.hpp>
//#include <SDL/md5_loader/md5_test.cpp>

/* Draw lists */
#include <common/draw/draw.cpp>

/* Voxel Models */
#include <voxel/voxel_volume.cpp>
#include <voxel/voxel_render.cpp>
#include <voxel/voxel_hitscan.cpp>
#include <voxel/voxel_dat.cpp>
#include <voxel/voxel_model.cpp>
#include <voxel/voxel_loader.cpp>
#include <voxel/vox_dat_init.cpp>

/* Entity system */
#include <entity/include.cpp>

/* particles */
//#include <particle/particle_lib.cpp>
#include <particle/_include.hpp>

/* Skybox */
#include <camera/skybox/skybox.cpp>

/* Agents */
#include <agent/agent_include.h>

/* animations */
#include <animations/_include.hpp>

/* Profiling */

#include <common/profiling/texture_surface.cpp>
#include <common/profiling/frame_graph.cpp>
#include <common/profiling/map_vbo_graph.cpp>

/* LUA */

#include <common/lua/lua.cpp>

/* map */

#include <t_map/_include.hpp>



#include <t_gen/_include.hpp>

/* mechanisms */
#include <t_mech/_include.hpp>

/* items */
#include <item/_include.hpp>
#include <item/container/_include.hpp>
#include <item/toolbelt/_include.hpp>
#include <item/particle/_include.hpp>

/* hud */
#include <t_hud/_include.hpp>

/* camera */
#include <camera/camera.cpp>
#include <camera/fulstrum_test.cpp>
#include <camera/fulstrum_test2.cpp>
//#ifdef linux
//#include <SDL/IMG_savepng.c>
//#endif

/* HUD */

#include <hud/reticle.cpp>
#include <hud/cube_selector.cpp>
#include <hud/font.cpp>

#include <hud/text.cpp>
#include <hud/map.cpp>
#include <hud/equipment.cpp>
#include <hud/compass.cpp>
#include <hud/hud.cpp>

/* input */
#include <input/input.cpp>
#include <input/handlers.cpp>
#include <input/skeleton_editor.cpp>

/* sound */
#include <sound/wav.cpp>
#include <sound/triggers.cpp>
#include <sound/csv_parser.cpp>
#include <sound/openal.cpp>
#include <sound/sound.cpp>

/* chat */
#include <chat/packets.cpp>
#include <chat/client.cpp>

#include <chat/interface.cpp>

/* client side map gen / utils */
//#include <map_gen/hopcroft-karp.cpp>
//#include <map_gen/dragon.cpp>

#include <state/client_state.cpp>
#include <state/packet_init.cpp>

//#include <unistd.h>

#ifdef linux
#include <signal.h>

void close_c_lib();
void signal_terminate_handler(int sig)
{
    close_c_lib();
    exit(1);
}
#endif

int init_c_lib() 
{    
    static int inited = 0;
    GS_ASSERT(inited == 0);
    inited++;

    #ifdef linux
    signal(SIGTERM, signal_terminate_handler);  // kill <pid>
    signal(SIGINT, signal_terminate_handler);   // ctrl-c
    #endif

    Log::init();
    printf("init c_lib\n");

    LUA::load_options(); //load game options
    srand(time(NULL));   // seed the RNG

    Components::init();
    Objects::init_net_interfaces();
    Objects::init();    // Entity system

    _set_resolution(Options::width, Options::height, Options::fullscreen);
    init_video();
    Sound::init();

    init_image_loader();
    TextureSheetLoader::init();
    //printf("Checkpoint 1 \n");
    t_map::init_t_map();
    //printf("Checkpoint 2 \n");
    HudCubeSelector::init();
    //printf("Checkpoint 3 \n");
    //lua_load_block_dat();  /* Load Block Dat */
    t_map::load_block_dat();

    //printf("Checkpoint 4 \n");
    t_map::init_for_draw();
    t_map::blit_block_item_sheet();
    //printf("Checkpoint 5 \n");

    HudText::init();
    HudFont::init();
    HudMap::init();

    Item::init();
    ItemContainer::init();

    Toolbelt::init();

    // Load Dats

    //t_map::load_map_dat();
    Item::load_item_dat();
    Item::load_nanite_store();

    t_map::load_map_drop_dat(); //load drop dat after items

    TextureSheetLoader::init_greyscale();   //item sheet grey scale textures
    TextureSheetLoader::init_item_texture();

    t_hud::init();
    t_hud::draw_init();

    t_mech::draw_init();
    //t_mech::state_init();

    Particle::init_particles();
    ItemParticle::init();

    Skybox::init();
    //Sound::test();
 
    HudEquipment::init();
    HudReticle::init();
    Compass::init();
    Hud::init();
    //vn::init();

    ClientState::init();

    VoxDats::init();
    init_voxel_volume();
    
    init_network();
    NetClient::init_net_client();
    
    init_input();
    init_handlers();
    init_cameras();
    init_chat_client();
    Particle::draw_init();
    ItemParticle::draw_init();

    Animations::init();

    obj_load::init_draw_model();

    //t_map::init_block_item_sheet();
    Item::load_crafting_dat();
    Item::load_smelting_dat();

    //init_voronoi_noise_maps();
    
    t_gen::noise_map_test();

    Hud::init_hud_draw_settings();

    //check_gl_error();
    return 0;
}

void close_c_lib()
{
    printf("Closing game...\n");

    #define TEARDOWN_DEBUG 0

    if (TEARDOWN_DEBUG) printf("t_map end t map\n");
    t_map::end_t_map();

    if (TEARDOWN_DEBUG) printf("t_hud draw teardown\n");
    t_hud::draw_teardown();
    if (TEARDOWN_DEBUG) printf("t_hud teardown\n");
    t_hud::teardown();
    if (TEARDOWN_DEBUG) printf("t_mech draw teardown\n");
    t_mech::draw_teardown();
    //t_mech::state_teardown();
    if (TEARDOWN_DEBUG) printf("particle draw teardown\n");
    Particle::draw_teardown();
    if (TEARDOWN_DEBUG) printf("item particle draw teardown\n");
    ItemParticle::draw_teardown();
    
    if (TEARDOWN_DEBUG) printf("item particle teardown\n");
    ItemParticle::teardown();

    if (TEARDOWN_DEBUG) printf("skybox teardown\n");
    Skybox::teardown();

    if (TEARDOWN_DEBUG) printf("texture sheet loader teardown\n");
    TextureSheetLoader::teardown();

    if (TEARDOWN_DEBUG) printf("netclient shutdown\n");
    NetClient::shutdown_net_client();

    if (TEARDOWN_DEBUG) printf("camera teardown\n");
    teardown_cameras();
    if (TEARDOWN_DEBUG) printf("hudfont teardown\n");
    HudFont::teardown();
    if (TEARDOWN_DEBUG) printf("chat client teardown\n");
    teardown_chat_client();

    if (TEARDOWN_DEBUG) printf("objects teardown\n");
    Objects::teardown();    // Entity system
    if (TEARDOWN_DEBUG) printf("components teardown\n");
    Components::teardown();
    if (TEARDOWN_DEBUG) printf("object net interfaces teardown\n");
    Objects::teardown_net_interfaces();
    
    if (TEARDOWN_DEBUG) printf("voxel volume teardown\n");
    teardown_voxel_volume();
    if (TEARDOWN_DEBUG) printf("hud text teardown\n");
    HudText::teardown();
    
    // free surfaces
    if (TEARDOWN_DEBUG) printf("t_map teardown\n");
    t_map::teardown_shader();
    if (TEARDOWN_DEBUG) printf("hud map teardown\n");
    HudMap::teardown();

    if (TEARDOWN_DEBUG) printf("client state teardown\n");
    ClientState::teardown();

    if (TEARDOWN_DEBUG) printf("particle teardown particles\n");
    Particle::teardown_particles(); // teardown after ClientState::agent_list, because of billboard particle

    if (TEARDOWN_DEBUG) printf("item teardown\n");
    Item::teardown();
    if (TEARDOWN_DEBUG) printf("toolbelt teardown\n");
    Toolbelt::teardown();
    if (TEARDOWN_DEBUG) printf("container teardown\n");
    ItemContainer::teardown();

    if (TEARDOWN_DEBUG) printf("sound close\n");
    Sound::close();
    if (TEARDOWN_DEBUG) printf("SDL close\n");
    close_SDL();

    #undef TEARDOWN_DEBUG

    printf("Game closed\n");

    Log::teardown();
}
