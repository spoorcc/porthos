#include "porthos/simulator.hpp"

//#include <thread>
#include <string>
#include <assert.h>
#include <iostream>

#include "simulator/simulator.hpp"
#include "simulator/entity.hpp"
#include "simulator/move_component.hpp"
#include "simulator/position_component.hpp"

static int simulator_start();

static Simulator simulator;

/*! \brief Initializes the simulator

The simulator library is initialized and ready to use.
*/
int simulator_init()
{
    int result = SIMULATOR_OK;

    result = simulator_start();

    simulator_status();

    return (int) result;
}

/* TODO: Create object factory */
static Entity* simulator_create_entity(void)
{
     static unsigned long int object_id = 0;

     Entity* entity = new Entity(object_id++);
     MoveComponent* move_comp = new MoveComponent;
     PositionComponent* pos_comp = new PositionComponent;

     entity->add_component(move_comp);
     entity->add_component(pos_comp);

     return entity;
}

static int simulator_start()
{
     Entity * entity = simulator_create_entity();
     simulator.add_entity(entity);

     return (int) SIMULATOR_OK;
}

int simulator_status()
{
     std::cout << std::string(simulator) << std::endl;
     return (int) SIMULATOR_OK;
}
