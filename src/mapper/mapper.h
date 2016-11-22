/*! error code for library */
typedef enum {MAPPER_OK = 0,          /*!< Everything went OK */ \
              MAPPER_PARAMETER_ERROR, /*!< Wrong parameter was provided */
              MAPPER_MEMORY_ERROR     /*!< Memory issues */
             } MapperError;

extern int range_init();
