
#ifndef GIT_H
#define GIT_H



typedef struct {
    int clean;
    int ahead;
    int behind;
    int modified;
    int staged;
    int untracked;
} GitStatus;


GitStatus git_get_status(const char *path);

int git_add(const char *path);
int git_commit(const char *path, const char *message);
int git_pull(const char *path);
int git_push(const char *path);


#endif
