#include "handlers.hpp"

InputState input_state;

// triggers
void toggle_mouse_bind()
{
    input_state.mouse_bound = (!input_state.mouse_bound);
}

void toggle_help_menu()
{
    input_state.help_menu = (!input_state.help_menu);
}

void toggle_inventory()
{
    input_state.inventory = (!input_state.inventory);
}

void toggle_scoreboard()
{
    input_state.scoreboard = (!input_state.scoreboard);
}

void toggle_map()
{
    input_state.map = (!input_state.map);
}

void toggle_chat()
{
    input_state.chat = (!input_state.chat);
}

void toggle_hud()
{
    input_state.hud = (!input_state.hud);
}

void enable_jump()
{
    input_state.can_jump = true;
}

void enable_quit()
{
    input_state.quit = true;
}

void toggle_input_mode()
{
    switch (input_state.input_mode)
    {
        case INPUT_STATE_AGENT:
            input_state.input_mode = INPUT_STATE_CAMERA;
            break;
        case INPUT_STATE_CAMERA:
            if (ClientState::playerAgent_state.you != NULL)
                input_state.input_mode = INPUT_STATE_AGENT;
            break;
        default: break;
    }
}

void toggle_camera_mode()
{
    switch (input_state.camera_mode)
    {
        case INPUT_STATE_AGENT:
            input_state.camera_mode = INPUT_STATE_CAMERA;
            break;
        case INPUT_STATE_CAMERA:
            if (ClientState::playerAgent_state.you != NULL)
                input_state.camera_mode = INPUT_STATE_AGENT;
            break;
        default: break;
    }
}


void init_handlers()
{
    // set input_state defaults
    input_state.mouse_bound = false;
    input_state.help_menu = false;
    input_state.inventory = false;
    input_state.scoreboard = false;
    input_state.map = false;
    input_state.chat = false;
    input_state.hud = true;
    
    input_state.can_jump = true;
    input_state.quit = false;

    input_state.input_mode = INPUT_STATE_CAMERA;
    input_state.camera_mode = INPUT_STATE_CAMERA;
}

/*
 * Reference:
 * http://wiki.libsdl.org/moin.cgi/SDLKeycodeLookup
 */

// key down
void key_down_handler(SDL_Event* event)
{

    //for Dany0 (azerty testing)
    //while holding n, will show key struct info
    if (keystate['r'] != 0)
    {
        printf("scancode = %d\n", (int)event->key.keysym.scancode);
        printf("keysym = %d\n", (int)event->key.keysym.sym);
        printf("\n");
    }

    switch (event->key.keysym.sym)
    {
        case SDLK_b:
            Animations::agent_bleed(
                ClientState::playerAgent_state.camera_state.x,
                ClientState::playerAgent_state.camera_state.y,
                ClientState::playerAgent_state.camera_state.z
            );
            break;
            
        case SDLK_g:
            toggle_camera_mode();
            break;
            
        case SDLK_h:
            toggle_help_menu();
            break;

        case SDLK_m:
            toggle_map();
            break;

        case SDLK_r:
            ClientState::playerAgent_state.action.reload();

        case SDLK_t:
            toggle_input_mode();
            break;
            
        case SDLK_u:
            toggle_mouse_bind();
            break;

        case SDLK_y:
            toggle_chat();
            break;

        case SDLK_TAB:
            toggle_scoreboard();
            break;

        case SDLK_SLASH:
            toggle_hud();
            break;

        case SDLK_LEFTBRACKET:
            ClientState::playerAgent_state.toggle_camera_mode();
            break;

        case SDLK_RIGHTBRACKET:
            Monsters::test(30);
            break;

        case SDLK_ESCAPE:
            enable_quit();
            break;

        case SDLK_HOME:
            save_screenshot();
            break;

        case SDLK_1:
            ClientState::playerAgent_state.action.switch_weapon(1-1);
            break;
        case SDLK_2:
            ClientState::playerAgent_state.action.switch_weapon(2-1);
            break;
        case SDLK_3:
            ClientState::playerAgent_state.action.switch_weapon(3-1);
            break;
        case SDLK_4:
            ClientState::playerAgent_state.action.switch_weapon(4-1);
            break;
        case SDLK_5:
            ClientState::playerAgent_state.action.switch_weapon(5-1);
            break;
        case SDLK_6:
            ClientState::playerAgent_state.action.switch_weapon(6-1);
            break;
        case SDLK_7:
            ClientState::playerAgent_state.action.switch_weapon(7-1);
            break;
        case SDLK_8:
            ClientState::playerAgent_state.action.switch_weapon(8-1);
            break;
        case SDLK_9:
            ClientState::playerAgent_state.action.switch_weapon(9-1);
            break;
        case SDLK_0:
            ClientState::playerAgent_state.action.switch_weapon(10-1);
            break;

        default: break;
    }
}

// key up
void key_up_handler(SDL_Event* event)
{

    switch (event->key.keysym.sym)
    {
        case SDLK_SPACE:
            enable_jump();
            break;
            
        default: break;
    }

}

// mouse down
void mouse_button_down_handler(SDL_Event* event)
{
    PlayerAgent_state* p;

    switch (event->button.button)
    {
        case SDL_BUTTON_LEFT:
            p = &ClientState::playerAgent_state;
            if (p->you == NULL) break;
            p->action.fire();
            break;

        case SDL_BUTTON_RIGHT:
            p = &ClientState::playerAgent_state;

            switch (input_state.input_mode)
            {
                case INPUT_STATE_AGENT:
                    if (p->you == NULL) break;
                    if (p->you->weapons.can_zoom())
                        agent_camera->toggle_zoom();
                    if (p->you->weapons.active == Weapons::TYPE_block_applier)
                        p->action.select_block();
                    break;

                case INPUT_STATE_CAMERA:
                    free_camera->toggle_zoom();
                    break;

                default: break;
            }

            break;

        case SDL_BUTTON_MIDDLE:
            // nothing
            break;

        case 4: // scroll up
            p = &ClientState::playerAgent_state;
            p->action.switch_weapon(-1);
            break;

        case 5: // scroll down
            p = &ClientState::playerAgent_state;
            p->action.switch_weapon(-2);
            break;

        default: break;
    }
    
}

// mouse up
void mouse_button_up_handler(SDL_Event* event)
{
}

// mouse motion
void mouse_motion_handler(SDL_Event* event)
{
}
