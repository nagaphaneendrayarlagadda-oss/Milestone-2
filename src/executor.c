#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "executor.h"
#include "builtin.h"

int execute_command(command_t *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
    {
        return 1;
    }

    if (is_builtin(cmd))
    {
        return execute_builtin(cmd);
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return 1;
    }

    if (pid == 0)
    {
        execvp(cmd->argv[0], cmd->argv);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    int status;

    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return 1;
    }

    if (WIFEXITED(status))
    {
        return WEXITSTATUS(status);
    }

    return 1;
}
