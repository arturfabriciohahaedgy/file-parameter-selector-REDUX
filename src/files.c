#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "files.h"

#define FILE 1
#define FOLDER 0
#define BUFFER_SIZE 256

void printarguments(struct Arguments *arg);
int islevel(char*);
int filetype(const char*);
void addargument(struct Arguments*, char*);
char *returnlevel(int);

void
printarguments(struct Arguments *arg)
/* debug function to print all the arguments in the array */
{
	int len = arg->used;
	printf("===============================================\n");
	printf("us: %d\n", len);
	for (int i = 0; i < len; i++) {
		printf("i: %d, arg: %s\n", i, arg->array[i]);
	}
	printf("===============================================\n");
}

int
islevel(char *argument)
/* returns LEVEL(0) if *argument has the string used to indicate levels, returns FILE(1) if not */
{
	if(strncmp(argument, "||LEVEL_", 8) == LEVEL)
		return LEVEL;
	else
		return NOTLEVEL;
}

int
filetype(const char *path)
/* return if file on given path is FILE(1) or FOLDER(0) */
{
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISREG(path_stat.st_mode);
}

char *
returnlevel(int number)
/* adds an indication of the dirlevel with a very specific string which no sane person would use to name directories */
{
	const char   *prefix;
	char          charnumber;
	char         *concatbuffer;

	concatbuffer = 0;
	/* allocs non-local memory */
	concatbuffer = malloc(BUFFER_SIZE * sizeof(char*));
	/* prefix for string concat */
	prefix = "||LEVEL_";
	strcpy(concatbuffer, prefix);
	/* turns int to char */
	charnumber = '0' + number;
	strncat(concatbuffer, &charnumber, 1);
	strcat(concatbuffer, "||");

	return concatbuffer;
}

int
getlevel(char *level)
/* converts "||LEVEL[number]||" string to it's respecitve integer, otherwise returns NOTLEVEL(99) to indicate that it's a file */
{
	int numberlevel;

	if (strncmp(level, "||LEVEL_", 8) == LEVEL) {
		numberlevel = level[8] - '0';
		return numberlevel;
	} else
		return NOTLEVEL;
}

void
addargument(struct Arguments *arg, char *value)
/* allocates non-local memory, copies the value passed, inserts the non-local value into array. */
{
	char *insertedvalue;
	insertedvalue = malloc(BUFFER_SIZE * sizeof(char*));
	strcpy(insertedvalue, value);
	arg->array[arg->used] = insertedvalue;
	arg->used++;
}

void
freearguments(struct Arguments *a)
/* free values inside array */
{
	int us = a->used;
	for (int i = 0; i < us; i++)
		free(a->array[i]);
	a->used = 0;
}


void
returndirs(const char* basepath, int indentlevel, struct Arguments *arg)
/* recursively parse directories and keep track of hierarchy */
{
	char           path[BUFFER_SIZE];
	struct dirent *entry;
	DIR           *folder;
	char          *filename;
	char          *dirlevel;

	dirlevel = returnlevel(indentlevel);
	addargument(arg, dirlevel);

    folder = opendir(basepath);
    if (folder == NULL) {
        fprintf(stderr, "ERROR: Could not open selected directory %s: %d\n", basepath, errno);
    }

    while ((entry = readdir(folder))) {
		filename = entry->d_name;
		/* us = arg->used; */
		if (filename[0] != '.') {
			strcpy(path, basepath);
			/* if it's on windows, concat the "\" path separator, if on Linux/BSD use "/" */
			#ifdef __WIN32
			strcat(path, "\\");
			#elif __unix__
			strcat(path, "/");
			#endif
			strcat(path, filename);
			if (filetype(path) == FOLDER) {
				printf("path: %s\n", path);
				addargument(arg, path);
				returndirs(path, indentlevel+1, arg);
				dirlevel = returnlevel(indentlevel);
				addargument(arg, dirlevel);
			}
		}
	}
	closedir(folder);
}

