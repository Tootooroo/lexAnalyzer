/* file: regex.h */

#include "list.h"

typedef enum RegexOp {
    REGEX_OP_CONCATE,
    REGEX_OP_ALTERNATE,
    REGEX_OP_CHAR,
    REGEX_OP_CHAR_CLASS,
    REGEX_OP_START,
    REGEX_OP_PLUS,
    REGEX_OP_ANY_CHAR,
    REGEX_OP_RIGHT_PAREN,
    REGEX_OP_LEFT_PAREN,
    REGEX_OP_VERTICAL_BAR
} RegexOp;

typedef struct Regex {
    RegexOp op;

    char *str;
    // Reference to the parent of current regex
    struct Regex *upper;
    // Reference to next of the current position
    // of regex stack
    struct Regex *down;
    list *subs;
} Regex;


/* Member functions implement as macros */
#define REGEX_OP(R) ((R)->op)
#define REGEX_SET_OP(R, OP) ((R)->op = (OP))

#define REGEX_STR(R) ((R)->str)
#define REGEX_SET_STR(R, S) ((R)->str = (S))

#define REGEX_DOWN(R) ((R)->down)
#define REGEX_SET_DOWN(R, D) ((R)->down = (D))

#define REGEX_SUBS(R) ((R)->subs)
#define REGEX_SET_SUBS(R, S) ((R)->subs = (S))

#define REGEX_UPPER(R) ((R)->upper)
#define REGEX_SET_UPPER(R, U) ((R)->upper = (U))

#define REGEX_OP_IS_MARKER(OP) ((OP) > REGEX_OP_RIGHT_PAREN)

#define REGEX_PUSH(R, R1) ((R1)->down = (R), (R) = (R1), 0)
#define REGEX_POP(R) ({\
    Regex *poped = R;\
    (R) = (R)->down;\
    poped->down = NULL; \
    poped;\
})

/* Prototypes */
Regex * regexCreate(RegexOp op, char *str);
Regex * regexDuplicate(Regex *r);
void    regexDestruct(Regex *r);
_Status_t regexAddSub(Regex *r, Regex *sub);
_Status_t regexPushSub(Regex *r, Regex *sub);

// Use Regex as as stack

// Regex parse function
Regex * regexParse(char *regex_str);
