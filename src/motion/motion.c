#include <stdio.h>
#include <math.h>
#include "motion.h"

/* Position tracked through motions in meters */
static position_t gl_position = {0};
static float      gl_direction = 0.0;

static position_t gl_goal_position = {0};

static float gl_speed = 1.2;
static float gl_turn_speed = 45.0;

/*! \brief Initializes the motion library

The motion library is initialized and ready to use.
*/
int motion_init()
{
    gl_position.x = 0.0;
    gl_position.y = 0.0;

    gl_goal_position.x = 0.0;
    gl_goal_position.y = 0.0;

    gl_direction = 0.0;

    return (int) MOTION_OK;
}

int motion_get_distance(float * const distance)
{
    MotionError result = MOTION_OK;

    if( distance == NULL)
    {
        result = MOTION_PARAMETER_ERROR;
    }
    else
    {
        *distance = hypot( (gl_goal_position.x - gl_position.x),
                           (gl_goal_position.y - gl_position.y));
    }

    return (int) result;
}

int motion_get_direction(float * const direction)
{
    MotionError result = MOTION_OK;

    if( direction == NULL)
    {
        result = MOTION_PARAMETER_ERROR;
    }
    else
    {
        *direction  = gl_direction;
    }

    return (int) result;
}

int motion_get_goal_direction(float * const direction)
{
    MotionError result = MOTION_OK;

    if( direction == NULL)
    {
        result = MOTION_PARAMETER_ERROR;
    }
    else
    {
        /* Calculate angle in radians */
        *direction  = atan2( (gl_goal_position.x - gl_position.x),
                             (gl_goal_position.y - gl_position.y));

        /* Radians to degrees */
        *direction = *direction * (180.0/M_PI);
    }

    return (int) result;
}

/*! \brief Returns current position
*/
int motion_get_current_position(position_t * const position)
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

/*! \brief Sets goal position
*/
int motion_move_to(position_t const * const position)
{
    MotionError result = MOTION_OK;

    if( position == NULL)
    {
        result = MOTION_PARAMETER_ERROR;
    }
    else
    {
       gl_goal_position = *position;
    }

    return (int) result;
}

int motion_update(void)
{
   MotionError result = MOTION_OK;

   float distance = 0.0;

   result = motion_get_distance(&distance);
}

int motion_update_direction(void)
{
   MotionError result = MOTION_OK;

   float goal_direction = 0.0;
   float diff = 0.0;

   result = motion_get_goal_direction(&goal_direction);

   diff = goal_direction - gl_direction;

   gl_direction += copysign(fmin(gl_turn_speed, fabs(diff)), diff);

   return (int) result;
}





