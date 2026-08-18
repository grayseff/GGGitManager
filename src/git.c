#include "git.h"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

GitStatus
git_get_status(const char *path)
{
    GitStatus status = {0};

    char command[512];
    snprintf(
        command,
        sizeof(command),
        "git -C \"%s\" status --porcelain",
        path
    );

    FILE *git = popen(command, "r");

    if (git == NULL)
        return status;

    char line[256];
    while (fgets(line, sizeof(line), git) != NULL) {
            if (line[0] == '?' && line[1] == '?') {
                status.untracked = 1;
            } else {
                if (line[0] != ' ')
                    status.staged = 1;

                if (line[1] != ' ')
                    status.modified = 1;
            }
        }

    pclose(git);
snprintf(
    command,
    sizeof(command),
    "git -C \"%s\" rev-parse --verify HEAD >/dev/null 2>&1",
    path
);

if (system(command) != 0) {
    status.ahead = 0;
    status.behind = 0;
    return status;
}

snprintf(
    command,
    sizeof(command),
    "git -C \"%s\" rev-list --left-right --count HEAD...@{upstream}",
    path
);

git = popen(command, "r");

if (git != NULL) {
    if (fscanf(git, "%d %d", &status.ahead, &status.behind) != 2) {
        status.ahead = 0;
        status.behind = 0;
    }

    pclose(git);
}

if (!status.staged &&
    !status.modified &&
    !status.untracked) {
    status.clean = 1;
}

return status;
   }
//--------------------------------------------------------------------------------------------------------//
int
git_add(const char *path)
{
    char command[512];

    snprintf(
        command,
        sizeof(command),
        "git -C \"%s\" add -A",
        path
    );

    return system(command) == 0 ? 0 : -1;
}
//--------------------------------------------------------------------------------------------------------//
int
git_commit(const char *path, const char *message)
{
    char command[1024];

    snprintf(
        command,
        sizeof(command),
        "git -C \"%s\" commit -m \"%s\"",
        path,
        message
    );

    return system(command) == 0 ? 0 : -1;
}
//----------------------------------------------------------------------------------------------------//
int
git_pull(const char *path)
{
    char command[512];

    snprintf(
        command,
        sizeof(command),
        "git -C \"%s\" pull",
        path
    );

    return system(command) == 0 ? 0 : -1;
}
//---------------------------------------------------------------------------------------------//
int
git_push(const char *path)
{
    char command[512];

    snprintf(
        command,
        sizeof(command),
        "git -C \"%s\" push",
        path
    );

    return system(command) == 0 ? 0 : -1;
}


