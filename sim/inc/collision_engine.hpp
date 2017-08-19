#ifndef COLLISION_ENGINE_CLASS_HPP
#define COLLISION_ENGINE_CLASS_HPP

#include <algorithm>
#include <utility>
#include <string>
#include <memory>

#include "engine.hpp"
#include "entity.hpp"
#include "position_component.hpp"
#include "rigid_body_component.hpp"

class CollisionEngine : public Engine {

    typedef std::pair<PositionComponent*, RigidBodyComponent*> pos_bod_pair_t;
    std::vector< pos_bod_pair_t > pos_bod_vect;

    public:
        CollisionEngine(void);
        ~CollisionEngine(void);

        virtual void update(void);

        virtual operator std::string() const;

        virtual void entity_added(std::shared_ptr<Entity> entity);
};

#endif /* COLLISION_ENGINE_CLASS_HPP */
