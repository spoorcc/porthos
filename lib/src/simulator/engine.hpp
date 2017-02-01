#ifndef ENGINE_CLASS_HPP
#define ENGINE_CLASS_HPP

#include <string>

class Engine {

    public:
        Engine(void){}
        ~Engine(void){}

        virtual void update(void) = 0;

        virtual operator std::string() const = 0;

        virtual void entity_added(Entity& entity) = 0;
};

#endif /* ENGINE_CLASS_HPP */
