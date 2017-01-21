#include <stdio.h>

/*! Temporary public global for simulating sensor reading */
unsigned char range_scan_result = 0x00;

/*! error code for library */
typedef enum {RANGE_OK = 0,          /*!< Everything went OK */ \
              RANGE_PARAMETER_ERROR, /*!< Wrong parameter was provided */
              RANGE_NO_REQUEST_ERROR /*!< Scan not requested before getting result */
             } RangeError;

extern int range_init();

extern int range_schedule_scan();

extern int range_get_result();
