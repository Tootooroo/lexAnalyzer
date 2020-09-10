/* file: tools.h */

#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "type.h"

void * zMalloc(size_t size);
void   zFree(void **);

#define isAlphabet(c) \
    (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))

#endif /* _TOOLS_H_ */
