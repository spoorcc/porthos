
#ifndef COMPONENT_H
#define COMPONENT_H

#include <typeinfo>

class Component {

    public:
        Component(){}
        ~Component(void){}

        bool is_of_type(const std::type_info& type) {
             return typeid(*this) == type;
         }

        const std::string type_name(void) {
             return typeid(*this).name();
         }
};

#endif /* COMPONENT_H */
