#include "entity.hpp"

#include <c_lib/state/server_state.hpp>
#include <c_lib/particles/textures.hpp>
#include <c_lib/particles/particles.hpp>

#include <c_lib/physics/verlet.hpp>
#include <c_lib/items/packets.hpp>

namespace ItemDrops
{

template <class Super, typename State>
inline void TickParticle<Super, State>::tick(State* state)
{
    Verlet::bounce(state->vp, state->damp);
    Super::tick(state);
}

template <class Super, typename State>
inline void TickTTL<Super, State>::tick(State* state)
{
    state->ttl++;
    Super::tick(state);
}

template <class Super, typename State>
inline void TickPickup<Super, State>::tick(State* state)
{
    #if DC_SERVER
    int agent_id = state->object->nearest_agent_in_range(state->vp->p, state->pickup_radius);
    if (agent_id >= 0 && STATE::agent_list->agent_pickup_item(agent_id, state->type))
    {   // was picked up, die
        state->object->was_picked_up(agent_id);
        state->ttl = state->ttl_max;
    }
    #endif
    Super::tick(state);
}

template <class Super, typename State>
inline void DiePickup<Super, State>::die(State* state)
{
    #if DC_SERVER
    if (state->broadcast_death)
    {
        item_picked_up_StoC msg;
        msg.type = state->type;
        msg.id = state->id;
        msg.agent_id = state->picked_up_by;
        msg.broadcast();
    }
    #endif
    Super::die(state);
}

template <class Super, typename State>
inline void BornPickup<Super, State>::born(State* state)
{
    #if DC_SERVER
    item_create_StoC msg;
    msg.type = state->type;
    msg.id = state->id;
    msg.x = state->vp->p.x;
    msg.y = state->vp->p.y;
    msg.z = state->vp->p.z;
    Vec3 m = state->vp->get_momentum();
    msg.mx = m.x;
    msg.my = m.y;
    msg.mz = m.z;
    msg.broadcast();
    #endif
    Super::born(state);
}


template <class Super, typename State>
inline void DrawBillboardSprite<Super, State>::draw(State* state)
{
    #if DC_CLIENT
    Vec3 v = state->vp->p;
    v.z += state->texture_scale;
    
    if (current_camera == NULL
    || !current_camera->in_view(v.x, v.y, v.z))
        return;

    Vec3 up = vec3_init(
        model_view_matrix[0]*state->texture_scale,
        model_view_matrix[4]*state->texture_scale,
        model_view_matrix[8]*state->texture_scale
    );
    Vec3 right = vec3_init(
        model_view_matrix[1]*state->texture_scale,
        model_view_matrix[5]*state->texture_scale,
        model_view_matrix[9]*state->texture_scale
    );

    float tx_min, tx_max, ty_min, ty_max;
    tx_min = (float)(state->texture_index%16)* (1.0/16.0);
    tx_max = tx_min + (1.0/16.0);
    ty_min = (float)(state->texture_index/16)* (1.0/16.0);
    ty_max = ty_min + (1.0/16.0);

    Vec3 p = vec3_sub(v, vec3_add(right, up));
    glTexCoord2f(tx_min,ty_max);
    glVertex3f(p.x, p.y, p.z);

    p = vec3_add(v, vec3_sub(up, right));
    glTexCoord2f(tx_max,ty_max);
    glVertex3f(p.x, p.y, p.z);

    p = vec3_add(v, vec3_add(up, right));
    glTexCoord2f(tx_max,ty_min);
    glVertex3f(p.x, p.y, p.z);

    p = vec3_add(v, vec3_sub(right, up));
    glTexCoord2f(tx_min,ty_min);
    glVertex3f(p.x, p.y, p.z);
    #endif
    Super::draw(state);
}

void GameObject_list::tick()
{
    if (this->num == 0) return;
    for (int i=0; i<this->n_max; i++)
        if (this->a[i] != NULL)
        {
            this->a[i]->tick();
            if (this->a[i]->state()->ttl >= this->a[i]->state()->ttl_max)
            {
                this->a[i]->die();
                this->destroy(this->a[i]->state()->id);
            }
        }
}

void GameObject_list::draw()
{
    if (this->num == 0) return;
    for (int i=0; i<this->n_max; i++)
        if (this->a[i] != NULL)
            this->a[i]->draw();
}

// TODO: restructure the list creation to adapt based on type
ObjectPolicyInterface* GameObject_list::create(float x, float y, float z, float mx, float my, float mz, Object_types type)
{
    int id = this->get_free_id();
    if (id < 0) return NULL;
    this->num++;
    this->id_c = id+1;

    PickupObject* obj = new PickupObject(id, x,y,z, mx,my,mz);
    this->a[id] = obj;

    float texture_scale, mass, damp;
    int texture_index, ttl_max;
    
    obj->state()->type = type;
    switch (type)
    {   // TODO: THIS WILL BE REPLACED BY ObjectType
        case OBJ_TYPE_GRENADE_REFILL:
            texture_index = GRENADE_REFILL_TEXTURE_ID;
            texture_scale = GRENADE_REFILL_TEXTURE_SCALE;
            mass = GRENADE_REFILL_MASS;
            ttl_max = GRENADE_REFILL_TTL;
            damp = GRENADE_REFILL_DAMP;
            break;
        case OBJ_TYPE_LASER_REFILL:
            texture_index = LASER_REFILL_TEXTURE_ID;
            texture_scale = LASER_REFILL_TEXTURE_SCALE;
            mass = LASER_REFILL_MASS;
            ttl_max = LASER_REFILL_TTL;
            damp = LASER_REFILL_DAMP;
            break;
        default:
            printf("WARNING: %s create() -- unhandled object type %d\n", name(), type);
            return NULL;
    };
    
    obj->state()->texture_index = texture_index;
    obj->state()->texture_scale = texture_scale;
    obj->state()->mass = mass;
    obj->state()->ttl_max = ttl_max;
    obj->state()->damp = damp;
    obj->born();

    return obj;
}

void GameObject_list::destroy(int id)
{
    ObjectPolicyInterface* obj = this->a[id];
    if (obj == NULL) return;
    obj->die();
    Object_list<ObjectPolicyInterface, GAME_OBJECTS_MAX>::destroy(id);
}

} // ItemDrops
