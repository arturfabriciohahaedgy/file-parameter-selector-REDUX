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

void printarguments(struct Arguments*);
int islevel(char*);
int filetype(const char*);
void addargument(struct Arguments*, char*);
char *returnlevel(int);

void
printarguments(struct Arguments *a)
/* debug function to print all the arguments in the dynamic array */
{
    int us = a->used;
    printf("===============================================\n");
    printf("us: %d\n", us);
    for (int i = 0; i < us; i++) {
        printf("i: %d, arg: %s\n", i, a->array[i]);
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

void
addargument(struct Arguments *arg, char *insertvalue)
/* Adds *insertvalue in argument array, double the size of the array if it is at max */
{
    printarguments(arg);
    if (arg->used == arg->size) {
        arg->size *= 2;
        arg->array = realloc(arg->array, arg->size * sizeof(char*));
    }
    arg->array[arg->used++] = insertvalue;
}

char *
returnlevel(int number)
/* Adds an indication of the dirlevel with a very specific string which no sane person would use to name directories */
{
    const char   *prefix;
    char    charnumber;
    char   *concatbuffer;

    concatbuffer = 0;
    /* allocs non-local memory */
    concatbuffer = (char*)malloc(BUFFER_SIZE * sizeof(char*));

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
returndir(void)
/* TODO: Remove function after project is finalized */
{
    char buffer[BUFFER_SIZE];
    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        printf("Current working dir: %s\n", buffer);
    } else {
        perror("getcwd() error");
    }
}

void
initarguments(struct Arguments *a, size_t initialsize)
/* initialize dynamic array */
{
    a->array = malloc(initialsize * sizeof(char*));
    a->used = 0;
    a->size = initialsize;
}

void
freearguments(struct Arguments *a)
/* free dynamic array */
{
    int us = a->used;

    for (int i = 0; i < us; i++) {
        if((islevel(a->array[i])) == 0)
            free(a->array[i]);
    }

    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void
resolvepath(const char* basepath, int indentlevel, struct Arguments *arg)
/* recursively parse directories and keep track of hierarchy */
/* TODO: Fix path parsing issue */
{
    char           path[BUFFER_SIZE];
    struct dirent *entry;
    DIR           *folder;
    char          *filename;
    char          *dirlevel;

   	dirlevel = returnlevel(indentlevel);
    printf("dirlevel: %s\n", dirlevel);
    addargument(arg, dirlevel);

    folder = opendir(basepath);
    if (folder == NULL) {
        fprintf(stderr, "ERROR: Could not open selected directory %s: %d", basepath, errno);
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
            if (filetype(path) == FILE) {
                addargument(arg, filename);
            }
            else if (filetype(path) == FOLDER) {
                addargument(arg, filename);
                resolvepath(path, indentlevel+1, arg);
            }
        }
    }
    closedir(folder);
}

