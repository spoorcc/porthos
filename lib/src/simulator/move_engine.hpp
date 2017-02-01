#ifndef MOVE_ENGINE_CLASS_HPP
#define MOVE_ENGINE_CLASS_HPP

#include <algorithm>
#include <string>
#include "simulator/move_component.hpp"

class MoveEngine : public Engine {

    std::vector<MoveComponent> components;

    public:
        MoveEngine(void){}
        ~MoveEngine(void){}

        virtual void update(void) {}

        virtual operator std::string() const {}

        virtual void entity_added(Entity& entity) {
            std::cout << "New entity!" << std::endl;

            if (entity.has_component(typeid(MoveComponent)))
                std::cout << "Has Move component" << std::endl;
        }


};

#endif /* MOVE_ENGINE_CLASS_HPP */
