#include "shzin/main.h"

#include "shzin/builtin.h"
#include "shzin/status.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
main() {
    char* line;
    char** args;
    int status;

    do {
        printf("shzin: ");

        line = shzin_read_line();
        args = shzin_split_line(line);
        status = shzin_execute(args);

        free(line);
        free(args);
    } while (status);

    return EXIT_SUCCESS;
}

void
allocation_error(void* buffer) {
    if (!buffer) {
        fprintf(stderr, "buffer allocation error");
        exit(EXIT_FAILURE);
    }
}

char*
shzin_read_line() {
    int bufsize = SHZIN_READ_LINE_BUFSIZE, position = 0, input;
    char* buffer = malloc(sizeof(char) * bufsize);

    allocation_error(buffer);

    // NOTE: getline is a modern way to do the same thing
    while (true) {
        input = getchar();

        if (input == EOF || input == '\n') {
            buffer[position++] = '\0';
            return buffer;
        } else {
            buffer[position++] = input;
        }

        if (position >= bufsize) {
            bufsize += SHZIN_READ_LINE_BUFSIZE;
            buffer = realloc(buffer, bufsize);

            allocation_error(buffer);
        }
    }
}

char**
shzin_split_line(char* line) {
    int bufsize = SHZIN_TOK_BUFSIZE, position = 0;
    char** tokens = malloc(sizeof(char*) * bufsize);
    char* token;

    allocation_error(tokens);

    token = strtok(line, SHZIN_TOK_DELIM);

    while (token != NULL) {
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize += SHZIN_TOK_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * bufsize);

            allocation_error(tokens);
        }

        token = strtok(NULL, SHZIN_TOK_DELIM);
    }

    tokens[position] = NULL;

    return tokens;
}

// NOTE: to not builtins commands
int
shzin_launch(char** args) {
    pid_t ppid, pid;
    int status;

    switch (fork()) {
        case -1:
            perror("shzin\n");
            break;
        case 0:
            if (execvp(args[0], args) == -1) {
                perror("shzin\n");
            }
            exit(EXIT_FAILURE);
        default:
            do {
                ppid = waitpid(pid, &status, WUNTRACED);
                // NOTE: 'waitpid.2' macros
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return SHZIN_CONTINUE;
}

int
shzin_execute(char** args) {
    if (args[0] == NULL) {
        return SHZIN_CONTINUE;
    }

    for (int i = 0; i < shzin_builtins_length(); i++) {
        if (strcmp(args[0], builtins[i]) == 0) {
            return (*shzin_builtin[i])(args);
        }
    }

    return shzin_launch(args);
}
