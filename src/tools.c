/* file: tools.c */

#include "tools.h"
#include <string.h>

/* Return an initialized memory buffer */
void * zMalloc(size_t size) {
    if (size <= 0) return null;

    void *mem = malloc(size);
    if (isNull(mem)) {
        printf("Malloc error\n");
        exit(1);
    }
    memset(mem, 0, size);

    return mem;
}

void zFree(void **ptr) {
    if (ptr == NULL)
        return;

    free(*ptr);
    *ptr = NULL;
}
