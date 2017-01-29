#include <vector>

class Position {

public:
    Position(const float x, const float y){}
    ~Position(void){}

    float x;
    float y;
};

class Polygon {

    public:
        Polygon(std::vector<Position> points){}
        ~Polygon(void){}

    private:
        float absorbtion;
};
