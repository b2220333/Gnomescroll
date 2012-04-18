#pragma once

// todo - merge subtypes

typedef enum
{
    OBJECT_NONE,

    OBJECT_AGENT,

    // mobs
    OBJECT_SLIME,
    OBJECT_MONSTER_BOX,
    OBJECT_MONSTER_SPAWNER,

    OBJECT_GRENADE,

    OBJECT_FLAG,
    OBJECT_BASE,

    OBJECT_SPAWNER,
    OBJECT_TURRET,

    OBJECT_HEALTH_REFILL,
    OBJECT_GRENADE_REFILL,
    OBJECT_LASER_REFILL,

    OBJECT_GEMSTONE_MALACHITE,
    OBJECT_GEMSTONE_RUBY,
    OBJECT_GEMSTONE_TURQUOISE,
    OBJECT_GEMSTONE_SILVER,
    OBJECT_GEMSTONE_AMETHYST,
    OBJECT_GEMSTONE_JADE,
    OBJECT_GEMSTONE_ONYX,

    OBJECT_DIRT_BLOCK_DROP,
    OBJECT_STONE_BLOCK_DROP,
    OBJECT_SOFT_ROCK_BLOCK_DROP,
    OBJECT_MEDIUM_ROCK_BLOCK_DROP,
    OBJECT_HARD_ROCK_BLOCK_DROP,
    OBJECT_INFECTED_ROCK_BLOCK_DROP,

    OBJECT_MEAT,

    OBJECT_INVENTORY,
    
} ObjectType;

typedef enum
{
    COMPONENT_NONE,

    COMPONENT_POSITION, // todo -- tag with _PHYSICS
    COMPONENT_POSITION_MOMENTUM,
    COMPONENT_VERLET,
    COMPONENT_STACKABLE,
    COMPONENT_PICKUP,
    COMPONENT_TTL,
    COMPONENT_HIT_POINTS,

    COMPONENT_BILLBOARD_SPRITE,
    COMPONENT_COLORED_VOXEL,
    COMPONENT_TEXTURED_VOXEL,
    
} ComponentType;

typedef enum
{
    COMPONENT_INTERFACE_NONE,
    COMPONENT_INTERFACE_PHYSICS,    // physics state setters/getters
    COMPONENT_INTERFACE_STACKABLE,  // stackable setters/getters
    COMPONENT_INTERFACE_DRAW,       // draw()
    COMPONENT_INTERFACE_UPDATE,     // update()
    COMPONENT_INTERFACE_TICK,       // tick()
    COMPONENT_INTERFACE_PICKUP,
    COMPONENT_INTERFACE_HEALTH,     // is_dead()
} ComponentInterfaceType;

//const Vec3 NULL_MOMENTUM = { 0,0,0 };
