/* MIT License
 *
 * Copyright (c) 2020 Tootooroo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom t*he Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "type.h"
#include "regex.h"
#include "check.h"

START_TEST(REGEX_CHAR) {
    Regex *tree = regexParse("abc");

    ck_assert_int_eq(REGEX_OP_CHAR, REGEX_OP(tree));
    ck_assert_str_eq("abc", REGEX_STR(tree));
}
END_TEST

START_TEST(REGEX_ALTERNATE) {
    Regex *tree = regexParse("a|b");
}

Suite *regex_suite(void) {
    Suite *suite = suite_create("Regex");
    TCase *tcases = tcase_create("REGEX_TESTCASES");

    tcase_add_test(tcases, REGEX_CHAR);
    suite_add_tcase(suite, tcases);

    return suite;
}

int main(void) {
    Suite *suite = regex_suite();
    SRunner *runner = srunner_create(suite);

    srunner_set_fork_status(runner, CK_NOFORK);
    srunner_run_all(runner, CK_NORMAL);
    return 0;
}
