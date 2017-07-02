#include <algorithm>
#include <utility>
#include <string>
#include <memory>

#include "rigid_body_component.hpp"

#include "ray_trace_engine.hpp"

using namespace std;

RayTraceEngine::RayTraceEngine(void){}

RayTraceEngine::~RayTraceEngine(void){}

void RayTraceEngine::update(void) {

}

RayTraceEngine::operator std::string() const {}

void RayTraceEngine::entity_added(shared_ptr<Entity> entity) {

    RigidBodyComponent* bod_ptr = entity->component<RigidBodyComponent>();

    if ( bod_ptr != NULL )
    {
        /* Store comp */
        components.push_back(bod_ptr);
    }
}
