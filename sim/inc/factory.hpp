#ifndef FACTORY_H
#define FACTORY_H

#include <memory>

#include "entity.hpp"
#include "datatypes.hpp"

extern std::shared_ptr<Entity> factory_create_wall( const xy_vec_t pos,
                                    const xy_vec_t size );
extern std::shared_ptr<Entity> factory_create_robot(void);

#endif /* FACTORY_H */
