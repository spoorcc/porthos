#ifndef SIMULATOR_CLASS_HPP
#define SIMULATOR_CLASS_HPP

#include <functional>
#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

#include "simulator/entity.hpp"
#include "simulator/engine.hpp"
#include "simulator/move_engine.hpp"

class Simulator {

    std::vector<Engine *> engines;
    std::vector<Entity *> entities;

    public:
        Simulator(void){
            add_engine(new MoveEngine);
        }
        ~Simulator(void){

            std::for_each(engines.begin(),
                          engines.end(),
                          std::default_delete<Engine>());
        }

        void add_entity(Entity * entity) {
             entities.push_back(entity);

             for(std::vector<Engine*>::iterator it = engines.begin();
                 it != engines.end();
                 ++it)
             {
                 (*it)->entity_added(*entity);
             }
        }

        operator std::string() const {
            std::stringstream output;

            output << "Simulator:\n" \
                   << "\tEntities: " << this->entities.size() \
                   << std::endl;

            return output.str();
        };

    private:

       void add_engine(Engine * engine)
       {
             this->engines.push_back(engine);
       }


};

#endif /* SIMULATOR_CLASS_HPP */
