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

Suite* motion (void) {
        Suite *suite = suite_create("motion");
        TCase *tcase = tcase_create("GW");

        tcase_add_test(tcase, test_init);

        tcase_add_test(tcase, test_get_current_position_GW001);
        tcase_add_test(tcase, test_get_current_position_BW001);

        tcase_add_test(tcase, test_move_to_GW001);
        tcase_add_test(tcase, test_move_to_BW001);

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
