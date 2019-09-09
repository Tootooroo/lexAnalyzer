/* type.h */

                                                \

#ifndef _TEST_LAB_TYPE_
#define _TEST_LAB_TYPE_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef null
#define null NULL
#endif

#ifndef ok
#define OK (0)
#endif

#ifndef ERROR
#define ERROR (1)
#endif

#ifndef private
#define private static
#endif

enum {
    Done = 2
};

typedef int _Status_t;
typedef unsigned int _Index_t;

#define isNull(v) ((v) == null)
#define isNonNull(v) ((v) != null)

#define errorMsg(msg, args...) fprintf(stderr, msg, ##args)
#define abortWithMsg(msg, args...) ({ errorMsg(msg, ##args); exit(1); })

#define container_of(ptr, ConType, member) ({\
        const typeof( ((ConType *)(0))->member) *__mptr = ptr;\
        (ConType *)((char *)__mptr - offsetof(ConType, member));\
})

#endif /* _TEST_LAB_TYPE_ */

