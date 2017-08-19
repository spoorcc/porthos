#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    /*! error code for library */
    typedef enum {SIMULATOR_OK = 0,          /*!< Everything went OK */ \
    } SimulatorError;

    extern int simulator_init();
    extern int simulator_run(unsigned long int seconds_to_run);
    extern int simulator_status();

#ifdef __cplusplus
}
#endif
