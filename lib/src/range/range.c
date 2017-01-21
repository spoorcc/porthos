#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "porthos/range.h"


bool gl_scan_scheduled = false;

/*! \brief Initializes the range library

The range library is initialized and ready to use.
*/
int range_init()
{
    gl_scan_scheduled = false;
    return (int) RANGE_OK;
}

/*! \brief Schedules a scan
*/
int range_schedule_scan()
{
    RangeError result = RANGE_OK;

    gl_scan_scheduled = true;

    return (int) result;
}

/*! \brief Get result of last scheduled scan

\pre range_schedule_scan was called
*/
int range_get_result(unsigned char * range)
{
    RangeError result = RANGE_OK;

    if(range == NULL)
    {
       result = RANGE_PARAMETER_ERROR;
    }

    if(result == RANGE_OK)
    {
        if(gl_scan_scheduled)
        {
           *range = range_scan_result;
        }
        else
        {
            result = RANGE_NO_REQUEST_ERROR;
        }
    }

    return (int) result;
}
