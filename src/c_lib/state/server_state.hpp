/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#pragma once

#if DC_CLIENT
# error Do not include this file in the client
#endif

#include <agent/agent_list.hpp>
#include <voxel/voxel_hitscan.hpp>
#include <particle/grenade.hpp>

namespace ServerState
{

extern bool main_inited;
extern bool signal_exit;
extern bool reload_settings;

extern class Entities::Entity* base;

// property lists
extern Voxels::VoxelHitscanList* voxel_hitscan_list;

void init_lists();
void teardown_voxel_lists();

struct Vec3 get_base_spawn_position();
void init_base();
void check_agents_at_base();

void damage_objects_within_sphere(const Vec3& position, float radius,
                                  int damage, AgentID owner,
                                  EntityType inflictor_type, int inflictor_id,
                                  bool suicidal=true);

void spawn_monsters(EntityType type, int n);

}   // ServerState
