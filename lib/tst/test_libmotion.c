#include <check.h>
#include <math.h>
#include <stdio.h>

#include "porthos/motion.h"

#define CALL(x) ck_assert_msg((0 == (x)), "Should succeed");
#define ck_assert_dbl_eq_msg(X, Y, msg) ck_assert_msg((fabs((X)-(Y)) < 1e-6), msg);

/*! \brief Test initialization

Test initialization of Motion library
*/
START_TEST (test_init)
{
    motion_init();
}
END_TEST


/*! \brief Test getting position
*/
START_TEST (test_get_current_position_GW001)
{
    position_t position = {0};

    /* Setup */
    CALL( motion_init());

    /* Execute */
    CALL(motion_get_current_position(&position));

    /* Verify */
    ck_assert_dbl_eq_msg(position.x, 0.0, "X-position is wrong");
    ck_assert_dbl_eq_msg(position.y, 0.0, "Y-position is wrong");

    /* Teardown */
}
END_TEST


/*! \brief Test getting position - NULL parameter
*/
START_TEST (test_get_current_position_BW001)
{
    int result = MOTION_OK;

    /* Setup */
    CALL(motion_init());

    /* Execute */
    result = motion_get_current_position(NULL);

    /* Verify */
    fail_if(result != (int) MOTION_PARAMETER_ERROR, "Should fail");

    /* Teardown */
}
END_TEST


/*! \brief Test setting position
*/
START_TEST (test_move_to_GW001)
{
    position_t position = {.x=42.0, .y=53.0};

    /* Setup */
    CALL(motion_init());

    /* Execute */
    CALL(motion_move_to(&position));

    /* Verify */

    /* Teardown */
}
END_TEST


/*! \brief Test setting position - NUL parameter
*/
START_TEST (test_move_to_BW001)
{
    int result = MOTION_OK;

    /* Setup */
    CALL(motion_init());

    /* Execute */
    result = motion_move_to(NULL);

    /* Verify */
    fail_if(result != (int) MOTION_PARAMETER_ERROR, "Should fail");

    /* Teardown */
}
END_TEST

/*! \brief Test getting distance
*/
START_TEST (test_get_distance_GW001)
{
    position_t position = {.x=3.0, .y=4.0};
    float distance = 0.0;

    /* Setup */
    CALL(motion_init());
    CALL(motion_move_to(&position));

    /* Execute */
    CALL(motion_get_distance(&distance));

    /* Verify */
    ck_assert_dbl_eq_msg(distance, 5.0, "Should be 3.0, 4.0, 5.0 triangle");

    /* Teardown */
}
END_TEST

/*! \brief Test getting distance
*/
START_TEST (test_get_distance_BW001)
{
    int result = MOTION_OK;

    /* Setup */
    CALL(motion_init());

    /* Execute */
    result = motion_get_distance(NULL);

    /* Verify */
    fail_if(result != (int) MOTION_PARAMETER_ERROR, "Should fail");

    /* Teardown */
}
END_TEST


/*! \brief Test getting direction
*/
START_TEST (test_get_direction_GW001)
{
    position_t position = {.x=3.0, .y=4.0};
    float direction = 0.0;

    /* Setup */
    CALL(motion_init());

    /* Execute */
    CALL(motion_get_direction(&direction));

    /* Verify */
    ck_assert_dbl_eq_msg(direction, 0.0, "Should be 0.0");

    /* Teardown */
}
END_TEST

/*! \brief Test getting distance
*/
START_TEST (test_get_direction_BW001)
{
    int result = MOTION_OK;

    /* Setup */
    CALL(motion_init());

    /* Execute */
    result = motion_get_direction(NULL);

    /* Verify */
    fail_if(result != (int) MOTION_PARAMETER_ERROR, "Should fail");

    /* Teardown */
}
END_TEST


/*! \brief Test getting goal direction
*/
#define NR_GET_DEG_TESTS (6)
static const position_t get_goal_direction_GW_positions[NR_GET_DEG_TESTS] = {{.x=-2.0, .y=-2.0},
                                                                        {.x=-2.0, .y=0.0},
                                                                        {.x=0.0, .y=-2.0},
                                                                        {.x=2.0, .y=-2.0},
                                                                        {.x=2.0, .y=0.0},
                                                                        {.x=0.0, .y=2.0}};
static const float get_goal_direction_GW_degrees[NR_GET_DEG_TESTS] = {-135.0, -90.0, 180.0, 135.0, 90.0, 0.0};

START_TEST (test_get_goal_direction_GW001)
{
    int result = MOTION_OK;
    float degrees = 0.0;

    /* Setup */
    CALL(motion_init());
    CALL(motion_move_to(&get_goal_direction_GW_positions[_i]));

    /* Execute */
    CALL(motion_get_goal_direction(&degrees));

    /* Verify */
    ck_assert_dbl_eq_msg(degrees,
                         get_goal_direction_GW_degrees[_i],
                         "Should be 135 degrees");

    /* Teardown */
}
END_TEST

/*! \brief Test getting goal direction
*/
START_TEST (test_get_goal_direction_BW001)
{
    int result = MOTION_OK;

    /* Setup */
    CALL(motion_init());

    /* Execute */
    result = motion_get_goal_direction(NULL);

    /* Verify */
    fail_if(result != (int) MOTION_PARAMETER_ERROR, "Should fail");

    /* Teardown */
}
END_TEST


/*! \brief Test updating direction
*/
START_TEST (test_update_direction_GW001)
{
    position_t position = {.x=-2.0, .y=-2.0};
    float current_direction = 0.0;
    float goal_direction = 0.0;

    /* Setup */
    CALL(motion_init());
    CALL(motion_move_to(&position));

    /* Execute */
    CALL(motion_update_direction());

    /* Verify */
    CALL(motion_get_direction(&current_direction));
    CALL(motion_get_goal_direction(&goal_direction));

    ck_assert_dbl_eq_msg(current_direction, -45.0, "Should be reduced to -45");

    /* Teardown */
}
END_TEST


/*! \brief Test updating direction
*/
START_TEST (test_update_direction_GW002)
{
    position_t position = {.x=sqrt(3), .y=1.0};
    float current_direction = 0.0;

    /* Setup */
    CALL(motion_init());
    CALL(motion_move_to(&position));

    /* Execute */
    CALL(motion_update_direction());

    /* Verify */
    CALL(motion_get_direction(&current_direction));
    ck_assert_dbl_eq_msg(current_direction, 45.0, "Should be reduced to -90");

    /* Teardown */
}
END_TEST

/* \brief Test updating movement
*/
START_TEST (test_update_movement_GW001)
{
    position_t position = {.x=0.0, .y=10.0};
    position_t current_position = {0};

    /* Setup */
    CALL(motion_init());
    CALL(motion_move_to(&position));

    /* Execute */
    CALL(motion_update_movement());

    /* Verify */
    CALL(motion_get_current_position(&current_position));
    ck_assert_dbl_eq_msg(current_position.x, 0.0, "Should moved along Y-axis");
    ck_assert_dbl_eq_msg(current_position.y, 1.2, "Should moved along Y-axis");

    /* Teardown */
}
END_TEST


/* \brief Test updating movement
*/
START_TEST (test_update_movement_BW001)
{
    int result = (int) MOTION_OK;
    position_t position = {.x=10.0, .y=10.0};

    /* Setup */
    CALL(motion_init());
    CALL(motion_move_to(&position));

    /* Execute */
    result = motion_update_movement();

    /* Verify */
    ck_assert_msg(result == (int) MOTION_NOT_ALIGNED_ERROR, "Should not move before turning");

    /* Teardown */
}
END_TEST

Suite* motion (void) {
        Suite *suite = suite_create("motion");
        TCase *tcase = tcase_create("GW");

        tcase_add_test(tcase, test_init);

        tcase_add_test(tcase, test_get_current_position_GW001);
        tcase_add_test(tcase, test_get_current_position_BW001);

        tcase_add_test(tcase, test_move_to_GW001);
        tcase_add_test(tcase, test_move_to_BW001);

        tcase_add_test(tcase, test_get_distance_GW001);
        tcase_add_test(tcase, test_get_distance_BW001);

        tcase_add_test(tcase, test_get_direction_GW001);
        tcase_add_test(tcase, test_get_direction_BW001);

        tcase_add_loop_test (tcase, test_get_goal_direction_GW001, 0, NR_GET_DEG_TESTS);
        tcase_add_test(tcase, test_get_goal_direction_BW001);

        tcase_add_test(tcase, test_update_direction_GW001);
        tcase_add_test(tcase, test_update_direction_GW002);

        tcase_add_test(tcase, test_update_movement_GW001);
        tcase_add_test(tcase, test_update_movement_BW001);

        suite_add_tcase(suite, tcase);
        return suite;
}

int main (int argc, char *argv[]) {
        int number_failed;
        Suite *suite = motion();
        SRunner *runner = srunner_create(suite);

        srunner_run_all(runner, CK_NORMAL);
        number_failed = srunner_ntests_failed(runner);
        srunner_free(runner);

        return number_failed;
}
