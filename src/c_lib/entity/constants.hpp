#pragma once

namespace Entities
{   // forward decl
class Entity;
}   // Entities

typedef void (*tickObject)(class Entities::Entity*);      // for physics
typedef void (*updateObject)(class Entities::Entity*);    // for draw prep

int MAX_OBJECT_TYPES = 0xFF;
typedef enum
{
    OBJECT_NONE = 0,    // null
    OBJECT_DESTINATION,     // abstract object

    OBJECT_AGENT,   // agent

    // projectiles
    OBJECT_GRENADE,

    // game items
    OBJECT_BASE,

    // fabs
    OBJECT_AGENT_SPAWNER,
    OBJECT_TURRET,
    OBJECT_ENERGY_CORE,

    // mobs
    OBJECT_MONSTER_BOMB,
    OBJECT_MONSTER_BOX,
    OBJECT_MONSTER_SPAWNER,

    // misc
    OBJECT_PLASMAGEN,
    OBJECT_CANNONBALL,

} EntityType;

int MAX_COMPONENT_TYPES = 0xFF;

typedef enum
{
    COMPONENT_NONE = 0,

    COMPONENT_POSITION,
    COMPONENT_POSITION_MOMENTUM,
    COMPONENT_VERLET,

    COMPONENT_TTL,
    COMPONENT_HIT_POINTS,

    COMPONENT_VOXEL_MODEL,

    COMPONENT_OWNER,

    COMPONENT_AGENT_SPAWNER,
    COMPONENT_MONSTER_SPAWNER,

    COMPONENT_SPAWN_CHILD,

    COMPONENT_MOTION_TARGETING,
    COMPONENT_WEAPON_TARGETING,
    COMPONENT_DESTINATION_TARGETING,
    COMPONENT_AGENT_TARGETING,

    COMPONENT_STATE_MACHINE,
    COMPONENT_WAITING,

    COMPONENT_DIMENSION,

    COMPONENT_RATE_LIMIT,

    COMPONENT_HEALER,

    #if DC_CLIENT
    COMPONENT_VOXEL_ANIMATION,
    #endif

    #if DC_SERVER
    COMPONENT_EXPLOSION,
    COMPONENT_ITEM_DROP,
    #endif

} ComponentType;

int MAX_COMPONENT_INTERFACE_TYPES = 0xFF;
typedef enum
{
    COMPONENT_INTERFACE_NONE = 0,
    COMPONENT_INTERFACE_PHYSICS,    // physics state setters/getters
    COMPONENT_INTERFACE_STACKABLE,  // stackable setters/getters
    COMPONENT_INTERFACE_UPDATE,     // update()
    COMPONENT_INTERFACE_TICK,       // tick()
    COMPONENT_INTERFACE_PICKUP,
    COMPONENT_INTERFACE_HEALTH,     // is_dead()
    COMPONENT_INTERFACE_OWNER,      // owner getter/setter
    COMPONENT_INTERFACE_VOXEL_MODEL,
    COMPONENT_INTERFACE_SPAWNER,
    COMPONENT_INTERFACE_SPAWN_CHILD,
    COMPONENT_INTERFACE_TARGETING,
    COMPONENT_INTERFACE_DIMENSION,
    COMPONENT_INTERFACE_RATE_LIMIT,
    COMPONENT_INTERFACE_HEALING,
    COMPONENT_INTERFACE_STATE_MACHINE,
    COMPONENT_INTERFACE_WAITING,

    #if DC_CLIENT
    COMPONENT_INTERFACE_DRAW,       // draw()
    COMPONENT_INTERFACE_ANIMATION,
    #endif

    #if DC_SERVER
    COMPONENT_INTERFACE_EXPLOSION,
    COMPONENT_INTERFACE_ITEM_DROP,
    #endif
} ComponentInterfaceType;

typedef enum
{
    STATE_NONE = 0,
    STATE_WAITING,
    STATE_IN_TRANSIT,
    STATE_CHASE_AGENT,
} EntityState;

typedef void (*stateRouter) (class Entities::Entity*, EntityState);   // for state machine

#include <limits.h>
#include <physics/vec3.hpp>

//const int MOB_BROADCAST_RATE = ONE_SECOND / 10;
const int MOB_BROADCAST_RATE = ONE_SECOND / 5;

const int NULL_ENTITY_ID = -1;      // cant use a positive number because it imposes constraint on all object list's max. dont send this over the network
const int NULL_OWNER = INT_MAX; // owners are ids of agents. they will never be INT_MAX
const int NULL_HEALTH = INT_MAX;
const Vec3 NULL_POSITION = vec3_init(0,0,0);
const Vec3 NULL_MOMENTUM = vec3_init(0,0,0);
const Vec3 NULL_ANGLES = vec3_init(0,0,0);
const float NULL_HEIGHT = 1.0f;

const int MAX_SPAWNERS = 1024;
const int BASE_SPAWN_ID = 1024;
