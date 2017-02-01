#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

#include "simulator/component.hpp"

struct Position {
    float x;
    float y;
};

class PositionComponent : public Component {

    Position position;

    public:
        PositionComponent() : position{0.0, 0.0} {}
        ~PositionComponent(void){}

        Position& get_position(void){return this->position;}
        void set_position(const Position& position){this->position = position;}
};

#endif /* POSITION_COMPONENT_H */
