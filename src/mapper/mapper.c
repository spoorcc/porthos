#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "mapper.h"

typedef struct Node {

    struct Node *children[4];
    MaptileValue value;

    } Node;

static Node gl_map = {0};

static float gl_size[2] = {1.0, 1.0};
static float gl_max_level = 1;


extern int _mapper_add_children(Node * node);

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

int mapper_add_point(float x, float y, const MaptileValue value)
{
    int result = (int) MAPPER_OK;
    char current_depth = 0;
    char index = 0;
    float half_x, half_y;

    Node *current_node = &gl_map;

    while( (current_depth < gl_max_level) && (result == MAPPER_OK))
    {
        ++current_depth;
        half_x = gl_size[0] / powf(2, current_depth);
        half_y = gl_size[1] / powf(2, current_depth);

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
             result = _mapper_add_children(current_node);
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

int mapper_get_point(float x, float y, MaptileValue * value)
{
    int result = (int) MAPPER_OK;
    char current_depth = 0;
    char index = 0;
    float half_x, half_y;

    Node *current_node = &gl_map;

    while( (current_depth < gl_max_level) && (result == MAPPER_OK))
    {
        /* Return value of lowest node */
        if( current_node->children[0] == NULL)
        {
             *value = current_node->value;
             break;
        }

        ++current_depth;
        half_x = gl_size[0] / powf(2, current_depth);
        half_y = gl_size[1] / powf(2, current_depth);

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

        current_node = current_node->children[index];
    }

    return (int) result;
}

int _mapper_add_children(Node * node)
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

    /* Cleanup if there is an error */
    if( result == MAPPER_MEMORY_ERROR )
    {
        for(i=0; i<4; ++i)
        {
            if(node->children[i] != NULL)
            {
                free(node->children[i]);
            }
        }
    }

    return (int) result;
}

int _mapper_remove_children(Node * node)
{
    int result = (int) MAPPER_OK;
    char current_depth = 0;
    char index = 0;

    return (int) result;
}

int mapper_print_map()
{
    char y = 0;
    char x = 0;
    int width  = pow(2, gl_max_level);
    int height = pow(2, gl_max_level);

    int half_x = 0;
    int half_y = 0;
    int index = 0;

    Node *current_node = &gl_map;
    MaptileValue value = MAPPER_FREE;

    int *index_stack = malloc(gl_max_level * sizeof(int));
    char current_depth = 0;
    char *map = calloc( (width + 1) * height, sizeof(char));

    for(y=0; y<height; ++y)
    {
        for(x=0; x<width; ++x)
        {
            current_node = &gl_map;
            char tx = x;
            char ty = y;
            while(current_node != NULL)
            {
                ++current_depth;
                value = current_node->value;
                half_x = width / (1+current_depth);
                half_y = height / (1+current_depth);

                /* West if x < 0.0 */
                if(tx < half_x)
                {
                    index = 0;
                }
                else
                {
                    index = 1;
                    tx -= half_x;
                }

                /* South if y in bottom */
                if(ty >= half_y)
                {
                    index += 2;
                    ty -= half_y;
                }

                current_node = current_node->children[index];
            }
            fprintf(stderr, "%s", value==MAPPER_BLOCKED?"X":".");
        }
        fprintf(stderr, "\n");
    }

    free(index_stack);

    return (int) MAPPER_OK;
}

