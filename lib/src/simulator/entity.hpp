#include <vector>
#include "simulator/component.hpp"

class Entity {

    public:
        Entity(unsigned long int ID){this->ID = ID;}
        ~Entity(void){}

        void add_component(Component &component) {this->components.push_back(component);}

    private:
       unsigned long int ID;
       std::vector<Component> components;
};

