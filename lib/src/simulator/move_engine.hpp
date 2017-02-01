#ifndef MOVE_ENGINE_CLASS_HPP
#define MOVE_ENGINE_CLASS_HPP

#include <algorithm>
#include <utility>
#include <string>
#include "simulator/move_component.hpp"
#include "simulator/position_component.hpp"

class MoveEngine : public Engine {

    typedef std::pair<PositionComponent*, MoveComponent*> pos_mov_pair_t;
    std::vector< pos_mov_pair_t > pos_mov_vect;

    public:
        MoveEngine(void){}
        ~MoveEngine(void){}

        virtual void update(void) {

             float speed;
             Position pos;

             std::vector<pos_mov_pair_t>::iterator it;
             for(it = pos_mov_vect.begin();
                 it != pos_mov_vect.end();
                 ++it)
             {
                 speed = (*it).second->get_speed();

                 pos = (*it).first->get_position();

                 pos.x += speed;
                 pos.y += speed;

                 std::cout << "x: " << pos.x << ", "\
                           << "y: " << pos.y << std::endl;
             }

        }

        virtual operator std::string() const {}

        virtual void entity_added(Entity* entity) {

            PositionComponent* pos_ptr = entity->component<PositionComponent>();
            MoveComponent* mov_ptr = entity->component<MoveComponent>();

            if (( pos_ptr != NULL ) &&
                (mov_ptr != NULL))
            {
                pos_mov_pair_t pm = std::make_pair(pos_ptr, mov_ptr);
                pos_mov_vect.push_back(pm);
            }
        }


};

#endif /* MOVE_ENGINE_CLASS_HPP */
