#include "porthos/simulator.hpp"

//#include <thread>
#include <string>
#include <assert.h>
#include <iostream>
#include <thread>
#include <chrono>

#define UPDATES_PER_SECOND (100)

#include "simulator.hpp"
#include "factory.hpp"

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
     simulator.add_entity(factory_create_robot());

     return (int) SIMULATOR_OK;
}

int simulator_run(unsigned long int seconds_to_run)
{
  std::cout << "Starting simulation\n";

  unsigned long int i;
  unsigned int j;

  for (i=seconds_to_run; i>0; --i) {

    for(j = UPDATES_PER_SECOND; j>0; --j)
    {
        std::this_thread::sleep_for (std::chrono::milliseconds(1000/UPDATES_PER_SECOND));
        simulator.update();
    }
  }
  std::cout << "Simulation done\n";
  return (int) SIMULATOR_OK;
}

int simulator_status()
{
     std::cout << std::string(simulator) << std::endl;
     return (int) SIMULATOR_OK;
}
