#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "component.hpp"
#include "datatypes.hpp"

class CollisionComponent : public Component {

    std::vector< xy_pos_t > bounding_box;

    public:
        CollisionComponent() : speed(0.0){}
        ~CollisionComponent(void){}

        std::vector<xyz_pos_t>& get_boundingbox(void){return this->bounding_box;}
};

#endif /* COLLISION_COMPONENT_H */
