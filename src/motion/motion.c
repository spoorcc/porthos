#include <stdio.h>
#include "motion.h"

/* Position tracked through motions in meters */
static position_t gl_position = {0};

/*! \brief Initializes the motion library

The motion library is initialized and ready to use.
*/
int motion_init()
{
    gl_position.x = 0.0;
    gl_position.y = 0.0;

    return (int) MOTION_OK;
}

/*! \brief Returns current position
*/
int motion_get_current_position(position_t *position)
{
    MotionError result = MOTION_OK;

    if( position == NULL)
    {
        result = MOTION_PARAMETER_ERROR;
    }
    else
    {
        *position = gl_position;
    }

    return (int) result;
}
