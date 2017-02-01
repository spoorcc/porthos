
#ifndef FACTORY_H
#define FACTORY_H

#include "simulator/entity.hpp"
#include "simulator/move_component.hpp"
#include "simulator/position_component.hpp"

static Entity* factory_create_entity(void)
{
     Entity* entity = new Entity();

     entity->add_component<MoveComponent>();
     entity->add_component<PositionComponent>();

     return entity;
}

#endif /* FACTORY_H */
