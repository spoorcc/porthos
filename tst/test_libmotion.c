#include <check.h>
#include <stdio.h>

#include "motion.h"

START_TEST (test_init)
{
    motion_init();
}
END_TEST

Suite* motion (void) {
        Suite *suite = suite_create("motion");
        TCase *tcase = tcase_create("GW");

        tcase_add_test(tcase, test_init);

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
