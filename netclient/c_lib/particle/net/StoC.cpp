#include "StoC.hpp"

#if DC_CLIENT
#include <particle/_interface.hpp>
//#include <t_hud/_interface.hpp>

#include <item/_interface.hpp>
#include <item/data/constant.hpp>

#endif

namespace Particle
{


#if DC_CLIENT

inline void item_particle_create_StoC::handle()
{
    printf("item_particle: creating %d \n", id);

    Item::create_item_particle(item_type, (ItemID)item_id, x,y,z,mx,my,mz);
}

inline void item_particle_destroy_StoC::handle()
{
    printf("item_particle: destroying %d \n", id);
    // destroy item via list
    Particle::item_particle_list->destroy(id);
}


inline void item_particle_picked_up_StoC::handle()
{
    printf("item_particle: %d picked up by agent %d \n", id, agent_id);
    /*
        Item Pickup Sound
    */
    Particle::item_particle_list->destroy(id);
}

#endif

#if DC_SERVER

inline void item_particle_create_StoC::handle() {}
inline void item_particle_destroy_StoC::handle() {}
inline void item_particle_picked_up_StoC::handle() {}

#endif

}
