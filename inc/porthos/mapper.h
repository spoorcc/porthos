#include <stdbool.h>

/*! error code for library */
typedef enum {MAPPER_OK = 0,          /*!< Everything went OK */ \
              MAPPER_PARAMETER_ERROR, /*!< Wrong parameter was provided */
              MAPPER_MEMORY_ERROR     /*!< Memory issues */
             } MapperError;

#define FOREACH_MAPPER_TILE_VALUE(TILE_VALUE) \
                    TILE_VALUE(MAPPER_UNKNOWN)  \
                    TILE_VALUE(MAPPER_FREE)     \
                    TILE_VALUE(MAPPER_BLOCKED)  \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define str(x) #x

typedef enum {
    FOREACH_MAPPER_TILE_VALUE(GENERATE_ENUM)
} MaptileValueEnum;

static const char *MaptileValueStr[] = {
    FOREACH_MAPPER_TILE_VALUE(GENERATE_STRING)
};

extern int mapper_init(float x_size, float y_size, unsigned int max_level);

extern int mapper_clear_map();

extern int mapper_add_point(float x, float y, const MaptileValueEnum value);
extern int mapper_get_point(float x, float y, MaptileValueEnum * value);
extern int mapper_set_area(float x1, float y1, float x2, float y2, MaptileValueEnum value);

extern int mapper_print_map(bool with_depth);

extern int mapper_get_z_order(const int x, const int y, int * z);
extern int mapper_get_xy_from_z_order(const int z, int * x, int * y);
