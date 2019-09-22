/* file: regex.c */

#include "regex.h"
#include "tools.h"
#include <string.h>

/* Private Prototypes */
private void doCollapse(RegexOp op);
private void doVerticalBar();
private void doLeftParen();
private void doAlternation();
private void doRightParen();

/* Private Variables */
Regex *stack_top;

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

    if (REGEX_SUBS(r) != NULL)
        listRelease(REGEX_SUBS(r));
}

_Status_t regexAddSub(Regex *sub) {
    if (REGEX_SUBS(stack_top) == NULL)
        REGEX_SET_SUBS(stack_top, listCreate());
    listAppend(REGEX_SUBS(stack_top), sub);
    return OK;
}

_Status_t regexPushSub(Regex *sub) {
    if (REGEX_SUBS(stack_top) == NULL)
        REGEX_SET_SUBS(stack_top, listCreate());
    listPush(REGEX_SUBS(stack_top), sub);
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
            doVerticalBar();
            break;
        case '(':
            doLeftParen();
            break;
        case ')':
            doRightParen();
            break;
        case '[':
            break;
        case '*':
            break;
        case '+':
            break;
        default:
            break;
        }

    }

    return re;
}

/* Private Procedures */

/* Collapse collection of continuous regex into one regex tree
 * with specified operation */
private void doCollapse(RegexOp op) {
    Regex *current = stack_top, *next;

    Regex *new = regexCreate(op, NULL);

    while (current != NULL && !REGEX_OP_IS_MARKER(REGEX_OP(stack_top))) {
        if (REGEX_OP(current) == REGEX_OP_CONCATE) {
            listJoin(current->subs, new->subs);
            new->subs = current->subs;

            next = REGEX_DOWN(current);
            free(current);
        } else {
            regexPushSub(current);

            next = REGEX_DOWN(current);
        }
        current = next;
    }
}

private void doConcatenation(Regex *top) {
    if (top == NULL || REGEX_OP_IS_MARKER(REGEX_OP(top))) {
        return;
    }
    doCollapse(REGEX_OP_CONCATE);
}

private void doAlternation() {
    doConcatenation(stack_top);

    if (stack_top == NULL || REGEX_OP_IS_MARKER(REGEX_OP(stack_top))) {
        return;
    }

    doCollapse(REGEX_OP_ALTERNATE);
}

private void doVerticalBar() {
    doConcatenation(stack_top);

    Regex *re1 = stack_top, *re2 = REGEX_DOWN(stack_top);

    /* Situation: REGEXS VERTICAL REGEX(Concated)
     *
     * Swap between VERTICAL BAR and regex concated so
     * regexs below vertical bar is a list of regex will be deal with
     * alternation. */
    if (re1 != NULL && (re2 != NULL && REGEX_OP(re2) == REGEX_OP_VERTICAL_BAR)) {
        REGEX_SET_DOWN(re1, REGEX_DOWN(re2));
        REGEX_SET_DOWN(re2, re1);
        stack_top = re2;
        return;
    }

    /* Situation: REGEX
     *
     * Just push a vertical bar onto the top of stack */
    REGEX_PUSH(stack_top, regexCreate(REGEX_OP_VERTICAL_BAR, NULL));
}

private void doLeftParen() {
    REGEX_PUSH(stack_top, regexCreate(REGEX_OP_LEFT_PAREN, NULL));
}

private void doRightParen() {
    doAlternation();

    /* Remove  */
    Regex *re1 = stack_top, *re2 = REGEX_DOWN(re1);

    if (re1 != NULL && REGEX_OP(re1) < REGEX_OP_LEFT_PAREN &&
        re2 != NULL && REGEX_OP(re2) == REGEX_OP_LEFT_PAREN) {

        REGEX_SET_DOWN(re1, REGEX_DOWN(re2));
    } else if (re1 != NULL && REGEX_OP(re1) == REGEX_OP_LEFT_PAREN) {
        stack_top = stack_top->down;
    } else {
        /* Error regular expression */
        abortWithMsg("Non-paired left paren and right paren");
    }
    regexDestruct(re2);
}
