#include <stdio.h>
#include <stdbool.h>

#include "mapper.h"

typedef struct {

    Node *children[4];
    bool value;

    } Node;

static Node gl_map = {0};

static float gl_size[2] = {1.0, 1.0};
static float gl_max_level = 1;

/*! \brief Initializes the mapper library
*/
int mapper_init(float x_size, float y_size, unsigned int max_level)
{
    mapper_clear_map();

    gl_size[0] = x_size;
    gl_size[1] = y_size;
    gl_max_level = max_level;

    return (int) MAPPER_OK;
}

int mapper_clear_map()
{
   /*    \todo  Cleanup memory  */
}

int mapper_add_point(float x, float y, const bool value)
{
    int result = (int) MAPPER_OK;
    char current_depth = 0;
    char index = 0;

    Node *current_node = &gl_map;

    while( (current_depth < gl_max_level) && (result == MAPPER_OK))
    {
        ++current_depth;
        half_x = gl_size[0] / (2**current_depth);
        half_y = gl_size[1] / (2**current_depth);

        /* West if x < 0.0 */
        if(x < half_x)
        {
            index = 0;
        }
        else
        {
            index = 1;
            x -= half_x;
        }

        /* South if y in bottom */
        if(y >= half_y)
        {
            index += 2;
            y -= half_y;
        }

        /* Create children if needed */
        if( current_node->children[0] == NULL)
        {
             result = mapper_add_children(current_node);
        }

        /* Get next node*/
        if ( result == MAPPER_OK)
        {
            current_node = current_node->children[index];
        }
    }

    current_node->value = value;

    return (int) result;
}

int mapper_add_children(Node * node)
{
    int result = (int) MAPPER_OK;
    char i = 0;

    for(i=0; (i<4)&&(result != MAPPER_MEMORY_ERROR); ++i)
    {
       node->children[i] = calloc(1, sizeof(Node));
       if(node->children[i] == NULL)
       {
           result = MAPPER_MEMORY_ERROR;
       }
    }

    return (int) result;
}
