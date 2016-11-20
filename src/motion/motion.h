#include <stdio.h>

/*! error code for library */
typedef enum {MOTION_OK = 0,         /*!< Everything went OK */ \
              MOTION_PARAMETER_ERROR /*!< Wrong parameter was provided */
             } MotionError;

/*! Simple position struct */
typedef struct {
   double x;
   double y;
} position_t;

extern int motion_init();

extern int motion_get_current_position(position_t * const position);
extern int motion_get_distance(float * const distance);
extern int motion_get_direction(float * const degrees);
extern int motion_get_goal_direction(float * const degrees);

extern int motion_move_to(position_t const * const position);

extern int motion_update(void);
extern int motion_update_direction(void);
