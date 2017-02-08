#ifndef SIMULATOR_CLASS_HPP
#define SIMULATOR_CLASS_HPP

#include <functional>
#include <algorithm>
#include <vector>
#include <memory>
#include <sstream>
#include <string>

#include "entity.hpp"
#include "engine.hpp"
#include "move_engine.hpp"
#include "collision_engine.hpp"

#ifdef PORTHOS_WITH_GUI
#include "render_engine.hpp"
#endif /* PORTHOS_WITH_GUI */ 

class Simulator {

    std::vector<Engine *> engines;
    std::vector<Entity *> entities;

    public:
        Simulator(void){
            add_engine(new MoveEngine);
            add_engine(new CollisionEngine);

            #ifdef PORTHOS_WITH_GUI
            add_engine(new RenderEngine);
            #endif /* PORTHOS_WITH_GUI */
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
                 (*it)->entity_added(entity);
             }
        }

        operator std::string() const {
            std::stringstream output;

            output << "Simulator:\n" \
                   << "\tEntities: " << this->entities.size() \
                   << std::endl;

            return output.str();
        };

       void update()
       {
             for(std::vector<Engine*>::iterator it = engines.begin();
                 it != engines.end();
                 ++it)
             {
                 (*it)->update();
             }
       }

    private:

       void add_engine(Engine * engine)
       {
             this->engines.push_back(engine);
       }


};

#endif /* SIMULATOR_CLASS_HPP */
