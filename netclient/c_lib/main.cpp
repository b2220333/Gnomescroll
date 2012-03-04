#include "main.hpp"

#include <c_lib/state/client_state.hpp>
#include <c_lib/SDL/SDL_functions.h>
#include <c_lib/sound/sound.hpp>
#include <c_lib/state/client_state.hpp>
#include <c_lib/camera/camera.hpp>
#include <c_lib/animations/animations.hpp>
#include <net_lib/host.hpp>
#include <c_lib/time/physics_timer.h>
#include <c_lib/t_map/t_vbo.hpp>
#include <c_lib/options.hpp>
#include <c_lib/common/common.hpp>

#include <c_lib/time/frame_rate_limiter.hpp>
#include <c_lib/common/profiling/frame_graph.hpp>

bool _quit = false;

/*
    #define INTERCEPT_CTRL_C 1
    #define GRACEFULL_CTRL_C_SHUTDOWN 0
    #ifdef linux
        #include <signal.h>
    #include <pthread.h>
        void intHandler(int dummy=0) 
        {
        #if GRACEFULL_CTRL_C_SHUTDOWN
            _quit = true;
        #else
            printf("Attempting Force Close\n");
            abort();
        #endif
        }
    #endif

    #if INTERCEPT_CTRL_C
        #ifdef linux
            signal(SIGINT, intHandler);
        #endif
    #endif
*/

namespace Main
{

void init()
{
    _set_resolution(Options::width, Options::height, Options::fullscreen);
    init_c_lib();
    ClientState::set_desired_name(Options::name);
    ClientState::ctf->start();
    _START_CLOCK(); // must start before networking

    // parse ip address and connect
    int address[4];
    address_from_string(Options::server, address);
    client_connect_to(address[0], address[1], address[2], address[3], Options::port);
}

int run()
{
    using Profiling::frame_graph;
    Profiling::init_frame_graph();

/* BEGIN SETUP */
    int ping_ticks = _GET_MS_TIME();
    
    int fps_average_index = 0;
    int fps_average[30+1];
    int fps_last_tick = _GET_MS_TIME();
    float fps_value = 0.0f;
/* END SETUP */

    // update mouse

    pan_camera();

    while (!input_state.quit)
    {

        if(_quit) break;


        frame_graph->frame_start();

        // update mouse

        pan_camera();
        
        // physics loop
        int physics_ticks = 0;
        while (1)
        {
            int tick_count = _GET_TICK();
            if (tick_count == 0 || physics_ticks > 0)
                break;
            physics_ticks++;

            // input
            process_events();
            get_key_state();

            // tick animations
            Animations::animations_tick();
            
            // tick client state
            ClientState::tick_client_state();

            // update sound listener
            ClientState::playerAgent_state.update_sound();

            // update mouse
            pan_camera();
        }

        if (physics_ticks >= 2)
            printf("Physics: %d ticks this frame\n", physics_ticks);

        // update mouse
        pan_camera();
            
        // update hud projected names
        ClientState::playerAgent_state.display_agent_names();

        // check if we've failed to receive any identify packets (lost in initialization)
        // Shouldn't be needed? initialization packets are reliable
        ClientState::agent_list->check_missing_names();  // will ratelimit itself
        ClientState::agent_list->check_if_at_base();  // will ratelimit itself

        // dispatch
        client_dispatch_network_events();
        // flush network
        flush_to_net();

        // set input options (set these in an options struct at load)   TODO
        set_input_options(
            Options::invert_mouse,
            Options::sensitivity
        );

        // update mouse
        pan_camera();

        apply_camera_physics(); //apply velocity
        // update camera state
        ClientState::update_camera();

        // switch to world projection
        world_projection();

        // draw map
        t_map::draw_map();


        // draw client state
        ClientState::agent_list->draw();
        ClientState::voxel_render_list->draw();
        
        ClientState::cspray_list->draw();
        ClientState::grenade_list->draw();
        ClientState::shrapnel_list->draw();
        ClientState::blood_list->draw();
        ClientState::minivox_list->draw();
        ClientState::billboard_text_list->draw();


        // draw animations
        Animations::animations_draw();

        // update mouse
        pan_camera();

        if (Options::hud)
        {
            // switch to hud  projection
            hud_projection();
            
            // draw hud
            Hud::set_hud_fps_display(fps_value);
            Hud::update_hud_draw_settings();
            Hud::draw_hud();
        }


        // update sound
        Sound::update();
        // update client_state
        ClientState::update_client_state();


        // flip sdl
        //frame_left(); //swap every 15 ms?

        //frame_graph->frame_flip_start();

        frame_graph->frame_stage(1);

        _swap_buffers();

        //updates 

        //frame_graph->frame_map_update_start();
        t_map::update_map();
        //frame_graph->frame_voxel_update_start();
        ClientState::voxel_render_list->update();

        //wait
        //frame_graph->frame_wait_start();
        
        frame_graph->frame_wait_stage(2);

        //frame_left(); //swap every 15 ms?

        /*
            Sample input during wait!!
        */
        frame_graph->frame_end(); //stage 3

        // do fps calculation
        if (Options::fps)
        {
            int fps_current_tick = _GET_MS_TIME();
            fps_average[fps_average_index++] = fps_current_tick - fps_last_tick;
            fps_last_tick = fps_current_tick;
            if (fps_average_index > 30)
            {
                int sum = 0;
                for (int i=0; i<fps_average_index; i++)
                    sum += fps_average[i];

                fps_value = ((float)sum) / ((float)fps_average_index);
                fps_average_index = 0;
            }
        }

        // check ping throttle
        if (Options::ping)
        {
            int ping_now = _GET_MS_TIME();
            if (ping_now - ping_ticks > Options::ping_update_interval)
            {
                ping_ticks = ping_now;
                ClientState::send_ping();
            }
        }

        // update mouse
        pan_camera();

    }

    Profiling::teardown_frame_graph();

    close_c_lib();
    
    return 0;
}

}
