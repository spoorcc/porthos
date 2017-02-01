#include "porthos/simulator.hpp"

//#include <thread>
#include <string>
#include <assert.h>
#include <iostream>

#include "simulator/simulator.hpp"
#include "simulator/factory.hpp"

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
    simulator.update();

    return (int) result;
}

static int simulator_start()
{
     simulator.add_entity(factory_create_entity());
     simulator.add_entity(factory_create_entity());

     return (int) SIMULATOR_OK;
}

int simulator_status()
{
     std::cout << std::string(simulator) << std::endl;
     return (int) SIMULATOR_OK;
}
