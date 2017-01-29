#include <check.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "porthos/simulator.hpp"

#define CALL(x) ck_assert_msg((0 == (x)), "Should succeed");
#define ck_assert_dbl_eq_msg(X, Y, msg) ck_assert_msg((fabs((X)-(Y)) < 1e-6), msg);


/*! \brief Test initialization

Test initialization of Simulator
*/
START_TEST (test_init)
{
    /* Execute */
    CALL(simulator_init());
}
END_TEST


Suite* simulator (void) {
        Suite *suite = suite_create("simulator");
        TCase *tcase = tcase_create("GW");

        tcase_add_test(tcase, test_init);

        suite_add_tcase(suite, tcase);
        return suite;
}

int main (int argc, char *argv[]) {
        int number_failed;
        Suite *suite = simulator();
        SRunner *runner = srunner_create(suite);

        srunner_set_fork_status(runner, CK_NOFORK);

        srunner_run_all(runner, CK_NORMAL);
        number_failed = srunner_ntests_failed(runner);
        srunner_free(runner);

        return number_failed;
}
