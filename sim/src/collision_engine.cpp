#include <algorithm>
#include <utility>
#include <string>
#include <iostream>

#include "rigid_body_component.hpp"
#include "position_component.hpp"

#include "collision_engine.hpp"

CollisionEngine::CollisionEngine(void){}

CollisionEngine::~CollisionEngine(void){}

void CollisionEngine::update(void) {

    Position pos;

    std::vector<pos_bod_pair_t>::iterator it;
    for(it = pos_bod_vect.begin();
            it != pos_bod_vect.end();
            ++it)
    {
          std::cout << "calculate collision\n";
    }
}

CollisionEngine::operator std::string() const {}

void CollisionEngine::entity_added(Entity* entity) {

    PositionComponent* pos_ptr = entity->component<PositionComponent>();
    RigidBodyComponent* bod_ptr = entity->component<RigidBodyComponent>();

    if (( pos_ptr != NULL ) &&
            (bod_ptr != NULL))
    {
        pos_bod_pair_t pm = std::make_pair(pos_ptr, bod_ptr);
        pos_bod_vect.push_back(pm);
    }
}
