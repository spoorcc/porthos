

#ifndef MOVE_COMPONENT_H
#define MOVE_COMPONENT_H

#include "simulator/component.hpp"

class MoveComponent : public Component {

    float speed;

    public:
        MoveComponent() : speed(0.0){}
        ~MoveComponent(void){}

        float& get_speed(void){return this->speed;}
        void set_speed(const float& speed){this->speed = speed;}
};

#endif /* MOVE_COMPONENT_H */
