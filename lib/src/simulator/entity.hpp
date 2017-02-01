#ifndef ENTITY_CLASS_HPP
#define ENTITY_CLASS_HPP

#include <map>
#include <typeinfo>
#include "simulator/component.hpp"

class Entity {

    typedef std::map<const std::string, Component *> component_map_t;
    typedef std::pair<const std::string, Component *> component_pair_t;

    component_map_t components;

    public:
        Entity(){
            static unsigned long int ID = 0;
            this->ID = ID++;
        }
        ~Entity(void){}

        unsigned long int ID;

        template <typename T>
        void add_component() {
             components[typeid(T).name()] = new T;
        }

        template <typename T>
        T* component() {

             T* comp_ptr = (T*) components[typeid(T).name()];

             std::cout << typeid(T).name() << " : " << comp_ptr << std::endl;
             return comp_ptr;
        }
};

#endif /* ENTITY_CLASS_HPP */
