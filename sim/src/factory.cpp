#include "datatypes.hpp"
#include "entity.hpp"
#include "move_component.hpp"
#include "position_component.hpp"
#include "rigid_body_component.hpp"

#include "factory.hpp"

#ifdef PORTHOS_WITH_GUI
#include "render_component.hpp"
#endif /* PORTHOS_WITH_GUI */

Entity* factory_create_wall( const xy_vec_t  pos,
                             const xy_vec_t size )
{
     Entity* entity = new Entity();

     entity->add_component<PositionComponent>();
     entity->add_component<RigidBodyComponent>();

     #ifdef PORTHOS_WITH_GUI
     entity->add_component<RenderComponent>();
     #endif /* PORTHOS_WITH_GUI */

     return entity;
}

Entity* factory_create_robot(void)
{
     Entity* entity = new Entity();

     entity->add_component<MoveComponent>();
     entity->add_component<PositionComponent>();
     entity->add_component<RigidBodyComponent>();

     #ifdef PORTHOS_WITH_GUI
     entity->add_component<RenderComponent>();
     #endif /* PORTHOS_WITH_GUI */

     return entity;
}
