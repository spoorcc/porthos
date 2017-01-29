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

     std::vector<Position> square;

     square.push_back(Position(0.0, 0.0));
     square.push_back(Position(1.0, 0.0));
     square.push_back(Position(1.0, 1.0));
     square.push_back(Position(0.0, 1.0));

     Polygon square_pol(square);

     simulator.add_polygon(square_pol);

     return (int) SIMULATOR_OK;
}
