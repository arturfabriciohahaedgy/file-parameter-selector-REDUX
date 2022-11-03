#include <stdlib.h>

#define LEVEL 0
#define NOTLEVEL 99

struct Arguments {
    char   **array;
    size_t   used;
    size_t   size;
}; /* struct used for dynamic array of filenames */

void initarguments(struct Arguments*, size_t);
void freearguments(struct Arguments*);

void resolvepath(const char*, int, struct Arguments*);

extern struct Arguments arg;