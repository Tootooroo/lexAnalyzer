/* file: regex.c */

#include "regex.h"
#include "tools.h"
#include <string.h>

/* Private Prototypes */
private void doVerticalBar(Regex *top);
private void doLeftParen(Regex *top);

/* Public Procedures */
Regex * regexCreate(RegexOp op, char *str) {
    Regex *r = (Regex *)zMalloc(sizeof(Regex));

    REGEX_SET_OP(r, op);
    REGEX_SET_STR(r, str);
    REGEX_SET_SUBS(r, NULL);

    return r;
}

Regex * regexDuplicate(Regex *r) {
    Regex *dup = regexCreate(REGEX_OP(r), strdup(REGEX_STR(r)));
    REGEX_SET_SUBS(dup, listDup(REGEX_SUBS(r)));

    return dup;
}

void regexDestruct(Regex *r) {
    zFree((void **)&REGEX_STR(r));
    listRelease(REGEX_SUBS(r));
}

_Status_t regexAddSub(Regex *r, Regex *sub) {
    listAppend(REGEX_SUBS(r), sub);
    return OK;
}

_Status_t regexPushSub(Regex *r, Regex *sub) {
    listPush(REGEX_SUBS(r), sub);
    return OK;
}

Regex * regexParse(char *regex_str) {
    if (regex_str == NULL)
        return NULL;

    char *current = regex_str;
    Regex *re = NULL;
    Regex *stackTop = NULL;

    while (current != NULL) {
        char c = *current;

        switch (c) {
        case '|':
            doVerticalBar(stackTop);
            break;
        case '(':
            doLeftParen(stackTop);
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
            REGEX_PUSH();
            break;
        }

    }

    return re;
}

/* Private Procedures */

/* Collapse collection of continuous regex into one regex tree
 * with specified operation */
private void doCollapse(Regex *top, RegexOp op) {
    Regex *current = top;

    Regex *new = regexCreate(op, NULL);

    while (current != NULL && !REGEX_OP_IS_MARKER(REGEX_OP(top))) {
        if (REGEX_OP(current) == REGEX_OP_CONCATE) {

        } else {
            regexPushSub(new, current);
        }
    }
}

private void doConcatenation(Regex *top) {
    if (top == NULL || REGEX_OP_IS_MARKER(REGEX_OP(top))) {
        return;
    }
    doCollapse(top, REGEX_OP_CONCATE);
}

private void doVerticalBar(Regex *top) {
    doConcatenation(top);
}

private void doLeftParen(Regex *top) {}
