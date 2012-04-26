#include "billboard_sprite.hpp"

#if DC_CLIENT

#include <draw/draw.hpp>
#include <entity/components/physics.hpp>

namespace Components
{

void ColoredVoxelComponent::call()
{
    PhysicsComponent* state = (PhysicsComponent*)this->object->get_component_interface(COMPONENT_INTERFACE_PHYSICS);
    if (state == NULL) return;
    drawColoredMinivox(state->get_position(), this->forward, this->right, this->normal, this->color);
}

} // Components

#endif
