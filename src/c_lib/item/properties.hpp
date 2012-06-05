#pragma once

#include <item/common/constant.hpp>

namespace Item
{

extern int sprite_array[MAX_ITEMS];
extern int type_array[MAX_ITEMS];
extern int group_array[MAX_ITEMS];
extern class ItemAttribute* item_attribute_array;

extern class NaniteStoreItem* nanite_store_item_array;

extern class NaniteStoreItem* nanite_store_item_array;
extern class CraftingRecipe* crafting_recipe_array;

// buffers for condensing craft bench inputs to unique type,count pairs
extern int craft_input_types[CRAFT_BENCH_INPUTS_MAX];
extern int craft_input_totals[CRAFT_BENCH_INPUTS_MAX];

// buffers for recipe outputs available
extern class CraftingRecipe* craft_recipes_possible[CRAFT_BENCH_OUTPUTS_MAX];
extern int craft_recipes_possible_count;

void init_properties();

class ItemAttribute* get_item_attributes(int item_type);
int get_item_fire_rate(int item_type);

int get_sprite_index_for_id(ItemID id);
int get_sprite_index_for_type(int type);

void set_item_name(int type, char* name, int length);
void set_item_name(int type, char* name);

char* get_item_name(int item_type);
int get_item_type(const char* name);

char* get_item_pretty_name(int type);

int get_item_group_for_type(int item_type);

int dat_get_item_type(const char* name);

bool item_type_is_voxel(int item_type);

int get_max_stack_size(int item_type);
int get_max_energy(int item_type);
int get_max_durability(int item_type);

int get_placer_block_type_id(int item_type);
int get_particle_voxel_texture(int item_type);

float get_weapon_range(int weapon_type);
int get_item_block_damage(int weapon_type, int block_type);
int get_item_object_damage(int weapon_type);

bool get_nanite_edibility(int item_type);

int get_nanite_store_item(int level, int xslot, int yslot);
int get_nanite_store_item(int level, int xslot, int yslot, int* cost);

class CraftingRecipe* get_craft_recipe(int recipe_id);

class CraftingRecipe* get_selected_craft_recipe(int container_id, int slot);
int get_selected_craft_recipe_type(int container_id, int slot);
int get_selected_craft_recipe_type(int container_id, int slot, bool* available);
int get_selected_craft_recipe_stack(int container_id, int slot);

bool container_type_is_block(ItemContainerType type);

}   // Item