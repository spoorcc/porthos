#include <check.h>
#include <stdio.h>

#include "motion.h"

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
    int result = MOTION_OK;
    position_t position = {0};

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

    /* Execute */
    result = motion_get_current_position(&position);

    /* Verify */
    fail_if(result != 0, "Should succeed");
    fail_if(position.x != 0.0, "Expected x-position 0.0");
    fail_if(position.y != 0.0, "Expected y-position 0.0");

    /* Teardown */
}
END_TEST


/*! \brief Test getting position - NULL parameter
*/
START_TEST (test_get_current_position_BW001)
{
    int result = MOTION_OK;

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

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
    int result = MOTION_OK;
    position_t position = {.x=42.0, .y=53.0};

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

    /* Execute */
    result = motion_move_to(&position);

    /* Verify */
    fail_if(result != 0, "Should succeed");

    /* Teardown */
}
END_TEST


/*! \brief Test setting position - NUL parameter
*/
START_TEST (test_move_to_BW001)
{
    int result = MOTION_OK;

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

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
    int result = MOTION_OK;
    position_t position = {.x=3.0, .y=4.0};
    float distance = 0.0;

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

    result = motion_move_to(&position);
    fail_if(result != 0, "Should succeed");

    /* Execute */
    result = motion_get_distance(&distance);

    /* Verify */
    fail_if(result != 0, "Should succeed");
    fail_if(distance != 5.0, "Should be 3.0, 4.0, 5.0 triangle");

    /* Teardown */
}
END_TEST

/*! \brief Test getting distance
*/
START_TEST (test_get_distance_BW001)
{
    int result = MOTION_OK;

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

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
    int result = MOTION_OK;
    position_t position = {.x=3.0, .y=4.0};
    float direction = 0.0;

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

    /* Execute */
    result = motion_get_direction(&direction);

    /* Verify */
    fail_if(result != 0, "Should succeed");
    fail_if(direction != 0.0, "Should be 0.0");

    /* Teardown */
}
END_TEST

/*! \brief Test getting distance
*/
START_TEST (test_get_direction_BW001)
{
    int result = MOTION_OK;

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

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
    result = motion_init();
    fail_if(result != 0, "Should succeed");

    result = motion_move_to(&get_goal_direction_GW_positions[_i]);
    fail_if(result != 0, "Should succeed");

    /* Execute */
    result = motion_get_goal_direction(&degrees);

    /* Verify */
    fail_if(result != 0, "Should succeed");

    fail_if(degrees != get_goal_direction_GW_degrees[_i], "Should be 135 degrees");

    /* Teardown */
}
END_TEST

/*! \brief Test getting goal direction
*/
START_TEST (test_get_goal_direction_BW001)
{
    int result = MOTION_OK;

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

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
    int result = MOTION_OK;
    position_t position = {.x=-2.0, .y=-2.0};
    float current_direction = 0.0;
    float goal_direction = 0.0;

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

    result = motion_move_to(&position);
    fail_if(result != 0, "Should succeed");

    /* Execute */
    result = motion_update_direction();
    fail_if(result != 0, "Should succeed");

    /* Verify */
    result = motion_get_direction(&current_direction);
    fail_if(result != 0, "Should succeed");

    result = motion_get_goal_direction(&goal_direction);
    fprintf(stderr, "\nDirection: Current=%f, Goal=%f\n", current_direction, goal_direction);

    fail_if(current_direction != -45.0, "Should be reduced to -45");

    /* Teardown */
}
END_TEST


/*! \brief Test updating direction
*/
START_TEST (test_update_direction_GW002)
{
    int result = MOTION_OK;
    position_t position = {.x=0.5, .y=1.0};
    float current_direction = 0.0;

    /* Setup */
    result = motion_init();
    fail_if(result != 0, "Should succeed");

    result = motion_move_to(&position);
    fail_if(result != 0, "Should succeed");

    /* Execute */
    result = motion_update_direction();
    fail_if(result != 0, "Should succeed");

    /* Verify */
    result = motion_get_direction(&current_direction);
    fail_if(result != 0, "Should succeed");
    fail_if((current_direction - 26.565) > 0.001, "Should be reduced to -90");

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
