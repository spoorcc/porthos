#include <stdio.h>

/*! error code for library */
typedef enum {RANGE_OK = 0,         /*!< Everything went OK */ \
              RANGE_PARAMETER_ERROR /*!< Wrong parameter was provided */
             } RangeError;

extern int range_init();
