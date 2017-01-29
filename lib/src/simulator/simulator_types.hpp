#include <vector>
#include <assert.h>

class Position {

public:
    Position(const float x, const float y){}
    ~Position(void){}

    float x;
    float y;
};

class Polygon {

    public:
        Polygon(){}
        Polygon(std::vector<Position> points){}
        ~Polygon(void){}

    private:
        float absorbtion;
};

class Square : public Polygon {

    public:
        Square(float size) : Polygon() {

            assert(size > 0.0);

            std::vector<Position> square;

            square.push_back(Position(0.0, 0.0));
            square.push_back(Position(size, 0.0));
            square.push_back(Position(size, size));
            square.push_back(Position(0.0, size));
        }

};
