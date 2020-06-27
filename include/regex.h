/* file: regex.h */

#include "list.h"

typedef enum RegexOp {
    REGEX_OP_ROOT,
    REGEX_OP_CONCATE,
    REGEX_OP_ALTERNATE,
    REGEX_OP_CHAR,
    REGEX_OP_CHAR_CLASS,
    REGEX_OP_START,
    REGEX_OP_PLUS,
    REGEX_OP_ANY_CHAR,
    REGEX_OP_LEFT_PAREN,
    REGEX_OP_RIGHT_PAREN,
    REGEX_OP_VERTICAL_BAR
} RegexOp;

typedef struct Regex {
    RegexOp op;

    char *str;
    // Reference to the parent of current regex
    struct Regex *parent;
    struct Regex *left;
    struct Regex *right;

    // Regex Stack index
    struct Regex *prev;
    struct Regex *next;
} Regex;


/* Member functions implement as macros */
#define REGEX_OP(R) ((R)->op)
#define REGEX_SET_OP(R, OP) ((R)->op = (OP))

#define REGEX_STR(R) ((R)->str)
#define REGEX_SET_STR(R, S) ((R)->str = (S))

#define REGEX_PARENT(R) ((R)->parent)
#define REGEX_SET_PARENT(R, P) ((R)->up = (P))

#define REGEX_LEFT(R) ((R)->left)
#define REGEX_SET_LEFT(R, L) ((R)->left = (L))

#define REGEX_RIGHT(R) ((R)->left)
#define REGEX_SET_RIGHT(R, R_CHILD) ((R)->right = (R_CHILD))

/* Prototypes */
Regex * regexCreate(RegexOp op, char *str);
Regex * regexDuplicate(Regex *r);
void    regexDestruct(Regex *r);

// Use Regex as as stack

// Regex parse function
Regex * regexParse(char *regex_str);
void regexTestCases(void **state);
