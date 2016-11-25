#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "mapper.h"

typedef struct Node {

    struct Node *children[4];
    MaptileValueEnum value;

    } Node;

static Node gl_map = {0};

static float gl_size[2] = {1.0, 1.0};
static int gl_max_level = 1;


static int _mapper_add_children(Node * node);
static int _mapper_get_node(float x, float y, Node **node, bool add_nodes);

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

int mapper_add_point(float x, float y, const MaptileValueEnum value)
{
    int result = (int) MAPPER_OK;
    Node * node = NULL;

    result = _mapper_get_node(x, y, &node, true);

    if( result == (int) MAPPER_OK)
    {
        node->value = value;
    }

    return (int) result;
}

int _mapper_get_node(float x, float y, Node **node, bool add_nodes)
{
    int result = (int) MAPPER_OK;
    int current_depth = 0;
    int index = 0;

    int max_depth = pow(2, gl_max_level);

    *node = &gl_map;

    int ax = x * max_depth / gl_size[0];
    int ay = y * max_depth / gl_size[1];

    for(current_depth=0; (current_depth <= gl_max_level) && (result == MAPPER_OK); ++current_depth)
    {
        /* \todo Cleanup */
        index =  (ax & (1<<(gl_max_level - current_depth)))?1:0;
        index += (ay & (1<<(gl_max_level - current_depth)))?2:0;

        /* Create children if needed */
        if((*node)->children[0] == NULL)
        {
            if(add_nodes)
            {
                result = _mapper_add_children(*node);
            }
            else
            {
                break;
            }
        }

        /* Get next node*/
        if ( result == MAPPER_OK)
        {
            *node = (*node)->children[index];
        }
    }

    return (int) result;
}

int mapper_get_point(float x, float y, MaptileValueEnum * value)
{
    int result = (int) MAPPER_OK;
    Node *node = NULL;

    result = _mapper_get_node(x, y, &node, false);

    if(result == (int) MAPPER_OK)
    {
        *value = node->value;
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

/**
\brief Calculates Z-order from x,y coordinates

\param[in] x X-coordinate
\param[in] y Y-coordinate
\param[out] z Z-order

Based on provided X,Y coordinates calculates the Z-order.
For more information see https://en.wikipedia.org/wiki/Z-order_curve

\pre level must be set using mapper_init
*/
int mapper_get_z_order(const int x, const int y, int * z)
{
   int result = (int) MAPPER_OK;
   int size  = pow(2, gl_max_level-1);
   int i  = 0;

   if (z == NULL)
   {
       result = (int) MAPPER_PARAMETER_ERROR;
   }
   else
   {
       *z = 0;

       for(i=0; i<=size; i+=2)
       {
          *z |= (x & (1 << i/2)) << i/2;
          *z |= (y & (1 << i/2)) << i/2+1;
       }
   }

   return (int) result;
}

/**
\brief Calculates X,Y coordinates based on Z-order

\param[in] z Z-order
\param[out] x X-coordinate
\param[out] y Y-coordinate

Based on provided Z-order calculates the X,Y coordinates.
For more information see https://en.wikipedia.org/wiki/Z-order_curve

\pre level must be set using mapper_init
*/
int mapper_get_xy_from_z_order(const int z, int * x, int * y)
{
   int result = (int) MAPPER_OK;
   int size  = pow(2, gl_max_level-1);
   int i  = 0;

   if ((x == NULL) || (y == NULL))
   {
       result = (int) MAPPER_PARAMETER_ERROR;
   }
   else
   {
       *x = 0;
       *y = 0;

       for(i=0; i<=size; i+=2)
       {
          *x |= (z & (1 << i)) >> i/2;
          *y |= (z & (1 << i+1)) >> i/2+1;
       }
   }

   return (int) result;
}

int mapper_print_map()
{
    int result = (int) MAPPER_OK;
    float y = 0;
    float x = 0;

    float dx = gl_size[0] / pow(2, gl_max_level);
    float dy = gl_size[1] / pow(2, gl_max_level);

    MaptileValueEnum value = MAPPER_FREE;

    for(y=0; y<gl_size[1]; y+=dy)
    {
        for(x=0; x<gl_size[0]; x+=dx)
        {
            result = mapper_get_point(x, y, &value);

            fprintf(stderr, "%s", value==MAPPER_BLOCKED?" X":" .");
        }
        fprintf(stderr, "\n");
    }

    return (int) MAPPER_OK;
}

