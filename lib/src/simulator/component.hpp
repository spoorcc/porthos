
#ifndef COMPONENT_H
#define COMPONENT_H

class Component {

    public:
        Component(){}
        ~Component(void){}

        virtual bool is_of_type(const std::type_info& type) {
             return typeid(*this) == type;
         }
};

#endif /* COMPONENT_H */
