#ifndef RIGID_BODY_COMPONENT_H
#define RIGID_BODY_COMPONENT_H

#include <vector>

#include "component.hpp"
#include "datatypes.hpp"

class RigidBodyComponent : public Component {

    std::vector<xy_pos_t> outline;

    public:
        RigidBodyComponent() : outline(){}
        ~RigidBodyComponent(void){}

        std::vector<xy_pos_t> & get_outline(void){return this->outline;}
};

#endif /* RIGID_BODY_COMPONENT_H */
