#include "packets.hpp"

#if DC_CLIENT
inline void item_create_StoC::handle()
{
    switch (type)
    {
        case OBJ_TYPE_GRENADE_REFILL:
        case OBJ_TYPE_LASER_REFILL:
            ClientState::object_list->create(x,y,z, mx,my,mz, (Object_types)type);
            break;
            
        default: return;
    }
}

// use privately
static inline void _destroy_item_handler(int type, int id) __attribute((always_inline));
static inline void _destroy_item_handler(int type, int id)
{
    switch (type)
    {
        case OBJ_TYPE_GRENADE_REFILL:
        case OBJ_TYPE_LASER_REFILL:
            ClientState::object_list->destroy(id);
            break;

        default: return;
    }    
}

inline void item_destroy_StoC::handle()
{
    _destroy_item_handler(type, id);
}

inline void item_picked_up_StoC::handle()
{
    using ClientState::playerAgent_state;
    if (playerAgent_state.you != NULL && playerAgent_state.you->id == agent_id)
        Sound::pickup_item();
    _destroy_item_handler(type, id);
}
#endif

#if DC_SERVER
inline void item_create_StoC::handle() {}
inline void item_destroy_StoC::handle() {}
inline void item_picked_up_StoC::handle() {}
#endif
