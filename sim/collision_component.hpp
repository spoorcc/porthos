#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include "component.hpp"
#include "datatypes.hpp"

class CollisionComponent : public Component {

    std::vector<

    public:
        CollisionComponent() : speed(0.0){}
        ~CollisionComponent(void){}

        float& get_speed(void){return this->speed;}
        void set_speed(const float& speed){this->speed = speed;}
};

#endif /* COLLISION_COMPONENT_H */
