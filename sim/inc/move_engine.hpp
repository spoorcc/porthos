#ifndef MOVE_ENGINE_CLASS_HPP
#define MOVE_ENGINE_CLASS_HPP

#include <algorithm>
#include <utility>
#include <string>
#include <vector>

#include "engine.hpp"
#include "entity.hpp"
#include "move_component.hpp"
#include "position_component.hpp"

class MoveEngine : public Engine {

    typedef std::pair<PositionComponent*, MoveComponent*> pos_mov_pair_t;
    std::vector< pos_mov_pair_t > pos_mov_vect;

    public:
        MoveEngine(void);
        ~MoveEngine(void);

        virtual void update(void);

        virtual operator std::string() const;

        virtual void entity_added(Entity* entity);
};

#endif /* MOVE_ENGINE_CLASS_HPP */
