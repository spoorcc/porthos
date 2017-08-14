#ifndef RENDER_COMPONENT_HPP
#define RENDER_COMPONENT_HPP

#include "component.hpp"

struct xyz_rgb_t {
    float x, y, z;
    float r, g, b;
};

class RenderComponent : public Component {

    public:
        RenderComponent() {}
        ~RenderComponent(void){}

        std::vector<xyz_rgb_t> vertices;
        std::vector<unsigned int> elements;

};

#endif /* RENDER_COMPONENT_HPP */
