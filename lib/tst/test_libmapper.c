#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "porthos/mapper.h"

#include "mapper/mapper_internal.h"

#define CALL(x) ck_assert_msg((0 == (x)), "Should succeed");
#define ck_assert_dbl_eq_msg(X, Y, msg) ck_assert_msg((fabs((X)-(Y)) < 1e-6), msg);


Node node = {0};

/*! \brief Test initialization

Test initialization of Mapper library
*/
START_TEST (test_init)
{
    /* Execute */
    CALL(mapper_init(1.0, 1.0, 3));
}
END_TEST

/*! \brief Test adding single point
*/
START_TEST (test_add_point)
{
    /* Setup */
    CALL(mapper_init(2.0, 2.0, 2));

    /* Execute */
    CALL(mapper_add_point(1.8, 1.8, MAPPER_BLOCKED));

    CALL(mapper_print_map(true));
}
END_TEST

/*----------------------------------------------------------------------------*/
/** \internal
 *  \addtogroup test_add_children
 *  Testing add_children functions
 *  @{
 */

void test_add_children_setup(void)
{
    node.z_order_start = 0;
    node.z_order_end = 3;
}

void test_cleanup_node_teardown()
{
    int i = 0;

    for(i=0; i<4; ++i)
    {
        if(node.children[i] != NULL)
        {
            free(node.children[i]);
        }
    }

    memset(&node, 0, sizeof(Node));
}

/** \brief Add children to node */
START_TEST (test_add_children_GW001)
{
    int i = 0;

    CALL(_mapper_add_children(&node));

    for(i=0; i<4; ++i)
    {
        ck_assert(node.children[i] != NULL);
    }
}
END_TEST

/** \brief Children must get same value */
START_TEST (test_add_children_GW002)
{
    int i = 0;

    CALL(_mapper_add_children(&node));

    for(i=0; i<4; ++i)
    {
        ck_assert_int_eq(node.children[i]->value, node.value);
    }
}
END_TEST

/** \brief Children must get proper z-order */
START_TEST (test_add_children_GW003)
{
    int i = 0;

    CALL(_mapper_add_children(&node));

    for(i=0; i<4; ++i)
    {
        ck_assert_int_eq(node.children[i]->z_order_start, i);
        ck_assert_int_eq(node.children[i]->z_order_end, i);
    }
}
END_TEST

/** \brief Children must get proper z-order */
START_TEST (test_add_children_GW004)
{
    int i = 0;

    node.z_order_start = 0;
    node.z_order_end = 15;

    CALL(_mapper_add_children(&node));

    for(i=0; i<4; ++i)
    {
        ck_assert_int_eq(node.children[i]->z_order_start, i*4);
        ck_assert_int_eq(node.children[i]->z_order_end, (1+i)*4 - 1);
    }
}
END_TEST

/** \brief Children must get proper z-order */
START_TEST (test_add_children_GW005)
{
    int i = 0;

    node.z_order_start = 0;
    node.z_order_end = 63;

    CALL(_mapper_add_children(&node));

    for(i=0; i<4; ++i)
    {
        ck_assert_int_eq(node.children[i]->z_order_start, i*16);
        ck_assert_int_eq(node.children[i]->z_order_end, (1+i)*16 - 1);
    }
}
END_TEST

/** \brief Providing NULL must fail */
START_TEST (test_add_children_BW001)
{
    int i = 0;
    int result = _mapper_add_children(NULL);

    ck_assert(result == MAPPER_PARAMETER_ERROR);
}
END_TEST

/** \brief No children may be non-NULL*/
START_TEST (test_add_children_BW002)
{
    int i = 0;

    Node child = {0};

    /* Make child non-NULL */
    node.children[0] = &child;
    int result = _mapper_add_children(&node);

    ck_assert(result == MAPPER_PARAMETER_ERROR);

    /* First node was made non-NULL, others must be untouched */
    for(i=1; i<4; ++i)
    {
        ck_assert(node.children[i] == NULL);
    }

    node.children[0] = NULL;
}
END_TEST
/** @}*/


/*----------------------------------------------------------------------------*/
/** \internal
 *  \addtogroup test_remove_children
 *  Testing remove_children functions
 *  @{
 */
void test_remove_children_setup()
{
    node.z_order_start = 0;
    node.z_order_end = 3;

    CALL(_mapper_add_children(&node));
}

/** \brief Remove children from node */
START_TEST (test_remove_children_GW001)
{
    int i = 0;

    CALL(_mapper_remove_children(&node));

    for(i=0; i<4; ++i)
    {
        ck_assert(node.children[i] == NULL);
    }
}
END_TEST

/** \brief Even if one child is NULL, mustn't fail */
START_TEST (test_remove_children_GW002)
{
    int i = 0;

    free(node.children[1]);
    node.children[1] = NULL;

    CALL(_mapper_remove_children(&node));

    for(i=0; i<4; ++i)
    {
        ck_assert(node.children[i] == NULL);
    }
}
END_TEST
/** @}*/

/*----------------------------------------------------------------------------*/
/** \internal
 *  \addtogroup test_flatten_node
 *  Testing _mapper_flatten_node functions
 *  @{
 */
void test_flatten_node_setup()
{
    node.z_order_start = 0;
    node.z_order_end = 3;

    node.value = MAPPER_BLOCKED;

    CALL(_mapper_add_children(&node));
}

/** \brief Flatten node removes all children if they all have same value */
START_TEST (test_flatten_node_GW001)
{
    int i = 0;

    CALL(_mapper_flatten_node(&node));

    for(i=0; i<4; ++i)
    {
        ck_assert(node.children[i] == NULL);
    }
}
END_TEST

/** \brief Parent node gets value of children */
START_TEST (test_flatten_node_GW002)
{
    node.value = MAPPER_UNKNOWN;

    CALL(_mapper_flatten_node(&node));

    ck_assert(node.value == MAPPER_BLOCKED);

}
END_TEST


/** \brief If one value different, no flattening */
START_TEST (test_flatten_node_GW003)
{
    int i = 0;

    node.children[i]->value = MAPPER_FREE;

    CALL(_mapper_flatten_node(&node));

    for(i=0; i<4; ++i)
    {
        ck_assert(node.children[i] != NULL);
    }
}
END_TEST

/** \brief NULL parameter results in MAPPER_PARAMETER_ERROR */
START_TEST (test_flatten_node_BW001)
{
    int result = MAPPER_OK;

    result = _mapper_flatten_node(NULL);

    ck_assert(result == MAPPER_PARAMETER_ERROR);
}
END_TEST

/** @}*/


/*----------------------------------------------------------------------------*/
/** \internal
 *  \addtogroup test_get_node
 *  Testing _mapper_get_node functions
 *  @{
 */

/** \brief NULL parameter results in MAPPER_PARAMETER_ERROR */
void test_get_node_setup()
{
    CALL(mapper_init(1.0, 1.0, 3));
    CALL(mapper_add_point(0.0, 0.0, MAPPER_BLOCKED));
}

void test_get_node_teardown()
{
    CALL(mapper_clear_map());
}

/** \brief */
START_TEST (test_get_node_GW001)
{
    int result = MAPPER_OK;

    float x = 0.0;
    float y = 0.0;

    Node * node = NULL;
    Node * parent = NULL;
    bool add_nodes = false;
    int depth = 0;

    /* Setup */

    /* Execute */
    CALL(_mapper_get_node(x, y, &node, add_nodes, &parent, &depth));

    /* Verify */
    mapper_print_map(true);

    ck_assert_int_eq(depth, 3);
}
END_TEST
/** @}*/

/*! \brief Test adding single point
*/
START_TEST (test_get_point)
{
    MaptileValueEnum value = MAPPER_FREE;

    CALL(mapper_init(1.0, 1.0, 2));

    CALL(mapper_add_point(.75, .25, MAPPER_BLOCKED));

    CALL(mapper_get_point(.75, .25, &value));
    ck_assert(value == MAPPER_BLOCKED);

    CALL(mapper_get_point(.25, .75, &value));
    ck_assert(value == MAPPER_UNKNOWN);
}
END_TEST


/*! \brief Test setting area
*/
START_TEST (test_set_area)
{
    MaptileValueEnum value = MAPPER_FREE;

    CALL(mapper_init(1.0, 1.0, 2));

    CALL(mapper_add_point(.75, .25, MAPPER_BLOCKED));

    CALL(mapper_print_map(true));
    CALL(mapper_set_area(0.0, 0.0, 0.4, 0.4, value));
    CALL(mapper_print_map(true));
}
END_TEST


/*! \brief Test getting z order

A vector provides X,Y and expected Z-order
*/
#define NR_GET_Z_ORDER_TESTS (4)
static const int test_get_z_order_vectors[NR_GET_Z_ORDER_TESTS][3] = {{0,0,0},
                                                                      {2,0,4},
                                                                      {0,2,8},
                                                                      {5,5,0b110011}};

START_TEST (test_get_z_order)
{
    int z = 0;
    CALL(mapper_init(1.0, 1.0, 3));

    CALL(mapper_get_z_order(test_get_z_order_vectors[_i][0],
                            test_get_z_order_vectors[_i][1], &z));
    ck_assert_int_eq(z, test_get_z_order_vectors[_i][2]);
}
END_TEST


/*! \brief Test xy from z order
*/

#define NR_GET_XY_FROM_Z_ORDER_TESTS (4)
static const int test_get_xy_from_z_order_vectors[NR_GET_XY_FROM_Z_ORDER_TESTS][3] \
                                                                   = {{0,0,0},
                                                                      {2,0,4},
                                                                      {0,2,8},
                                                                      {5,5,0b110011}};

START_TEST (test_get_xy_from_z_order)
{
    int x = 0;
    int y = 0;
    CALL(mapper_init(1.0, 1.0, 3));

    CALL(mapper_get_xy_from_z_order(test_get_xy_from_z_order_vectors[_i][2], &x, &y));
    ck_assert_int_eq(x, test_get_xy_from_z_order_vectors[_i][0]);
    ck_assert_int_eq(y, test_get_xy_from_z_order_vectors[_i][1]);
}
END_TEST

/*! \brief Test local index from abs coords
*/
#define NR_INDEX_FROM_ABS_COORDS_TESTS (7)
static const unsigned int test_nr_index_from_abs_coords_vectors[NR_INDEX_FROM_ABS_COORDS_TESTS][5] \
                                                                   = {{0,0,0,0,0},
                                                                      {1,1,1,1,3},
                                                                      {1,1,1,0,0},
                                                                      {1,1,2,0,0},
                                                                      {1,1,2,1,0},
                                                                      {1,1,2,2,3},
                                                                      {2,1,3,3,2}};

START_TEST (test_index_from_abs_coords)
{
    unsigned int index = 0;
    index = _mapper_index_from_abs_coords(test_nr_index_from_abs_coords_vectors[_i][0],
                                          test_nr_index_from_abs_coords_vectors[_i][1],
                                          test_nr_index_from_abs_coords_vectors[_i][2],
                                          test_nr_index_from_abs_coords_vectors[_i][3]);

    ck_assert_int_eq(index, test_nr_index_from_abs_coords_vectors[_i][4]);
}
END_TEST

Suite* mapper (void) {
        Suite *suite = suite_create("range");
        TCase *tcase = tcase_create("GW");

        tcase_add_test(tcase, test_init);
        tcase_add_test(tcase, test_add_point);
        tcase_add_test(tcase, test_get_point);
        tcase_add_test(tcase, test_set_area);
        tcase_add_loop_test(tcase, test_get_z_order, 0, NR_GET_Z_ORDER_TESTS);
        tcase_add_loop_test(tcase, test_get_xy_from_z_order, 0, NR_GET_XY_FROM_Z_ORDER_TESTS);
        tcase_add_loop_test(tcase, test_index_from_abs_coords, 0, NR_INDEX_FROM_ABS_COORDS_TESTS);

        /* Add children */
        TCase *add_children_tcase = tcase_create("add_children");
        tcase_add_checked_fixture(add_children_tcase,
                                  test_add_children_setup,
                                  test_cleanup_node_teardown);
        tcase_add_test(add_children_tcase, test_add_children_GW001);
        tcase_add_test(add_children_tcase, test_add_children_GW002);
        tcase_add_test(add_children_tcase, test_add_children_GW003);
        tcase_add_test(add_children_tcase, test_add_children_GW004);
        tcase_add_test(add_children_tcase, test_add_children_GW005);
        tcase_add_test(add_children_tcase, test_add_children_BW001);
        tcase_add_test(add_children_tcase, test_add_children_BW002);

        /* Remove children */
        TCase *remove_children_tcase = tcase_create("remove_children");
        tcase_add_checked_fixture(remove_children_tcase,
                                  test_remove_children_setup,
                                  test_cleanup_node_teardown);
        tcase_add_test(remove_children_tcase, test_remove_children_GW001);
        tcase_add_test(remove_children_tcase, test_remove_children_GW002);

        /* Flatten node */
        TCase *flatten_node_tcase = tcase_create("flatten_node");
        tcase_add_checked_fixture(flatten_node_tcase,
                                  test_flatten_node_setup,
                                  test_cleanup_node_teardown);
        tcase_add_test(flatten_node_tcase, test_flatten_node_GW001);
        tcase_add_test(flatten_node_tcase, test_flatten_node_GW002);
        tcase_add_test(flatten_node_tcase, test_flatten_node_GW003);
        tcase_add_test(flatten_node_tcase, test_flatten_node_BW001);

        /* Get node */
        TCase *get_node_tcase = tcase_create("get_node");
        tcase_add_checked_fixture(get_node_tcase,
                                  test_get_node_setup,
                                  test_get_node_teardown);
        tcase_add_test(get_node_tcase, test_get_node_GW001);

        suite_add_tcase(suite, tcase);
        suite_add_tcase(suite, add_children_tcase);
        suite_add_tcase(suite, remove_children_tcase);
        suite_add_tcase(suite, flatten_node_tcase);
        suite_add_tcase(suite, get_node_tcase);
        return suite;
}

int main (int argc, char *argv[]) {
        int number_failed;
        Suite *suite = mapper();
        SRunner *runner = srunner_create(suite);

        srunner_set_fork_status(runner, CK_NOFORK);

        srunner_run_all(runner, CK_NORMAL);
        number_failed = srunner_ntests_failed(runner);
        srunner_free(runner);

        return number_failed;
}
