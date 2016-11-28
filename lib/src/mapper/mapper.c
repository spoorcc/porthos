#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Public interface */
#include "porthos/mapper.h"

/* Private interface */
#include "mapper/mapper_internal.h"

/* Module globals */
static Node gl_map = {0};

static float gl_size[2] = {1.0, 1.0};
static int gl_max_level = 1;

/** \addtogroup mapper-library
 *  Library for keeping map of surroundings
 *  @{
 */

/*! \brief Initializes the mapper library

\param[in] x_size     Size in X-direction
\param[in] y_size     Size in Y-direction
\param[in] max_level  Two to the max_level of detail

First mapper_init must be called to configure the library. With this call the
real-world size that the map spans is set. The max_level sets the 2 to the Nth 
maximum level of detail. For example max_level of 2 means the map will be divided
in 4x4 grid.
*/
int mapper_init(float x_size, float y_size, unsigned int max_level)
{
    int result = mapper_clear_map();

    /* Store settings */
    gl_size[0] = x_size;
    gl_size[1] = y_size;
    gl_max_level = max_level;

    /* Initialize root-node */
    gl_map.value = MAPPER_UNKNOWN;
    gl_map.z_order_start = 0;
    gl_map.z_order_end = pow(2, gl_max_level+2)-1;

    return (int) MAPPER_OK;
}

int _mapper_remove_childless_nodes_cb(Node ** node)
{
    int i = 0;

    if(!_mapper_node_has_children(*node) && (*node != &gl_map))
    {
        free(*node);
        *node = NULL;
    }
    return (int) MAPPER_OK;
}

bool _mapper_node_has_children(Node * node)
{
    bool has_children = false;
    int i = 0;

    for(i=0; (i<4)&&(!has_children)&&(node != NULL); ++i)
    {
        has_children = (node->children[i] != NULL);
    }

    return has_children;
}

/**
\brief Clears entire map
*/
int mapper_clear_map()
{
    int result = (int) MAPPER_OK;

    while(_mapper_node_has_children(&gl_map) && (result == MAPPER_OK))
    {
        result = _mapper_visit_graph(NULL, _mapper_remove_childless_nodes_cb);
    }

    return result;
}

/** \brief Sets value at given coordinate

\param[in] x X-coordinate
\param[in] y Y-coordinate
\param[in] value

The X,Y coordinate must be within map_size set during mapper_init

\pre mapper_init has been called

*/
int mapper_add_point(float x, float y, const MaptileValueEnum value)
{
    int result = (int) MAPPER_OK;
    Node * node = NULL;
    Node * parent = NULL;

    result = _mapper_get_node(x, y, &node, true, &parent, NULL);

    if( result == (int) MAPPER_OK && (node != NULL) && (parent != NULL))
    {
        node->value = value;

        result = _mapper_flatten_node(parent);
    }

    return (int) result;
}

static float gl_set_area_field[2][2] = {{0.0, 0.0}, {0.0, 0.0}};
static MaptileValueEnum gl_set_area_value = MAPPER_UNKNOWN;
static int _mapper_set_area_cb(Node ** node)
{
    int nx1, ny1, nx2, ny2;
    int result = MAPPER_OK;

    if(*node == NULL)
    {
        result = MAPPER_PARAMETER_ERROR;
    }

    if(!_mapper_node_has_children(*node) && (result == MAPPER_OK))
    {
        result = mapper_get_xy_from_z_order((*node)->z_order_start, &nx1, &ny1);

        if(result == MAPPER_OK)
        {
            result = mapper_get_xy_from_z_order((*node)->z_order_end, &nx2, &ny2);
        }

        fprintf(stderr, "%d,%d --> %d,%d\n", nx1, ny1, nx2, ny2);
        if(result == MAPPER_OK)
        {
            /* Node completly within area */
            if( nx1 >= gl_set_area_field[0][0] &&
                ny1 >= gl_set_area_field[0][1] &&
                nx2 <= gl_set_area_field[1][0] &&
                ny2 <= gl_set_area_field[1][1])
            {
                (*node)->value = gl_set_area_value;
            }
            else
            {
                result = _mapper_add_children(*node);

                if(result == MAPPER_OK)
                {
                    result = _mapper_visit_graph(*node, _mapper_set_area_cb);
                }
            }
        }

    }

    return (int) result;
}

/**

\param[in] x1
\param[in] y1
\param[in] x2
\param[in] y2
\param[in] value

Sets the region with x1,y1 --> x2,y2 to given value

\retval MAPPER_OK              All went well
\retval MAPPER_PARAMETER_ERROR Coordinates are not OK
*/
int mapper_set_area(float x1, float y1, float x2, float y2, MaptileValueEnum value)
{
    int result = (int) MAPPER_OK;

    int max_depth = pow(2, gl_max_level+1);

    if((x1 >= x2) || (y1 >= y2))
    {
        result = MAPPER_PARAMETER_ERROR;
    }
    else
    {
        gl_set_area_field[0][0] = x1 * max_depth / gl_size[0];
        gl_set_area_field[0][1] = y1 * max_depth / gl_size[1];
        gl_set_area_field[1][0] = x2 * max_depth / gl_size[0];
        gl_set_area_field[1][1] = y2 * max_depth / gl_size[1];

        gl_set_area_value = value;

        result = _mapper_visit_graph(NULL, _mapper_set_area_cb);
    }

    return result;
}

int _mapper_get_node(float x, float y, Node **node,
                     bool add_nodes, Node **parent, int * depth)
{
    int result = (int) MAPPER_OK;
    int current_depth = 0;
    int index = 0;

    int max_depth = pow(2, gl_max_level+1);

    *node = &gl_map;
    *parent = NULL;

    int ax = x * max_depth / gl_size[0];
    int ay = y * max_depth / gl_size[1];

    for(current_depth=0; (current_depth < gl_max_level) && (result == MAPPER_OK); ++current_depth)
    {
        /* \todo Cleanup */
        index =  (ax & (1<<(gl_max_level - current_depth)))?1:0;
        index += (ay & (1<<(gl_max_level - current_depth)))?2:0;

        /* Create children if needed */
        if(!_mapper_node_has_children(*node))
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
        if (( result == MAPPER_OK) && ((*node)->children[index] != NULL))
        {
            *parent = *node;
            *node = (*node)->children[index];
        }
    }

    if(depth != NULL)
    {
        *depth = current_depth;
    }

    return (int) result;
}

int _mapper_visit_graph(Node *node,
                               node_cb_func callback_func)
{
    int result = (int) MAPPER_OK;
    char i = 0;

    if(node == NULL)
    {
        result = _mapper_visit_graph(&gl_map, callback_func);
    }
    else
    {
        for(i=0; (i<4)&&(result==MAPPER_OK); ++i)
        {
            if(node->children[i] != NULL)
            {
                result = callback_func(&node->children[i]);

                if(result == MAPPER_OK)
                {
                    /* TODO Make non-recursive */
                    result = _mapper_visit_graph(node->children[i], callback_func);
                }
            }
        }
    }

    return (int) result;
}

/** \brief Gets value at given coordinate

\param[in] x X-coordinate
\param[in] y Y-coordinate
\param[out] value

The X,Y coordinate must be within map_size set during mapper_init

\pre mapper_init has been called

*/
int mapper_get_point(float x, float y, MaptileValueEnum * value)
{
    int result = (int) MAPPER_OK;
    Node *node = NULL;
    Node *parent = NULL;

    result = _mapper_get_node(x, y, &node, false, &parent, NULL);

    if(result == (int) MAPPER_OK)
    {
        *value = node->value;
    }

    return (int) result;
}

/**
\brief Adds 4 new children to given node

\param[in] node Pointer node to add children to

\retval MAPPER_OK              Everything went OK
\retval MAPPER_PARAMETER_ERROR One of the children already exists
\retval MAPPER_MEMORY_ERROR    Allocating memory went wrong
*/
int _mapper_add_children(Node * node)
{
    int result = (int) MAPPER_OK;
    char i = 0;

    int z_order_delta = 0;

    if( node == NULL)
    {
        result = MAPPER_PARAMETER_ERROR;
    }
    else
    {
       z_order_delta = (1 + node->z_order_end - node->z_order_start)/4;

       for(i=0; (i<4)&&(result == MAPPER_OK)&&(z_order_delta!=0); ++i)
       {
          if(node->children[i] != NULL)
          {
              result = MAPPER_PARAMETER_ERROR;
          }
          else
          {
              node->children[i] = calloc(1, sizeof(Node));
              if(node->children[i] == NULL)
              {
                  result = MAPPER_MEMORY_ERROR;
              }
              else
              {
                  node->children[i]->value = node->value;
                  node->children[i]->z_order_start = node->z_order_start + i * z_order_delta;
                  node->children[i]->z_order_end = node->z_order_start + (i+1) * z_order_delta -1;
              }
          }
       }
    }

    /* Cleanup if there is an error during allocation */
    if( result != MAPPER_OK )
    {
        for(i=0; i<4; ++i)
        {
            if((result == MAPPER_MEMORY_ERROR) &&
               (node != NULL) && (node->children[i] != NULL))
            {
                free(node->children[i]);
                node->children[i] = NULL;
            }
        }
    }

    return (int) result;
}

/**
\brief If all children have same value remove children and set parent to same value
*/
int _mapper_flatten_node(Node * node)
{
    int result = (int) MAPPER_OK;
    char i = 0;
    bool same_value = true;

    if(node == NULL)
    {
        result = MAPPER_PARAMETER_ERROR;
    }
    else
    {
       /* See if all children have the same value */
       for(i=0; same_value && (i<3)&&(result == MAPPER_OK); ++i)
       {
          if((node->children[i] != NULL) &&
             (node->children[i+1] != NULL))
          {
              same_value = (node->children[i]->value == node->children[i+1]->value);
          }
       }
       if(same_value)
       {
           node->value = node->children[i]->value;

           result = _mapper_remove_children(node);
       }
    }

    return result;
}

/**
\brief Frees memory of all non-NULL children pointers
*/
int _mapper_remove_children(Node * node)
{
    int result = (int) MAPPER_OK;
    char i = 0;

    for(i=0; (i<4)&&(result == MAPPER_OK); ++i)
    {
       if(node->children[i] != NULL)
       {
           if(node->children[i]->children[0] != NULL)
           {
               result = MAPPER_MEMORY_ERROR;
           }
           else
           {
              free(node->children[i]);
              node->children[i] = NULL;
           }
       }
    }

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

/**
\brief Print the map on stderr

\param[in] with_depth If enabled, depth of graph-node is shown
*/
int mapper_print_map(bool with_depth)
{
    int result = (int) MAPPER_OK;

    Node * node = NULL;
    Node * parent = NULL;
    int depth = 0;

    float y = 0;
    float x = 0;

    float dx = gl_size[0] / pow(2, gl_max_level);
    float dy = gl_size[1] / pow(2, gl_max_level);

    MaptileValueEnum value = MAPPER_FREE;

    for(y=0; y<gl_size[1]; y+=dy)
    {
        for(x=0; x<gl_size[0]; x+=dx)
        {
            result = _mapper_get_node(x, y, &node, false, &parent, &depth);
            value = node->value;
            fprintf(stderr, "%c%s", with_depth?depth+0x30:0x20, value==MAPPER_BLOCKED?"X":value==MAPPER_UNKNOWN?"?":".");
        }
        fprintf(stderr, "\n");
    }

    return (int) MAPPER_OK;
}

/** @}*/
