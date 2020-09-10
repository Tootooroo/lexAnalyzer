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
#include "tools.h"
#include <string.h>

/* Private Prototypes */
private void
doLeftParen(Regex *root, char **pChar);
private void doVertical(Regex *root);
private char * doCharacters(Regex *root, char **pChar);
private Regex * _regexParse(Regex *root, char **regex_str);

/* Private Variables */
Regex *regexTree;

/* Public Procedures */
Regex * regexCreate(RegexOp op, char *str) {
    Regex *r = (Regex *)zMalloc(sizeof(Regex));

    REGEX_SET_OP(r, op);
    REGEX_SET_STR(r, str);
    REGEX_SET_LEFT(r, NULL);
    REGEX_SET_RIGHT(r, NULL);

    return r;
}

Regex * regexDuplicate(Regex *r) {
    if (r == NULL) return NULL;
    Regex *dup = regexCreate(REGEX_OP(r), strdup(REGEX_STR(r)));

    return dup;
}

void regexDestruct(Regex *r) {
    zFree((void **)&REGEX_STR(r));
    free(r);
}

Regex * regexParse(char *regex_str) {
    regexTree = regexCreate(REGEX_OP_ROOT, NULL);
    return _regexParse(regexTree, &regex_str);
}

private Regex * _regexParse(Regex *root, char **regex_str) {
    if (regex_str == NULL)
        return NULL;

    char *current = *regex_str;

    while (true) {
        char c = *current;

        if (c == '\0') break;

        switch (c) {
        case '|':
            doVertical(root);
            break;
        case '(':
            doLeftParen(root, &current);
            break;
        case ')':
            break;
        case '[':
            break;
        case '*':
            break;
        case '+':
            break;
        default:
            /* Chaaracters */
            doCharacters(root, &current);
            continue;
        }
        ++current;
    }

    return REGEX_LEFT(root);
}

/* Private Procedures */
private void attach(Regex *root, Regex *node) {
    Regex *root_node = REGEX_LEFT(root);

    if (root_node == NULL) {

    } else if (REGEX_RIGHT(root_node) == NULL) {

    } else {

    }
}

private void doLeftParen(Regex *root, char **pChar) {
    Regex *subRoot = regexCreate(REGEX_OP_ROOT, NULL);

    REGEX_SET_LEFT(subRoot, regexCreate(REGEX_OP_LEFT_PAREN, NULL));

    _regexParse(subRoot, pChar);

    Regex *subTree = REGEX_LEFT(subRoot),
          *tree = REGEX_LEFT(root);

    // Empty subtree so tree structure will not
    // affected by it just return.
    if (subTree == NULL) {
        return;
    }

    // Mark subtree as a group

    if (tree == NULL) {
        REGEX_SET_LEFT(root, subTree);
    } else {
        REGEX_SET_RIGHT(tree, subTree);
    }
}

private void doRightParen(Regex *root) {

}

private void doVertical(Regex *root) {
    Regex *root_node = REGEX_LEFT(root);
    Regex *r_vertical = regexCreate(REGEX_OP_VERTICAL_BAR, NULL);

    if (root_node == NULL) {
        REGEX_SET_LEFT(root, r_vertical);
    } else {
        REGEX_SET_LEFT(r_vertical, root_node);
        REGEX_SET_LEFT(root, r_vertical);
    }
}

#define isAlphabet(c) ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z')
private char * doCharacters(Regex *root, char **pChar) {
    char c = **pChar;
    char *start = *pChar, *end = *pChar;

    while (isAlphabet(c)) {
        c = *(++end);
    }
    *pChar = end;

    int len = end - start;
    char *str = (char *)malloc(len);

    strncpy(str, start, len);

    Regex *root_node = REGEX_LEFT(root);
    Regex *r = regexCreate(REGEX_OP_CHAR, str);

    if (root_node == NULL) {
        REGEX_SET_LEFT(root, r);
    } else if (REGEX_RIGHT(root_node) == NULL) {
        REGEX_SET_RIGHT(root_node, r);
    } else {
        abortWithMsg("doCharacters(): REGEX_RIGHT(root) is not NULL");
    }

    return end;
}


// Test Cases
void regexTestCases(void **state) {
    Regex *re = regexParse("abc");
}
