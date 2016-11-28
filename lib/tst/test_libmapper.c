#include <check.h>
#include <math.h>
#include <stdio.h>

#include "porthos/mapper.h"

#include "mapper/mapper_internal.h"

#define CALL(x) ck_assert_msg((0 == (x)), "Should succeed");
#define ck_assert_dbl_eq_msg(X, Y, msg) ck_assert_msg((fabs((X)-(Y)) < 1e-6), msg);

/*! \brief Test initialization

Test initialization of Mapper library
*/
START_TEST (test_init)
{
    CALL(mapper_init(1.0, 1.0, 3));
}
END_TEST

/*! \brief Test adding single point
*/
START_TEST (test_add_point)
{
    CALL(mapper_init(2.0, 2.0, 4));

    CALL(mapper_add_point(1.8, 1.8, MAPPER_BLOCKED));
    CALL(mapper_add_point(0.0, 0.0, MAPPER_BLOCKED));

    CALL(mapper_add_point(1.2, 0.0, MAPPER_FREE));
    CALL(mapper_add_point(1.2, 0.8, MAPPER_FREE));
    CALL(mapper_add_point(1.8, 0.0, MAPPER_FREE));
    CALL(mapper_add_point(1.8, 0.8, MAPPER_FREE));

    CALL(mapper_print_map(true));
    fprintf(stderr, "\n");
    CALL(mapper_print_map(false));
}
END_TEST


/** \internal
 *  \addtogroup test_add_children
 *  Testing add_children functions
 *  @{
 */
Node node = {0};

START_TEST (test_add_children_setup)
{
    node.z_order_start = 0;
    node.z_order_end = 3;
}
END_TEST

START_TEST (test_add_children_teardown)
{
    int i = 0;

    for(i=0; i<4; ++i)
    {
        if(node.children[i] != NULL)
        {
            free(node.children[i]);
        }
    }
}
END_TEST

START_TEST (test_add_children_GW001)
{
    int i = 0;

    CALL(_mapper_add_children(&node));

    for(i=0; i<4; ++i)
    {
        ck_assert(node.children[i] != NULL);
        free(node.children[i]);
    }
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

Suite* mapper (void) {
        Suite *suite = suite_create("range");
        TCase *tcase = tcase_create("GW");

        tcase_add_test(tcase, test_init);
        tcase_add_test(tcase, test_add_point);
        tcase_add_test(tcase, test_get_point);
        tcase_add_test(tcase, test_set_area);
        tcase_add_loop_test(tcase, test_get_z_order, 0, NR_GET_Z_ORDER_TESTS);
        tcase_add_loop_test(tcase, test_get_xy_from_z_order, 0, NR_GET_XY_FROM_Z_ORDER_TESTS);


        TCase *add_children_tcase = tcase_create("GW");
        tcase_add_checked_fixture(add_children_tcase,
                                  test_add_children_setup,
                                  test_add_children_teardown);
        tcase_add_test(add_children_tcase, test_add_children_GW001);

        suite_add_tcase(suite, tcase);
        return suite;
}

int main (int argc, char *argv[]) {
        int number_failed;
        Suite *suite = mapper();
        SRunner *runner = srunner_create(suite);

        srunner_run_all(runner, CK_NORMAL);
        number_failed = srunner_ntests_failed(runner);
        srunner_free(runner);

        return number_failed;
}
