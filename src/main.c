#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/history.h>
#include <readline/readline.h>

#include "history.h"
#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "expand.h"
#include "builtin.h"

int main(void)
{
    printf("=====================================\n");
    printf("      Shellforge \n");
    printf(" A Unix Style Shell written in C\n");
    printf("=====================================\n");

    using_history();

    token_list_t tokens;
    pipeline_t pipeline;

    char *line;

    while (1)
    {
        line = readline("shellforge$ ");

        if (line == NULL)
        {
            printf("\nGoodbye!\n");
            break;
        }

        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        if (strcmp(line, "history") == 0)
        {
            print_history();
            free(line);
            continue;
        }

        add_history(line);

        lexer(line, &tokens);

        if (parser(&tokens, &pipeline))
        {
            expand_variables(&pipeline);
        }

        if (pipeline.command_count == 1 &&
            pipeline.commands[0].argc > 0 &&
            is_builtin(&pipeline.commands[0]))
        {
            if (strcmp(pipeline.commands[0].argv[0], "exit") == 0)
            {
                free(line);
                printf("Exiting...\n");
                break;
            }

            execute_builtin(&pipeline.commands[0]);
        }

        free(line);
    }

    return 0;
}
