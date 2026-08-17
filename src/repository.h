#ifndef REPOSITORY_H
#define REPOSITORY_H
#include <stddef.h>
typedef struct {
    char *path;
    char *name;
} Repository;

typedef struct {
    Repository *items;
    size_t count;
    size_t capacity;
} RepositoryList;

void repository_list_init(RepositoryList *list);
void repository_list_free(RepositoryList *list);
int repository_list_add(RepositoryList *list, const char *path);
int repository_list_scan(RepositoryList *list, const char *root);
#endif
