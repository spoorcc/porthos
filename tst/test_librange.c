#include <check.h>
#include <math.h>
#include <stdio.h>

#include "range.h"

#define CALL(x) ck_assert_msg((0 == (x)), "Should succeed");
#define ck_assert_dbl_eq_msg(X, Y, msg) ck_assert_msg((fabs((X)-(Y)) < 1e-6), msg);

/*! \brief Test initialization

Test initialization of Range library
*/
START_TEST (test_init)
{
    CALL(range_init());
}
END_TEST

/*! \brief Test scheduling & retrieving result
*/
START_TEST (test_schedule_and_retrieve_GW001)
{
    unsigned char actual_result  = 0;

    /* Setup */
    CALL( range_init());
    range_scan_result = 42;

    /* Execute */
    CALL(range_schedule_scan());
    CALL(range_get_result(&actual_result));

    /* Verify */
    ck_assert_int_eq(actual_result, range_scan_result);

    /* Teardown */
}
END_TEST


/*! \brief Test retrieving result without schedule
*/
START_TEST (test_schedule_and_retrieve_BW001)
{
    int result = (int) RANGE_OK;
    unsigned char actual_result  = 0;

    /* Setup */
    CALL( range_init());

    /* Execute */
    result = range_get_result(&actual_result);

    /* Verify */
    ck_assert(result == (int) RANGE_NO_REQUEST_ERROR);

    /* Teardown */
}
END_TEST


/*! \brief Test retrieving result without schedule
*/
START_TEST (test_schedule_and_retrieve_BW002)
{
    int result = (int) RANGE_OK;

    /* Setup */
    CALL( range_init());

    /* Execute */
    result = range_get_result(NULL);

    /* Verify */
    ck_assert(result == (int) RANGE_PARAMETER_ERROR);

    /* Teardown */
}
END_TEST

Suite* range (void) {
        Suite *suite = suite_create("range");
        TCase *tcase = tcase_create("GW");

        tcase_add_test(tcase, test_init);

        tcase_add_test(tcase, test_schedule_and_retrieve_GW001);
        tcase_add_test(tcase, test_schedule_and_retrieve_BW001);
        tcase_add_test(tcase, test_schedule_and_retrieve_BW002);

        suite_add_tcase(suite, tcase);
        return suite;
}

int main (int argc, char *argv[]) {
        int number_failed;
        Suite *suite = range();
        SRunner *runner = srunner_create(suite);

        srunner_run_all(runner, CK_NORMAL);
        number_failed = srunner_ntests_failed(runner);
        srunner_free(runner);

        return number_failed;
}
