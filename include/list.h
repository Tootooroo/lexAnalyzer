/* list.h */

#ifndef _LIST_H_
#define _LIST_H_

#include "type.h"

typedef struct listNode {
    void *value;
    struct listNode *prev;
    struct listNode *next;
} listNode;

typedef struct list {
    listNode *node;
    listNode *tail;
    /* Action(release, _Status_t, OK, ERROR) */
    _Status_t (*release)(void *);
    /* Predicate(match) */
    _Bool (*match)(const void *, const void *);
    /* Action(dup, void *, NON_NULL, NULL) */
    void * (*dup)(void *);
    /* Predicate(relation) */
    _Bool (*relation)(const void *, const void *);
} list;

typedef enum {
    LITER_FORWARD,
    LITER_BACKWARD
} LITER_DIR;

typedef struct listIter {
    listNode *node;
    list *l;
    LITER_DIR dir;
} listIter;

/* Member function implemented as macros */
#define isListDirValid(dir) ((dir) == LITER_FORWARD || (dir) == LITER_BACKWARD)
#define listGetNode(l) ((l)->node)
#define listSetNode(l, n) ((l)->node = n)
#define listGetTail(l) ((l)->tail)
#define listSetTail(l, t) ((l)->tail = t)
#define listGetHead(l) ((l)->node)
#define listSetReleaseMethod(l, m) ((l)->release = m)
#define listSetMatchMethod(l, m) ((l)->match = m)
#define listSetDupMethod(l, m) ((l)->dup = m)

/* list Prototypes */
list *      listCreate(void);
_Status_t   listRelease(list *l);
_Status_t   listPush(list *l, void *value);
void     *  listPop(list *l);
_Status_t   listAppend(list *l, void *value);
void     *  listRetrive(list *l);
_Status_t   listJoin(list *l, list *r);
_Status_t   listDelNode(list *l, void *key);
_Status_t   listSort_step(list *l);
_Status_t   listSort(list *l);
list *      listDup(const list *l);
listNode *  listSearch(const list *, const void *key);
void     *  listSearch_v(const list *, const void *key);
listNode *  listPrev(listIter *);
void     *  listPrev_v(listIter *);
listNode *  listNext(listIter *);
void     *  listNext_v(listIter *);
_Status_t   listRewind(listIter *);
_Bool       listIsEqual(list *l, list *r);

_Bool    listIsIterValid(listIter i);
listIter listGetIter(list *l, LITER_DIR dir);
listIter listSuccessor(listIter i);
listIter listPredecessor(listIter i);

#ifdef _TEST_LAB_UNIT_TESTING_

void list_Basic(void **state);

#endif /* _TEST_LAB_UNIT_TESTING_ */


#endif /* ifndef _LIST_H_ */

