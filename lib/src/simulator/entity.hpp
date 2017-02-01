#ifndef ENTITY_CLASS_HPP
#define ENTITY_CLASS_HPP

#include <vector>
#include <typeinfo>
#include "simulator/component.hpp"

class Entity {

    unsigned long int ID;
    std::vector<Component *> components;

    public:
        Entity(unsigned long int ID){this->ID = ID;}
        ~Entity(void){}

        void add_component(Component *component) {
             this->components.push_back(component);
        }
        bool has_component(const std::type_info& component_type ) {

             for( std::vector<Component*>::iterator it = components.begin();
                  it != components.end();
                  ++it)
             {
                  if ((*it)->is_of_type(component_type))
                      return true;
             }
             return false;
        }

};

#endif /* ENTITY_CLASS_HPP */
