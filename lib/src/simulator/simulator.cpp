#include "porthos/simulator.hpp"

#include <vector>
#include <assert.h>
#include <iostream>

#include "simulator/entity.hpp"
#include "simulator/move_component.hpp"

class Simulator {

    public:
        Simulator(void){}
        ~Simulator(void){}

        void add_entity(Entity entity) { this->entities.push_back(entity);}

    private:
        std::vector<Entity> entities;

};

static Simulator simulator;

/*! \brief Initializes the simulator

The simulator library is initialized and ready to use.
*/
int simulator_init()
{
     Entity entity(1);
     MoveComponent move_comp;

     entity.add_component(move_comp);

     simulator.add_entity(entity);

     return (int) SIMULATOR_OK;
}

int simulator_status()
{
     return (int) SIMULATOR_OK;
}
