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

_Bool is_regex_tree_expected(Regex *tree, int *expected, int idx) {

    _Bool expected_current = REGEX_OP(tree) == expected[idx];

    if (REGEX_LEFT(tree) == NULL && REGEX_RIGHT(tree) == NULL) {
        return expected_current;
    }

    _Bool expected_left = is_regex_tree_expected(REGEX_LEFT(tree), expected, idx*2);
    _Bool expected_right = is_regex_tree_expected(REGEX_RIGHT(tree), expected, idx*2+1);

    return expected_current && expected_left && expected_right;
}

START_TEST(REGEX_CHAR) {
    Regex *tree = regexParse("abc");

    int tree_shape[1] = { REGEX_OP_CHAR };
    ck_assert(is_regex_tree_expected(tree, tree_shape, 0));
    ck_assert_str_eq("abc", REGEX_STR(tree));
}
END_TEST

START_TEST(REGEX_ALTERNATE) {

    /* Case 1 */
    Regex *tree = regexParse("a|b");

    /* Check op node */
    ck_assert_int_eq(REGEX_OP_ALTERNATE, REGEX_OP(tree));

    /* Check left child node */
    Regex *left = REGEX_LEFT(tree);
    ck_assert_int_eq(REGEX_OP_CHAR, REGEX_OP(left));
    ck_assert_str_eq("a", REGEX_STR(left));

    /* Check right child node */
    Regex *right = REGEX_RIGHT(tree);
    ck_assert_int_eq(REGEX_OP_CHAR, REGEX_OP(right));
    ck_assert_str_eq("b", REGEX_STR(right));

    /* Case 2 */
    tree = regexParse("a|b|c");

    /* Check op node */
    ck_assert_int_eq(REGEX_OP_ALTERNATE, REGEX_OP(tree));
    ck_assert_int_eq(REGEX_OP_ALTERNATE, REGEX_OP(REGEX_LEFT(tree)));

    /* Case 3 */
    tree = regexParse("|");

    /* Wrong regex expression */
    ck_assert_ptr_eq(tree, NULL);
}

START_TEST(REGEX_GROUP) {
    Regex *tree;

    /* Case 1 */
    tree = regexParse("(abc)");
    ck_assert_int_eq(REGEX_OP_CHAR, REGEX_OP(tree));
    ck_assert_int_eq(true, tree->isGroup);

    /* Case 2 */
    tree = regexParse("(abc)|(defg)");
    ck_assert_int_eq(REGEX_OP_ALTERNATE, REGEX_OP(tree));
    ck_assert_int_eq(REGEX_OP_CHAR, REGEX_OP(REGEX_LEFT(tree)));
    ck_assert_int_eq(true, REGEX_LEFT(tree)->isGroup);
    ck_assert_int_eq(REGEX_OP_CHAR, REGEX_OP(REGEX_RIGHT(tree)));
    ck_assert_int_eq(true, REGEX_RIGHT(tree)->isGroup);

    /* Case 3 */
    tree = regexParse("((a(bc)d)|(ef(g)h))");
    int expected[] =
        {
         REGEX_OP_ROOT,
         REGEX_OP_ALTERNATE, REGEX_OP_CONCATE, REGEX_OP_CONCATE,
         REGEX_OP_CONCATE, REGEX_OP_CHAR, REGEX_OP_CONCATE,
         REGEX_OP_CHAR, REGEX_OP_CHAR, REGEX_OP_CHAR, 0, 0,
         REGEX_OP_CHAR, REGEX_OP_CHAR
        };
    ck_assert(is_regex_tree_expected(tree, expected, 1));
}

Suite *regex_suite(void) {
    Suite *suite = suite_create("Regex");
    TCase *tcases = tcase_create("REGEX_TESTCASES");

    tcase_add_test(tcases, REGEX_CHAR);
    tcase_add_test(tcases, REGEX_ALTERNATE);
    tcase_add_test(tcases, REGEX_GROUP);
    suite_add_tcase(suite, tcases);

    return suite;
}

int main(void) {
    Suite *suite = regex_suite();
    SRunner *runner = srunner_create(suite);

    srunner_set_fork_status(runner, CK_NOFORK);
    srunner_run_all(runner, CK_NORMAL);
    int number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
