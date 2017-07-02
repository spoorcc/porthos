#ifndef RAY_TRACE_ENGINE_CLASS_HPP
#define RAY_TRACE_ENGINE_CLASS_HPP

#include <algorithm>
#include <utility>
#include <string>
#include <vector>
#include <memory>

#include "engine.hpp"
#include "entity.hpp"
#include "move_component.hpp"
#include "position_component.hpp"

class RayTraceEngine : public Engine {

    std::vector< RigidBodyComponent* > components;

    public:
        RayTraceEngine(void);
        ~RayTraceEngine(void);

        virtual void update(void);

        virtual operator std::string() const;

        virtual void entity_added(std::shared_ptr<Entity> entity);
};

#endif /* RAY_TRACE_ENGINE_CLASS_HPP */
