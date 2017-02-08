#include <algorithm>
#include <utility>
#include <string>

#include "collision_component.hpp"
#include "position_component.hpp"

#include "collision_engine.hpp"

CollisionEngine::CollisionEngine(void){}

CollisionEngine::~CollisionEngine(void){}

void CollisionEngine::update(void) {

    float speed;
    Position pos;

    std::vector<pos_col_pair_t>::iterator it;
    for(it = pos_col_vect.begin();
            it != pos_col_vect.end();
            ++it)
    {
        speed = (*it).second->get_speed();

        pos = (*it).first->get_position();

        pos.x += speed;
        pos.y += speed;
    }
}

CollisionEngine::operator std::string() const {}

void CollisionEngine::entity_added(Entity* entity) {

    PositionComponent* pos_ptr = entity->component<PositionComponent>();
    CollisionComponent* col_ptr = entity->component<CollisionComponent>();

    if (( pos_ptr != NULL ) &&
            (col_ptr != NULL))
    {
        pos_col_pair_t pm = std::make_pair(pos_ptr, col_ptr);
        pos_col_vect.push_back(pm);
    }
}
