#include <algorithm>
#include <utility>
#include <string>

#include "move_component.hpp"
#include "position_component.hpp"

#include "move_engine.hpp"

MoveEngine::MoveEngine(void){}

MoveEngine::~MoveEngine(void){}

void MoveEngine::update(void) {

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
    }
}

MoveEngine::operator std::string() const {}

void MoveEngine::entity_added(Entity* entity) {

    PositionComponent* pos_ptr = entity->component<PositionComponent>();
    MoveComponent* mov_ptr = entity->component<MoveComponent>();

    if (( pos_ptr != NULL ) &&
            (mov_ptr != NULL))
    {
        pos_mov_pair_t pm = std::make_pair(pos_ptr, mov_ptr);
        pos_mov_vect.push_back(pm);
    }
}
