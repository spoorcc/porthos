#ifndef COLLISION_ENGINE_CLASS_HPP
#define COLLISION_ENGINE_CLASS_HPP

#include <algorithm>
#include <utility>
#include <string>

#include "engine.hpp"
#include "entity.hpp"
#include "move_component.hpp"
#include "position_component.hpp"

class CollisionEngine : public Engine {

    typedef std::pair<PositionComponent*, CollisionComponent*> pos_col_pair_t;
    std::vector< pos_col_pair_t > pos_col_vect;

    public:
        CollisionEngine(void);
        ~CollisionEngine(void);

        virtual void update(void);

        virtual operator std::string() const;

        virtual void entity_added(Entity* entity);
};

#endif /* COLLISION_ENGINE_CLASS_HPP */
