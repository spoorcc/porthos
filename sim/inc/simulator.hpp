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
#include "ray_trace_engine.hpp"

#ifdef PORTHOS_WITH_GUI
#include "render_engine.hpp"
#endif /* PORTHOS_WITH_GUI */ 

class Simulator {

    std::vector<std::unique_ptr<Engine> > engines;
    std::vector<Entity *> entities;

    public:
        Simulator(void){
            add_engine(std::unique_ptr<Engine>{ std::make_unique<MoveEngine>() } );
            add_engine(std::unique_ptr<Engine>{ std::make_unique<CollisionEngine>() } );
            add_engine(std::unique_ptr<Engine>{ std::make_unique<RayTraceEngine>() } );

            #ifdef PORTHOS_WITH_GUI
            add_engine(std::unique_ptr<Engine>{ std::make_unique<RenderEngine>() } );
            #endif /* PORTHOS_WITH_GUI */
        }
        ~Simulator(void){
        }

        void add_entity(Entity * entity) {
             entities.push_back(entity);

             for( auto& engine: engines)
             {
                 engine->entity_added(entity);
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
             for( auto& engine: engines)
             {
                 engine->update();
             }
       }

    private:

       void add_engine(std::unique_ptr<Engine> engine)
       {
             engines.push_back(std::move(engine));
       }


};

#endif /* SIMULATOR_CLASS_HPP */
