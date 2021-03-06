/* Gnomescroll, Copyright (c) 2013 Symbolic Analytics
 * Licensed under GPLv3 */
#pragma once

namespace ItemParticle
{

extern class ItemParticle_list* item_particle_list;

void init();
void teardown();
void init_packets();

void tick();
void destroy(ItemParticleID particle_id);

class ItemParticle* get(ItemParticleID particle_id);

#if DC_CLIENT
void draw();

class ItemParticle* create_item_particle(ItemParticleID particle_id, ItemType item_type,
                                         const Vec3& position, const Vec3& velocity);
#endif

#if DC_SERVER
class ItemParticle* create_item_particle(ItemID item_id, ItemType item_type,
                                         const Vec3& position, const Vec3& velocity);

// create Item and ItemParticle
class Item::Item* create_item_particle(ItemType item_type, const Vec3& position,
                                       const Vec3& velocity);

void broadcast_particle_item_create(ItemParticleID particle_id);
void send_particle_items_to_client(ClientID client_id);
void broadcast_particle_item_state(ItemParticleID particle_id);
void broadcast_particle_item_destroy(ItemParticleID particle_id);
void send_particle_item_picked_up(ClientID client_id, AgentID agent_id, ItemParticleID particle_id);
void broadcast_particle_item_picked_up(AgentID agent_id, ItemParticleID particle_id);
void broadcast_particle_item_pickup_cancelled(ItemParticleID particle_id);

void check_item_pickups();
void throw_agent_item(AgentID agent_id, ItemID item_id);
void dump_container_item(ItemID item_id, const Vec3& position);
#endif

}   // ItemParticle
