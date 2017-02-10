#ifndef FACTORY_H
#define FACTORY_H

#include "entity.hpp"
#include "datatypes.hpp"

extern Entity* factory_create_wall( const xy_vec_t pos,
                                    const xy_vec_t size );
extern Entity* factory_create_robot(void);

#endif /* FACTORY_H */
