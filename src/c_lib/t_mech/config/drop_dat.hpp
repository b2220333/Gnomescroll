#pragma once

#if DC_CLIENT
# error Don't include this file in the client
#endif

#include <item/config/item_drop_alt.hpp>

namespace t_mech
{

extern class Item::ItemDropConfig* drop_dat;

void init_drop_dat();
void teardown_drop_dat();
void load_drop_dat();

void handle_drop(int x, int y, int z, int type);

}   // t_mech
