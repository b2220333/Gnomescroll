#pragma once

namespace t_hud
{

// UI State
extern class AgentContainerUI* agent_container;
extern class AgentToolbeltUI* agent_toolbelt;
// TODO -- TMP -- replace witha ctual types
extern class AgentNaniteUI* nanite_container;
extern class CraftingUI* crafting_container;

void set_container_id(ItemContainerType container_type, int container_id);

extern float mouse_x;
extern float mouse_y;

// Input Handling
typedef struct
{
    int container_id;
    int slot;
    bool nanite;
    bool craft_output;
} ContainerInputEvent;

void enable_agent_container_hud();
void disable_agent_container_hud();
void enable_block_container_hud();
void disable_block_container_hud();

void close_container(int container_id);

ContainerInputEvent left_mouse_down(int x, int y);
ContainerInputEvent left_mouse_up(int x, int y);
ContainerInputEvent right_mouse_down(int x, int y);
ContainerInputEvent right_mouse_up(int x, int y);
ContainerInputEvent mouse_motion(int x, int y);
ContainerInputEvent scroll_up();
ContainerInputEvent scroll_down();
ContainerInputEvent select_slot(int numkey);
ContainerInputEvent null_input_event();

// network
void network_container_assignment(ObjectType type, int id);

// Init
void init();
void teardown();

void draw_init();
void draw_teardown();

// render
void draw_hud();

}
