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
#include <setjmp.h>
#include <string.h>

/* Private Prototypes */
private void doLeftParen(Regex *root, char **pChar);
private void doVertical(Regex *root);
private char * doCharacters(Regex *root, char **pChar);
private Regex * _regexParse(Regex *root, char **regex_str);
private void doStart(Regex *root);
private void doPlus(Regex *root);
private void attach(Regex *root, Regex *node);

/* Private Variables */
private Regex *regexTree;
private jmp_buf regex_entry;

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

    /* Setjmp for error handling */
    if (setjmp(regex_entry)) {
        return NULL;
    }

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
            /* Ignore '(' */
            ++current;
            doLeftParen(root, &current);
            break;
        case ')':
            goto PARSE_DONE;
        case '[':
            longjmp(regex_entry, 1);
            break;
        case ']':
            longjmp(regex_entry, 1);
            break;
        case '*':
            doStart(root);
            break;
        case '+':
            doPlus(root);
            break;
        default:
            /* Chaaracters */
            doCharacters(root, &current);
            continue;
        }
        ++current;
    }

 PARSE_DONE:
    *regex_str = current;
    return REGEX_LEFT(root);
}

/* Private Procedures */

/* Operator specific attach */

/* Function that attach node to root_node
 * root_node should be an operator node */
private void attach_to_operator_node(Regex *root, Regex *node) {
    Regex *op_node, *root_node = REGEX_LEFT(root);

    switch (REGEX_OP(node)) {
    case REGEX_OP_CHAR:
        op_node = regexCreate(REGEX_OP_CONCATE, NULL);
        REGEX_SET_LEFT(op_node, root_node);
        REGEX_SET_RIGHT(op_node, node);
        REGEX_SET_LEFT(root, op_node);
        break;
    case REGEX_OP_START:
    case REGEX_OP_PLUS:
    case REGEX_OP_ALTERNATE:
        REGEX_SET_LEFT(node, root_node);
        REGEX_SET_LEFT(root, node);
        break;
    }
}

private void attach_to_char_node(Regex *root, Regex *node) {
    Regex *op, *root_node = REGEX_LEFT(root);

    switch (REGEX_OP(node)) {
    case REGEX_OP_CHAR:
        op = regexCreate(REGEX_OP_CONCATE, NULL);
        attach(root, op);
        attach(root, node);
        return;
    case REGEX_OP_START:
        op = regexCreate(REGEX_OP_START, NULL);
        break;
    case REGEX_OP_PLUS:
        op = regexCreate(REGEX_OP_PLUS, NULL);
        break;
    case REGEX_OP_ALTERNATE:
        op = regexCreate(REGEX_OP_ALTERNATE, NULL);
        break;
    case REGEX_OP_CONCATE:
        op = regexCreate(REGEX_OP_CONCATE, NULL);
    }

    /* Attach operator node to root */
    REGEX_SET_LEFT(op, root_node);
    REGEX_SET_LEFT(root, op);
}

private void attach(Regex *root, Regex *node) {
    Regex *root_node = REGEX_LEFT(root);

    if (root_node == NULL) {
        /* Empty tree */
        REGEX_SET_LEFT(root, node);
    } else if (REGEX_OP(root_node) != REGEX_OP_CHAR &&
               REGEX_RIGHT(root_node) == NULL) {
        /* Incompleted operator node */
        REGEX_SET_RIGHT(root_node, node);
    } else if (REGEX_OP(root_node) == REGEX_OP_CHAR) {
        /* Character node */
        attach_to_char_node(root, node);
    } else {
        /* Completed operator node */
        attach_to_operator_node(root, node);
    }
}

private void doLeftParen(Regex *root, char **pChar) {
    Regex *subRoot = regexCreate(REGEX_OP_ROOT, NULL);

    _regexParse(subRoot, pChar);

    Regex *subTree = REGEX_LEFT(subRoot),
          *tree = REGEX_LEFT(root);

    // Empty subtree so tree structure will not
    // affected by it just return.
    if (subTree == NULL) {
        return;
    }

    // Mark subtree as a group
    subTree->isGroup = true;

    attach(root, subTree);
}

private void doStart(Regex *root) {
    Regex *node = regexCreate(REGEX_OP_START, NULL);
    attach(root, node);
}

private void doPlus(Regex *root) {
    Regex *node = regexCreate(REGEX_OP_PLUS, NULL);
    attach(root, node);
}

private void doVertical(Regex *root) {
    Regex *node = regexCreate(REGEX_OP_ALTERNATE, NULL);
    attach(root, node);
}

private char * doCharacters(Regex *root, char **pChar) {
    char c = **pChar;
    char *start = *pChar, *end = *pChar;

    while (isAlphabet(c)) {
        c = *(++end);
    }

    /* Update scan pointer position */
    *pChar = end;

    /* Create character node and attach to tree. */
    int len = end - start;
    char *str = (char *)calloc(1, len+1);
    strncpy(str, start, len);
    attach(root, regexCreate(REGEX_OP_CHAR, str));

    return end;
}
