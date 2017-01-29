#include "porthos/simulator.hpp"

#include "simulator/simulator_types.hpp"
#include <vector>

class Simulator {

    public:
        Simulator(void){}
        ~Simulator(void){}

        void add_polygon(Polygon polygon) { this->polygons.push_back(polygon);}

    private:
        std::vector<Polygon> polygons;

};

/*! \brief Initializes the simulator

The simulator library is initialized and ready to use.
*/
int simulator_init()
{
     Simulator simulator;

     Square square_pol(1.0);

     simulator.add_polygon(square_pol);

     return (int) SIMULATOR_OK;
}

int simulator_status()
{

     return (int) SIMULATOR_OK;
}
