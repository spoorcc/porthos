#include <check.h>
#include <math.h>
#include <stdio.h>

#include "mapper.h"

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
    CALL(mapper_init(1.0, 1.0, 2));

    CALL(mapper_add_point(.75, .25, MAPPER_BLOCKED));

    CALL(mapper_print_map());

}
END_TEST

Suite* mapper (void) {
        Suite *suite = suite_create("range");
        TCase *tcase = tcase_create("GW");

        tcase_add_test(tcase, test_init);
        tcase_add_test(tcase, test_add_point);

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
