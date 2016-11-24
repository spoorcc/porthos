/*! error code for library */
typedef enum {MAPPER_OK = 0,          /*!< Everything went OK */ \
              MAPPER_PARAMETER_ERROR, /*!< Wrong parameter was provided */
              MAPPER_MEMORY_ERROR     /*!< Memory issues */
             } MapperError;

typedef enum {MAPPER_FREE = 0,
              MAPPER_BLOCKED
             } MaptileValue;

extern int mapper_init(float x_size, float y_size, unsigned int max_level);

extern int mapper_clear_map();

extern int mapper_add_point(float x, float y, const MaptileValue value);

extern int mapper_print_map();

extern int mapper_get_z_order(const int x, const int y, int * z);

extern int mapper_get_xy_from_z_order(const int z, int * x, int * y);
