#ifndef RENDER_ENGINE_CLASS_HPP
#define RENDER_ENGINE_CLASS_HPP

#include <algorithm>
#include <utility>
#include <string>
#include "simulator/render_component.hpp"

class RenderEngine : public Engine {

    std::vector< RenderComponent > components;

    public:
        RenderEngine(void){}
        ~RenderEngine(void){}

        virtual void update(void) {
        }

        virtual operator std::string() const {}

        virtual void entity_added(Entity* entity) {

            RenderComponent* render_ptr = entity->component<RenderComponent>();

            if ( render_ptr != NULL ) &&
            {
                pos_mov_vect.push_back(pm);
            }
        }


};

#endif /* RENDER_ENGINE_CLASS_HPP */
