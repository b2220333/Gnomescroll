#include "object.hpp"

#include <c_lib/entity/constants.hpp>
#include <c_lib/entity/component/component.hpp>
#include <c_lib/entity/network/packets.hpp>
#include <c_lib/entity/components/draw/billboard_sprite.hpp>
#include <c_lib/entity/components/pickup.hpp>
#include <c_lib/entity/components/health/ttl.hpp>

namespace Objects
{

using Components::Component;

void Object::add_component(int slot, Component* component)
{
    assert(slot >= 0);
    assert(slot < this->n_components);
    assert(this->components[slot] == NULL);
    this->components[slot] = component;
}

// returns component of type if available
// must cast to desired component manually
Component* Object::get_component(ComponentType type)
{
    int slot = object_data->get_component_slot(this->type, type);
    if (slot < 0) return NULL;
    assert(slot < this->n_components);
    return this->components[slot];
}

Component* Object::get_component_interface(ComponentInterfaceType interface)
{
    int slot = object_data->get_component_interface_slot(this->type, interface);
    if (slot < 0) return NULL;
    assert(slot < this->n_components);
    return this->components[slot];
}

void Object::broadcastDeath()
{
    object_destroy_StoC msg;
    msg.id = this->id;
    msg.type = this->type;
    msg.broadcast();
}

void Object::init(int n_components)
{
    assert(this->components == NULL);
    assert(this->n_components == 0);
    this->components = (Component**)calloc(n_components, sizeof(Component*));
    this->n_components = n_components;
}

Object::~Object()
{
    if (this->components != NULL) free(this->components);
}

} // Objects
