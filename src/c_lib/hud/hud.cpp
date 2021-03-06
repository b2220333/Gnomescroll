/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#include "hud.hpp"

#include <float.h>
#include <hud/reticle.hpp>
#include <hud/cube_selector.hpp>
#include <hud/font.hpp>
#include <input/handlers.hpp>
#include <options/options.hpp>
#include <common/profiling/frame_graph.hpp>
#include <item/_interface.hpp>
#include <item/toolbelt/_interface.hpp>
#include <t_map/_interface.hpp>
#include <hud/form.hpp>
#include <hud/_interface.hpp>
#include <hud/guide.hpp>

/* Configuration */
namespace Hud
{

FormScreen auth_screen;
LoginForm login_form;
CreateAccountForm create_account_form;

TerminalRenderer terminal_renderer;
MeterGraphic meter_graphic;

static const char help_text[] =
"    Key:            Action:\n"
"    Esc             Quit\n"
"    WASD            Move\n"
"    Space           Jump\n"
"    Left Shift      Jetpack\n"
"    E               Open inventory\n"
"    Num keys        Select item in hand\n"
"    Scrollwheel     Select item in hand\n"
"    Left click      Trigger item in hand\n"
"    Right click     Open a container, furnace etc\n"
"    Arrow keys      Select block within block placer\n"
"    T, Enter        Chat\n"
"    `               Show chat log\n"
"    H               Toggle help\n"
"    ;               Open guide\n"
"    Tab             See who is playing\n"
"    Home(print screen)            Take Screenshot\n"
"  Chat Commands:\n"
"       /home               Go to the spawn\n"
"       /kill               Suicide\n"
"       /color R G B        Choose body color\n"
"  \n"
"  \n"
"  wiki.gnomescroll.com   for info & recipes"
;

static struct HudDrawSettings
{
    bool zoom;
    bool cube_selector;
    bool help;
    bool dead;
    float fps_val;
    int ping_val;
    int reliable_ping_val;
    bool agent_status;
    bool chat;  // draw chat messages normally (using timeouts)
    bool player_chat;   // draw other players' chat messages
    bool chat_input;    // draw chat input area
    bool full_chat;     // draw chat messages (ignoring timeouts)
    bool scoreboard;
    bool map;
    bool draw;
    bool confirm_quit;
    bool prompt;
    bool diagnostics;
    bool vbo_debug;
    bool admin_controls;
    bool terminal_is_open;
} hud_draw_settings;

void set_hud_fps_display(float fps_val)
{   // sanitize
    fps_val = (fps_val >= 1000.0f) ? 999.99f : fps_val;
    fps_val = (fps_val < 0.0f) ? 0.0f : fps_val;
    hud_draw_settings.fps_val = fps_val;
}

void init_hud_draw_settings()
{
    update_hud_draw_settings();
}

void update_hud_draw_settings()
{   // read game state to decide what to draw
    hud_draw_settings.draw = input_state.draw_hud;

    hud_draw_settings.zoom = current_camera->zoomed;
    static int block_placer_type = Item::get_item_type("block_placer");
    GS_ASSERT(block_placer_type != NULL_ITEM_TYPE);
    hud_draw_settings.cube_selector = (Toolbelt::get_selected_item_type() == block_placer_type);

    hud_draw_settings.help = input_state.help_menu;
    if (hud_draw_settings.help) hud_draw_settings.prompt = false;   // clear this after opening help once

    class Agents::Agent* you = ClientState::player_agent.you();
    hud_draw_settings.dead = (you != NULL && you->status.dead);

    // sanitize
    int ping_val = ClientState::last_ping_time;
    ping_val = (ping_val >= 1000) ? 999 : ping_val;
    hud_draw_settings.ping_val = (ping_val < 0) ? 0 : ping_val;

    int reliable_ping_val = ClientState::last_reliable_ping_time;
    reliable_ping_val = (reliable_ping_val >= 1000) ? 999 : reliable_ping_val;
    hud_draw_settings.reliable_ping_val = (reliable_ping_val < 0) ? 0 : reliable_ping_val;

    hud_draw_settings.agent_status = true;

    hud_draw_settings.chat = true;
    hud_draw_settings.player_chat = Options::player_chat;
    hud_draw_settings.chat_input = input_state.chat;
    hud_draw_settings.terminal_is_open = input_state.terminal_is_open;
    hud_draw_settings.full_chat = input_state.full_chat;
    hud_draw_settings.admin_controls = input_state.admin_controls;

    hud_draw_settings.scoreboard = input_state.scoreboard;

    hud_draw_settings.map = input_state.map;

    hud_draw_settings.diagnostics = input_state.diagnostics;
    hud_draw_settings.vbo_debug = input_state.vbo_debug;

    // update chat rendering
    if (hud != NULL && hud->inited &&
        hud->chat != NULL && hud->chat->inited)
    {
        HudText::Text *t = hud->chat->input;
        if (t != NULL)
        {
            if (Chat::chat_client != NULL)
                t->set_text(Chat::chat_client->input->buffer);
            hud->chat->set_cursor(t->text, t->x, t->y);
        }

        bool timeout = true;
        if (hud_draw_settings.full_chat)
            timeout = false;
        else if (hud_draw_settings.chat_input)
            timeout = false;
        hud->chat->update(timeout, hud_draw_settings.player_chat);
    }

    hud_draw_settings.confirm_quit = input_state.confirm_quit;
    hud_draw_settings.prompt = true;

    IF_ASSERT(hud == NULL)
        hud_draw_settings.prompt = (hud->prompt->text != NULL &&
                                    hud->prompt->text[0] != '\0');
}

void set_prompt(const char* msg)
{   // Set prompt text for flashy message
    IF_ASSERT(hud == NULL) return;
    hud->prompt->set_text(msg);
    hud->prompt->set_position((_xresf - hud->prompt->get_width()) / 2.0f, hud->prompt->get_height() + HudContainer::toolbelt->height() + 30);
}

void clear_prompt(const char* msg)
{
    IF_ASSERT(hud == NULL) return;
    if (strcmp(msg, hud->prompt->text) == 0)
        hud->prompt->set_text("");
    hud->prompt->set_position((_xresf - hud->prompt->get_width()) / 2.0f, hud->prompt->get_height() + HudContainer::toolbelt->height() + 30);
}

void set_login_message(const char* msg)
{
    IF_ASSERT(hud == NULL) return;
    hud->awesomium_message->set_text(msg);
}

void clear_login_message(const char* msg)
{   // clears if msg matches current text
    IF_ASSERT(hud == NULL) return;
    if (strcmp(msg, hud->awesomium_message->text) == 0)
        clear_login_message();
}

void clear_login_message()
{
    IF_ASSERT(hud == NULL) return;
    hud->awesomium_message->set_text("");
}

/* Draw routines */

void draw_reference_center()
{
    int w = 2;
    float x = (_xresf/2.0f) - w/2;
    float y = (_yresf/2.0f) - w/2;
    draw_rect(Color(10, 255, 255), x, y, w, w);
}

void set_color_from_ratio(float ratio, unsigned char alpha, bool invert_color_for_damage)
{
    // old durability color jumps

    //glColor4ub(7, 247, 0, alpha);    // green
    //glColor4ub(243, 247, 0, alpha);  // yellow
    //glColor4ub(247, 71, 0, alpha);   // red-orange
    //glColor4ub(247, 14, 0, alpha);   // red

    float small_to_big = ratio * 2.0f;
    Color red    = Color(255,   0, 0, alpha);
    Color green  = Color(0,   255, 0, alpha);
    Color yellow = Color(255, 190, 0, alpha);

    Color full = green;
    Color empty = red;
    Color dyn;

    if (invert_color_for_damage)
    {
        full = red;
        empty = green;
    }

    if (small_to_big > 1.0f)
    {
        small_to_big -= 1.0f;
        dyn = interpolate_color(yellow, full, small_to_big);
    }
    else
        dyn = interpolate_color(empty, yellow, small_to_big);

    glColor4ub(dyn.r, dyn.g, dyn.b, alpha);
}

/* Display logic */

static void draw_weapon_cooldown_meter()
{
    AgentID agent_id = ClientState::player_agent.agent_id;
    if (!isValid(agent_id)) return;
    ItemType item_type = Toolbelt::get_selected_item_type();
    int cooldown = Toolbelt::agent_fire_cooldown[agent_id];
    int cooldown_max = Item::get_item_fire_rate(item_type);
    float cooldown_ratio = float(cooldown) / float(cooldown_max);
    int w = _xres / 4;
    int h = _yres / 128;
    glColor4ub(200, 20, 150, 115);
    meter_graphic.draw(0, h, w, h, cooldown_ratio, MeterGraphic::METANCH_LEFT);
    glColor4ub(255, 255, 255, 255);
}

static void draw_weapon_charge_meter()
{
    AgentID agent_id = ClientState::player_agent.agent_id;
    if (!isValid(agent_id)) return;
    float charge_ratio = Toolbelt::get_charge_progress();
    int w = _xres / 4;
    int h = _yres / 128;
    unsigned char alpha = 115;
    if (charge_ratio >= 1.0f)
        alpha *= 2;
    glColor4ub(20, 200, 150, alpha);
    meter_graphic.draw(0, h*2, w, h, charge_ratio, MeterGraphic::METANCH_LEFT);
    glColor4ub(255, 255, 255, 255);
}

void draw_hud_textures()
{
    // meters
    int w = _xres / 4;
    int h = _yres / 128;

    if (input_state.login_mode)
    {
        auth_screen.draw();
        return;
    }

    // jetpack
    glColor4ub(255,255,255,115); // white, more than half translucent
    float ratio = float(ClientState::player_agent.jetpack.fuel) / float(Agents::JETPACK_FUEL_MAX);
    meter_graphic.draw(0,0, w,h, ratio, MeterGraphic::METANCH_LEFT);

    draw_weapon_cooldown_meter();
    draw_weapon_charge_meter();

    if (!hud_draw_settings.draw) return;

    if (!has_error())
    {
        if (hud_draw_settings.scoreboard)
            hud->scoreboard->draw_badges();
    }

    if (hud_draw_settings.zoom)
    {
        HudReticle::scope_reticle.draw();
        return;
    }

    HudReticle::reticle.draw();

    // draw terminal background
    if (hud_draw_settings.terminal_is_open)// && hud->inited && hud->chat != NULL && hud->chat->inited)
        terminal_renderer.draw_background();

    // draw terminal cursor
    if (hud_draw_settings.terminal_is_open)
        terminal_renderer.draw_cursor();

    if (hud_draw_settings.cube_selector)
        HudCubeSelector::cube_selector.draw();

    if (hud_draw_settings.map)
        HudMap::draw();

    if (hud_draw_settings.diagnostics)
    {
        using Profiling::frame_graph;
        frame_graph->draw(_xresf - frame_graph->ts->screen_width(), 0);
    }
}

void draw_login_message()
{
    start_font_draw();
    //const int large_text_size = 32;
    //HudFont::set_properties(large_text_size);
    HudFont::reset_default();
    set_texture();

    IF_ASSERT(hud == NULL || hud->awesomium_message == NULL) return;
    hud->awesomium_message->draw_centered();

    end_font_draw();
    HudFont::reset_default();
}

static void update_targeted_text(const char* s)
{
    IF_ASSERT(hud == NULL || hud->target == NULL) return;
    hud->target->set_text(s);
    float w = hud->target->get_width();
    float x = ((_xresf - w) * 0.5f);
    float y = (_yresf * 0.75f);
    hud->target->set_position(x, y);
}

static void draw_targeted_text()
{
    using ClientState::hitscan;
    ItemType equipped_type = Toolbelt::get_selected_item_type();
    float range = Item::get_weapon_range(equipped_type);
    update_targeted_text("");
    if (hitscan.distance > range || hitscan.type == HITSCAN_TARGET_NONE)
        return;

    switch (hitscan.type)
    {
        case HITSCAN_TARGET_VOXEL:
            if (hitscan.voxel_target.entity_type == ENTITY_AGENT)
            {
                Agents::Agent* agent = Agents::get_agent(AgentID(hitscan.voxel_target.entity_id));
                if (agent != NULL)
                    update_targeted_text(agent->status.name);
            }
            else
                update_targeted_text(Entities::get_entity_pretty_name(hitscan.voxel_target.entity_type));
            break;

        case HITSCAN_TARGET_BLOCK:
            update_targeted_text(t_map::get_cube_pretty_name(hitscan.cube_type));
            break;

        case HITSCAN_TARGET_MECH:
            {
                MechType mtype = t_mech::get_mech_type(hitscan.mech_id);
                if (isValid(mtype))
                {
                    if (mtype == t_mech::get_mech_type("terminal_basic"))
                    {
                        const char* text = t_mech::get_mech_text(hitscan.mech_id);
                        update_targeted_text(text);
                    }
                    else
                        update_targeted_text(t_mech::get_mech_pretty_name(mtype));
                }
            }
            break;

        case HITSCAN_TARGET_SPRITE_MOB:
            {
                t_mob::SpriteMob* mob = t_mob::get_sprite_mob(hitscan.sprite_mob_id);
                if (mob != NULL)
                    update_targeted_text(t_mob::get_mob_pretty_name(mob->type));
            }
            break;

        case HITSCAN_TARGET_NONE:
            break;
    }

    hud->target->draw();

    // rdn quotes:
    //       //MechType type = t_mech::get_mech_type(ClientState::hitscan.mech_id);
    //       //if (type == get_mech_type("terminal"))
    //       //    Input::open_terminal();


    ////HaltingSate quotes:
    //// > char* get_mech_text(int mech_id);
    ////  void mech_text_update(int mech_id, int pos, int key);
    ////  in interface .hpp
    ////  that gets the text
    ////  on the block
    ////  on the mech, by id
    ////  but check if its sign type block

}

void draw_hud_text()
{
    if (!hud->inited) return;
    if (!hud_draw_settings.draw) return;

    start_font_draw();
    HudFont::reset_default();
    set_texture();

    if (input_state.login_mode)
    {
        auth_screen.draw_text();
        end_font_draw();
        return;
    }

    if (!has_error())
    {
        // move large text to the front, so we dont swap textures twice
        const int large_text_size = 32;
        HudFont::set_properties(large_text_size);
        set_texture();

        if (hud_draw_settings.confirm_quit)
            hud->confirm_quit->draw_centered();
        else
        if (hud_draw_settings.dead)
            hud->dead->draw_centered();

        hud->awesomium_message->draw_centered();

        if (hud_draw_settings.map)
        {
            HudFont::set_properties(HudMap::text_icon_size);
            set_texture();
            HudMap::draw_text();
        }

        HudFont::reset_default();
        set_texture();

        if (hud_draw_settings.terminal_is_open)
            terminal_renderer.draw_text();

        if (hud_draw_settings.help)
            hud->help->draw();

        //draw_targeted_text();
    }
    else
    {
        HudFont::reset_default();
        set_texture();
    }

    if (hud->chat->inited)
    {
        if (hud_draw_settings.chat)
            hud->chat->draw_messages();
        if (hud_draw_settings.chat_input)
            hud->chat->draw_input();
    }

    if (!has_error())
    {
        if (hud_draw_settings.scoreboard)
            hud->scoreboard->draw_text();

        // everything after this is hidden when zoomed
        if (hud_draw_settings.zoom)
        {
            end_font_draw();
            return;
        }
    }

    if (input_state.admin_controls)
    {
        const char* camera = "Free";
        if (agent_camera->is_current())
            camera = "Agent";
        const char* controls = "Camera";
        if (input_state.input_mode == INPUT_STATE_AGENT)
            controls = "Agent";
        hud->admin_controls->update_formatted_string(2, controls, camera);
        hud->admin_controls->draw();
    }

    if (hud_draw_settings.diagnostics)
    {
        float fps_val = 0.0f;
        if (hud_draw_settings.fps_val >= 0.1f) fps_val = 1000.0f / hud_draw_settings.fps_val;
        hud->fps->update_formatted_string(1, fps_val);
        hud->fps->draw();

        hud->ping->update_formatted_string(1, hud_draw_settings.ping_val);
        hud->ping->draw();
        //hud->reliable_ping->update_formatted_string(1, hud_draw_settings.reliable_ping_val);
        //hud->reliable_ping->draw();

        hud->location->update_formatted_string(3, current_camera_position.x, current_camera_position.y, current_camera_position.z);
        hud->location->draw();

        if (current_camera != NULL)
        {
            Vec3 f = current_camera->forward_vector();
            hud->look->update_formatted_string(3, f.x, f.y, f.z);
            hud->look->set_position(hud->look->x, _yresf - hud->location->get_height() - HudFont::font->data.line_height);
            hud->look->draw();
        }
    }

    if (!has_error())
    {
        guide::update_guide();
        if (hud_draw_settings.agent_status)
        {
            Agents::Agent* a = ClientState::player_agent.you();
            if (a != NULL)
            {
                int health_max = Agents::get_attribute_int(a->id, "health_max");
                int health = Agents::get_attribute_int(a->id, "health");
                hud->health->update_formatted_string(1, health);
                int len = int(strlen(health_color_string));
                int n = 0;
                if (health >= health_max)
                    n = len;
                else if (health > 0)    // force to 0 in case of float point error
                    n = (float(health)/float(health_max))*float(len);
                if (health > 0 && n == 0) n = 1;

                // update green portion of health text
                int green_color_index = hud->health->get_color(HEALTH_GREEN);
                int green_range = hud->health->get_color_range(green_color_index);
                if (green_range < 0)
                {
                    green_range = hud->health->add_char_range(n, len);
                    hud->health->set_char_range_color(green_range, green_color_index);
                }
                else
                    hud->health->update_char_range(green_range, 0, n);

                // update grey portion of health text
                int grey_color_index = hud->health->get_color(HEALTH_GREY);
                int grey_range = hud->health->get_color_range(grey_color_index);
                if (grey_range < 0)
                {
                    grey_range = hud->health->add_char_range(n, len);
                    hud->health->set_char_range_color(grey_range, grey_color_index);
                }
                else
                    hud->health->update_char_range(grey_range, n, len);

                // update numbers color
                static int white_color_index = hud->health->get_color(HEALTH_WHITE);
                GS_ASSERT(white_color_index >= 0);
                int white_range = hud->health->get_color_range(white_color_index);
                static int red_color_index = hud->health->get_color(HEALTH_RED);
                GS_ASSERT(red_color_index >= 0);
                int red_range = hud->health->get_color_range(red_color_index);

                static int blink_tick = 0;
                static int blink_step = 1;
                if (health <= 0)
                {
                    Color blink_color = HEALTH_RED;
                    if (!a->status.dead)
                    {
                        const int NO_HEALTH_WARNING_TEXT_BLINK_RATE = 15;
                        blink_tick += blink_step;
                        if (blink_tick >= NO_HEALTH_WARNING_TEXT_BLINK_RATE)
                            blink_step = -1;
                        else if (blink_tick <= 0)
                            blink_step = 1;

                        float t = ((float)blink_tick)/((float)NO_HEALTH_WARNING_TEXT_BLINK_RATE);
                        blink_color = interpolate_color(HEALTH_WHITE, HEALTH_RED, t);
                    }

                    hud->health->set_color_index_color(red_color_index, blink_color);

                    int range = red_range;
                    if (range < 0) range = white_range;
                    GS_ASSERT(range >= 0);
                    hud->health->set_char_range_color(range, red_color_index);
                }
                else
                {
                    blink_tick = 0;
                    int range = white_range;
                    if (range < 0) range = red_range;
                    GS_ASSERT(range >= 0);
                    hud->health->set_char_range_color(range, white_color_index);
                }
            }
            else
                hud->health->set_text(no_agent_text);

            //hud->health->draw();
        }   // agent_status
    }

    end_font_draw();
}

void draw_guide()
{
    if (!hud_draw_settings.prompt || hud->prompt->empty())
        return;

    // draw a background
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    Vec2 p = hud->prompt->get_position();
    Vec2 d = hud->prompt->get_dimensions();
    p.y -= d.y;
    const float margin = 4.0f;
    d = vec2_add(d, vec2_init(margin));
    p = vec2_scalar_sub(p, margin);
    draw_rect(Color(0, 0, 0, 192), p, d);

    // draw the text
    HudFont::reset_default();
    HudFont::set_texture();
    start_font_draw();
    // blinks blue/white
    static unsigned int press_help_tick = 0;
    const int press_help_anim_len = 240;
    const Color start_color = interpolate_color(COLOR_BLUE, COLOR_WHITE, 0.5f);
    const Color end_color = interpolate_color(COLOR_BLUE, COLOR_WHITE, 0.85f);
    float t = float(press_help_tick % (2*press_help_anim_len)) / float(press_help_anim_len);
    t -= 1.0f;
    if (t < 0.0f)
        hud->prompt->set_color(interpolate_color(start_color, end_color, 1.0f+t));
    else
        hud->prompt->set_color(interpolate_color(end_color, start_color, t));
    hud->prompt->draw();
    press_help_tick++;
    end_font_draw();
}

/* HUD */

void HUD::init()
{
    auth_screen.init(2);
    auth_screen.add_form(&login_form);
    auth_screen.add_form(&create_account_form);
    float xq = _xresf / 2.0f;
    float y = _yresf * (1.0f/8.0f);
    login_form.set_position(vec2_init(xq - login_form.width()*0.5f, y * 6));
    login_form.focus();
    create_account_form.set_position(vec2_init(xq - create_account_form.width()*0.5f, y * 3));
    create_account_form.unfocus();

    IF_ASSERT(this->inited) return;
    IF_ASSERT(HudFont::font == NULL) return;
    using HudText::text_list;
    using HudText::Text;

    init_errors();
    int line_height = HudFont::font->data.line_height;

    help = text_list->create();
    IF_ASSERT(help == NULL) return;
    help->set_text(help_text);
    help->shadowed = true;
    int help_width = help->get_width();
    help->set_color(Color(255,255,255,255));
    help->set_position(_xres - help_width - 5, _yresf - 5);

    dead = text_list->create();
    IF_ASSERT(dead == NULL) return;
    dead->set_text(dead_text);
    dead->set_color(Color(Color(200,4,2)));
    dead->set_position(_xresf/2, _yresf/2);

    fps = text_list->create();
    IF_ASSERT(fps == NULL) return;
    fps->set_format(fps_format);
    fps->set_format_extra_length(6 - 5);
    fps->set_color(Color(200,4,2));
    fps->set_position(3, line_height+3);

    ping = text_list->create();
    IF_ASSERT(ping == NULL) return;
    ping->set_format(ping_format);
    ping->set_format_extra_length(3 - 2);
    ping->set_color(Color(200,4,2));
    ping->set_position(3, (line_height*2)+3);

    reliable_ping = text_list->create();
    IF_ASSERT(reliable_ping == NULL) return;
    reliable_ping->set_format(ping_format);
    reliable_ping->set_format_extra_length(3 - 2);
    reliable_ping->set_color(Color(200,4,2));
    reliable_ping->set_position(3, (line_height*3)+3);

    location = text_list->create();
    IF_ASSERT(location == NULL) return;
    location->set_format(location_format);
    location->set_format_extra_length((40 + 20 + 1 - 2) * 3);
    location->set_color(Color(200,4,2));
    location->set_position(3, _yresf-3);

    look = text_list->create();
    IF_ASSERT(look == NULL) return;
    look->set_format(location_format);
    look->set_format_extra_length((40 + 20 + 1 - 2) * 3);
    look->set_color(Color(200,4,2));
    look->set_position(3, _yresf-3);

    admin_controls = text_list->create();
    IF_ASSERT(admin_controls == NULL) return;
    admin_controls->set_format("Debug Mode\n%s controls\n%s camera");
    admin_controls->set_format_extra_length(strlen("Camera") + strlen("Agent"));
    admin_controls->update_formatted_string(2, "Camera", "Agent");
    admin_controls->set_color(Color(200,4,3));
    admin_controls->set_position(_xresf - 4 - admin_controls->get_width(), _yresf - 3);

    health = new AnimatedText;

    int health_max = Agents::get_base_attribute_int("health_max");
    health->set_format(health_format);
    health->set_format_extra_length(11);
    health->update_formatted_string(1, health_max);
    IF_ASSERT(HudContainer::energy_tanks == NULL) return;
    float health_x = HudContainer::energy_tanks->xoff + HudContainer::energy_tanks->width() + 1;
    health->set_position(health_x, _yresf - HudContainer::energy_tanks->yoff - health->get_height());
    health->set_color(Color(255,255,255,255));

    // add color for health text animations
    health->set_color_count(4);
    health->set_char_range_count(4);
    health->add_color(HEALTH_GREY);
    health->add_color(HEALTH_GREEN);
    health->add_color(HEALTH_RED);

    int white_range_index = health->add_char_range(strlen(health_color_string), -1);
    GS_ASSERT(white_range_index >= 0);
    int white_color_index = health->add_color(HEALTH_WHITE);
    GS_ASSERT(white_color_index >= 0);
    health->set_char_range_color(white_range_index, white_color_index);

    confirm_quit = text_list->create();
    IF_ASSERT(confirm_quit == NULL) return;
    confirm_quit->set_text(confirm_quit_text);
    confirm_quit->set_color(Color(200,4,2));
    confirm_quit->set_position(_xresf/2, (3*_yresf)/4);

    prompt = text_list->create();
    IF_ASSERT(prompt == NULL) return;
    if (Options::show_tips)
        prompt->set_text(press_help_text);
    else
        prompt->set_text("");
    prompt->set_color(Color(255,255,255,255));
    prompt->set_position((_xresf - prompt->get_width()) / 2.0f, prompt->get_height() + HudContainer::toolbelt->height());
    prompt->shadowed = true;

    target = text_list->create();
    IF_ASSERT(target == NULL) return;
    target->set_text("not looking at anything");
    target->set_color(Color(255,0,255,255));
    target->set_position((_xresf - target->get_width()) / 2.0f,
        _yresf/4);
    target->shadowed = true;

    error = text_list->create();
    IF_ASSERT(error == NULL) return;
    error->set_color(Color(200,4,2));
    error->set_position(_xresf/2, _yresf/2);

    error_subtitle = text_list->create();
    IF_ASSERT(error_subtitle == NULL) return;
    error_subtitle->set_color(Color(200,4,2));
    error_subtitle->set_position(_xresf/2, error->y - error->get_height());
    error_subtitle->set_text(error_subtitle_text);

    awesomium_message = text_list->create();
    IF_ASSERT(awesomium_message == NULL) return;
    awesomium_message->set_color(Color(200,4,2));
    awesomium_message->set_position(_xresf/2, _yresf - 2);
    awesomium_message->set_text("");

    scoreboard = new Scoreboard();
    scoreboard->init();

    chat = new ChatRender();
    chat->init();

    this->inited = true;
}

HUD::HUD() :
    inited(false),
    help(NULL),
    dead(NULL),
    fps(NULL),
    ping(NULL),
    reliable_ping(NULL),
    location(NULL),
    look(NULL),
    health(NULL),
    confirm_quit(NULL),
    prompt(NULL),
    target(NULL),
    error(NULL),
    error_subtitle(NULL),
    awesomium_message(NULL),
    admin_controls(NULL),
    scoreboard(NULL),
    chat(NULL)
{}

HUD::~HUD()
{
    using HudText::text_list;
    if (help != NULL)
        text_list->destroy(help->id);
    if (dead != NULL)
        text_list->destroy(dead->id);
    if (fps != NULL)
        text_list->destroy(fps->id);
    if (ping != NULL)
        text_list->destroy(ping->id);
    if (reliable_ping != NULL)
        text_list->destroy(reliable_ping->id);
    if (location != NULL)
        text_list->destroy(location->id);
    if (look != NULL)
        text_list->destroy(look->id);
    if (confirm_quit != NULL)
        text_list->destroy(confirm_quit->id);
    if (prompt != NULL)
        text_list->destroy(prompt->id);
    if (target != NULL)
        text_list->destroy(target->id);
    if (error != NULL)
        text_list->destroy(error->id);
    if (error_subtitle != NULL)
        text_list->destroy(error_subtitle->id);
    if (awesomium_message != NULL)
        text_list->destroy(awesomium_message->id);
    if (admin_controls != NULL)
        text_list->destroy(admin_controls->id);
    delete health;
    delete scoreboard;
    delete chat;
}

/* ChatRender */

void ChatRender::init()
{
    IF_ASSERT(this->inited) return;

    int line_height = HudFont::font->data.line_height + 2;
    const int lines_offset = 5;
    const int x_offset = 2;   // from the left
    for (int i=0; i<CHAT_MESSAGE_RENDER_MAX; i++)
    {
        HudText::Text* t = HudText::text_list->create();
        IF_ASSERT(t == NULL) return;
        t->set_position(x_offset, line_height * (CHAT_MESSAGE_RENDER_MAX+lines_offset - i));
        t->set_text("");
        t->set_format("%s%s%s");
        t->set_format_extra_length(PLAYER_NAME_MAX_LENGTH + Chat::CHAT_MESSAGE_SIZE_MAX + CHAT_NAME_SEPARATOR_LENGTH_MAX - 4);
        t->set_color(Color(255,255,255,255));
        t->shadowed = true;
        messages[i] = t;
    }

    input = HudText::text_list->create();
    IF_ASSERT(input == NULL) return;
    input->set_text("");
    input->set_color(AGENT_DEFAULT_COLOR);
    input->set_position(x_offset, line_height*lines_offset);
    input->shadowed = true;

    this->inited = true;
}

void ChatRender::set_cursor(const char* text, float x, float y)
{
    if (HudFont::font == NULL) return;
    if (Chat::chat_client == NULL) return;

    int len = 0;
    int h = 0;

    int s_len = (int)strlen(text);
    char* tmp_text = (char*)malloc(sizeof(char) * (s_len + 1));
    strcpy(tmp_text, text);
    if (Chat::chat_client->input->cursor <= s_len)    // truncate text buffer to cursor position
        tmp_text[Chat::chat_client->input->cursor] = '\0';
    HudFont::font->get_string_pixel_dimension(tmp_text, &len, &h);
    free(tmp_text);
    h = HudFont::font->data.line_height;

    cursor_x = x + len;
    if (s_len && Chat::chat_client->input->cursor == s_len)
        cursor_x += 4;  // margin at the end
    cursor_y = y - h;
    cursor_w = 8;
    cursor_h = h;
}

void ChatRender::draw_cursor()
{
    if (cursor_w_is_growing)
    {
        curr_cursor_w++;

        if (curr_cursor_w > cursor_w)
        {
            curr_cursor_w = cursor_w - 1;
            cursor_w_is_growing = false;
        }
    }
    else
    {  // shrink
        curr_cursor_w--;

        if (curr_cursor_w < MIN_CURSOR_SPAN)
        {
            curr_cursor_w = MIN_CURSOR_SPAN + 1;
            cursor_w_is_growing = true;
        }
    }

    if (cursor_h_is_growing)
    {
        curr_cursor_h++;

        if (curr_cursor_h > cursor_h)
        {
            curr_cursor_h = cursor_h - 1;
            cursor_h_is_growing = false;
        }
    }
    else
    {  // shrink
        curr_cursor_h--;

        if (curr_cursor_h < MIN_CURSOR_SPAN)
        {
            curr_cursor_h = MIN_CURSOR_SPAN + 1;
            cursor_h_is_growing = true;
        }
    }

    // ******** old way (based on player color) ********
    //Color color = AGENT_DEFAULT_COLOR;
    //using ClientState::player_agent;
    //class Agents::Agent* you = player_agent.you();
    //if (you != NULL) color = you->status.color;

    // draw twice for an outline
    int w_mar = 1;  // width margin (how far out from original profile we go)
    int h_mar = 4;  // height
    draw_rect(COLOR_BLACK,
        cursor_x + (cursor_w - curr_cursor_w) / 2 - w_mar,
        cursor_y + (cursor_h - curr_cursor_h) / 2 - h_mar,
        curr_cursor_w + w_mar * 2,
        curr_cursor_h + h_mar * 2);

    w_mar = 0;
    h_mar = 3;
    draw_rect(Color(255,255,255),  // white is probably a contrasting color
        cursor_x + (cursor_w - curr_cursor_w) / 2 - w_mar,
        cursor_y + (cursor_h - curr_cursor_h) / 2 - h_mar,
        curr_cursor_w + w_mar * 2,
        curr_cursor_h + h_mar * 2);
}

void ChatRender::draw_messages()
{
    if (!this->inited) return;
    for (int i=0; i<CHAT_MESSAGE_RENDER_MAX; messages[i++]->draw());
}

void ChatRender::draw_input()
{
    if (!this->inited) return;
    using ClientState::player_agent;
    class Agents::Agent* you = player_agent.you();
    if (you != NULL) this->input->set_color(you->status.color);
    this->input->draw();
}

void ChatRender::update(bool timeout, bool other_players)
{   // read chat client messages and format for display
    if (!this->inited) return;

    if (Chat::chat_message_list == NULL) return;

    class Agents::Agent* you = ClientState::player_agent.you();
    ClientID client_id = NULL_CLIENT;
    if (you != NULL) client_id = you->client_id;

    int now = _GET_MS_TIME();
    Chat::chat_message_list->sort_by_most_recent();
    unsigned int i = paging_offset;
    int j = CHAT_MESSAGE_RENDER_MAX-1;
    int n_draw = 0;
    for (; i<Chat::chat_message_list->n_filtered; i++)
    {
        if (n_draw == CHAT_MESSAGE_RENDER_MAX) break;
        class Chat::ChatMessage* m = Chat::chat_message_list->filtered_objects[i];
        if (m == NULL) break;
        if (timeout && now - m->timestamp > Chat::CHAT_MESSAGE_RENDER_TIMEOUT) break;
        n_draw++;
    }

    j = n_draw;
    i = 0;

    if (!other_players)
    {
        HudText::Text* t = this->messages[i++];
        t->set_text("PLAYER CHAT OFF (use /chaton /chatoff)");
        t->set_color(Color(200,10,10));
    }

    for (;j>0;)
    {
        class Chat::ChatMessage* m = Chat::chat_message_list->filtered_objects[--j];
        if (!other_players && m->sender != Chat::CHAT_SENDER_SYSTEM && m->sender != client_id) continue;

        HudText::Text* t = this->messages[i++];

        if (m->sender == Chat::CHAT_SENDER_SYSTEM)
            t->update_formatted_string(3, "", "", m->payload);
        else
            t->update_formatted_string(3, m->name, CHAT_NAME_DEFAULT_SEPARATOR, m->payload);
        t->set_color(m->color);
    }

    for (int k=i; k<CHAT_MESSAGE_RENDER_MAX; this->messages[k++]->set_text(""));
}

//void ChatRender::page_up()
//{
    //paging_offset += 4;
    //if (paging_offset > chat_message_list->num-8)
        //paging_offset = chat_message_list->num-8;
//}

//void ChatRender::page_down()
//{
    //paging_offset -= 4;
    //if (paging_offset < 0)
        //paging_offset = 0;
//}


ChatRender::ChatRender() :
    curr_cursor_w(0),
    curr_cursor_h(0),
    cursor_w_is_growing(true),
    cursor_h_is_growing(true),
    inited(false),
    input(NULL),
    paging_offset(0),
    cursor_x(0.0f),
    cursor_y(0.0f),
    cursor_w(MIN_CURSOR_SPAN),
    cursor_h(MIN_CURSOR_SPAN)
{
    for (int i=0; i<CHAT_MESSAGE_RENDER_MAX; messages[i++] = NULL);
}

ChatRender::~ChatRender()
{
    for (int i=0; i<CHAT_MESSAGE_RENDER_MAX; i++)
        if (messages[i] != NULL)
            HudText::text_list->destroy(messages[i]->id);

    if (input != NULL) HudText::text_list->destroy(input->id);
}

}   // Hud
