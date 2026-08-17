#include "repository.h"
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>







static int scan_directory(RepositoryList *list, const char *path);


void 
repository_list_init(RepositoryList *list)
{
    list->items = NULL;
    list->count = 0;
    list->capacity=0;
}
void
repository_list_free(RepositoryList *list)
{
    for (size_t i = 0; i < list->count ; i++)
    {
        free(list->items[i].path);
        free(list->items[i].name);
    }
    free(list->items);

    list->items = NULL;
    list->count=0;
    list->capacity=0;
}

int repository_list_add(RepositoryList *list, const char *path)
{
    if (list->count == list->capacity) {
        size_t new_capacity;

        if (list->capacity == 0)
            new_capacity = 8;
        else
            new_capacity = list->capacity * 2;

        Repository *new_items = realloc(
            list->items,
            new_capacity * sizeof(Repository)
        );

        if (new_items == NULL)
            return -1;

        list->items = new_items;
        list->capacity = new_capacity;
    }

    Repository *repository = &list->items[list->count];

    repository->path = strdup(path);

    if (repository->path == NULL)
        return -1;

    const char *name = strrchr(path, '/');

    if (name != NULL)
        name++;
    else
        name = path;

    repository->name = strdup(name);

    if (repository->name == NULL) {
        free(repository->path);
        return -1;
    }

    list->count++;

    return 0;
}

static int
scan_directory(RepositoryList *list, const char *path)
{
    DIR *directory;
    struct dirent *entry;

    directory = opendir(path);

    if (directory == NULL)
        return -1;

    while ((entry = readdir(directory)) != NULL) {
        /* Ignore . and .. */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;
        size_t path_length = strlen(path);
        size_t name_length = strlen(entry->d_name);

        char *full_path = malloc(path_length + 1 + name_length + 1);

        if (full_path == NULL) {
            closedir(directory);
            return -1;
        }

    snprintf(
        full_path,
        path_length + 1 + name_length + 1,
        "%s/%s",
        path,
        entry->d_name
        );
    struct stat info;

    if (stat(full_path, &info) != 0) {
        free(full_path);
        continue;
    }

    if (S_ISDIR(info.st_mode)) {
        char *git_path = malloc(strlen(full_path) + 6);

    if (git_path == NULL) {
        free(full_path);
        closedir(directory);
        return -1;
    }

    snprintf(
        git_path,
        strlen(full_path) + 6,
        "%s/.git",
        full_path
        );      
    struct stat git_info;

        if (stat(git_path, &git_info) == 0) {
            repository_list_add(list, full_path);
        }   else {
                scan_directory(list, full_path);
            }

        free(git_path);
        }

        free(full_path);
        }

    closedir(directory);

    return 0;
}
int repository_list_scan(RepositoryList *list, const char *root)
{
    return scan_directory(list,root);
}

