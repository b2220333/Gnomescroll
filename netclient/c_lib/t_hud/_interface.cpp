#include "_interface.hpp"

#include <c_lib/t_hud/inventory_hud.hpp>
#include <c_lib/t_hud/tool_belt_hud.hpp>

namespace t_hud
{

/*
    Init
*/
void draw_init()
{
	init_texture();
}

void draw_teardown()
{

}
/*
    Input Handling
*/


void enable_inventory_hud()
{
	printf("t_item::enable_inventory_hud \n");
}

void disable_inventory_hud()
{
	printf("t_item::disable_inventory_hud \n");
}

void mouse_motion(int x, int y, int dx, int dy)
{

}

void left_mouse_down(int x, int y)
{
}

void handle_right_mouse_click(int x, int y)
{
}

void left_mouse_up(int x, int y)
{

}

void right_mouse_down(int x, int y)
{

}

void right_mouse_up(int x, int y)
{
	
}

/*
    Drawing
*/
void draw_hud()
{
    if (!input_state.inventory) return;

    draw_inventory_hud();
    draw_toolbelt_hud();
    //static ItemGrid g;
    //g.draw(300,300);
}


}
