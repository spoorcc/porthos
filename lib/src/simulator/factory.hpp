
#ifndef FACTORY_H
#define FACTORY_H

#include "simulator/entity.hpp"
#include "simulator/move_component.hpp"
#include "simulator/position_component.hpp"

#ifdef PORTHOS_WITH_GUI
#include "simulator/render_component.hpp"
#endif /* PORTHOS_WITH_GUI */

static Entity* factory_create_entity(void)
{
     Entity* entity = new Entity();

     entity->add_component<MoveComponent>();
     entity->add_component<PositionComponent>();

     #ifdef PORTHOS_WITH_GUI
     entity->add_component<RenderComponent>();
     #endif /* PORTHOS_WITH_GUI */

     return entity;
}

#endif /* FACTORY_H */
