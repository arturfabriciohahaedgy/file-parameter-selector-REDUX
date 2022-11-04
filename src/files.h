#include <stdlib.h>

#define LEVEL 0
#define NOTLEVEL 99
#define DIR_ARRAY_SIZE 256

struct Arguments {
	int   used;
	char *array[DIR_ARRAY_SIZE];
};

void returndirs(const char*, int, struct Arguments *arg);
void freearguments(struct Arguments *arg);

extern struct Arguments arg;