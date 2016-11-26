#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "porthos/mapper.h"

typedef struct Node {

    struct Node *children[4];
    MaptileValueEnum value;

    } Node;

static Node gl_map = {0};

static float gl_size[2] = {1.0, 1.0};
static int gl_max_level = 1;

/* Module forward declarations */
static int _mapper_add_children(Node * node);
static int _mapper_remove_children(Node * node);
static int _mapper_get_node(float x, float y, Node **node, bool add_nodes, Node **parent, int *depth);
static int _mapper_flatten_node(Node * node);

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

    if( result == (int) MAPPER_OK)
    {
        node->value = value;

        result = _mapper_flatten_node(parent);
    }

    return (int) result;
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

    for(i=0; (i<4)&&(result == MAPPER_OK); ++i)
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
       }
    }

    /* Cleanup if there is an error */
    if( result != MAPPER_OK )
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
              same_value = (node->children[i]->value != node->children[i+1]->value);
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
static int _mapper_remove_children(Node * node)
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

