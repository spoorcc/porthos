#ifndef _DATATYPES_HPP_
#define _DATATYPES_HPP_


struct xyz_vec_t {
    float x, y, z;
};

struct xy_vec_t {
    float x, y;

    xy_vec_t(float x,float y) : x(x), y(y) {}
};

#endif /* _DATATYPES_HPP_ */
