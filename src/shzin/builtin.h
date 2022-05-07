#ifndef SHZIN_BUILTIN_H
#define SHZIN_BUILTIN_H

#include "status.h"

#include <stdio.h>
#include <unistd.h>

char* builtins[] = {"cd", "help", "exit"};

int
shzin_builtins_length() {
    return sizeof(builtins) / sizeof(char*);
}

int
shzin_cd(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "shzin: expected arg to 'cd' command\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("shzin\n");
        }
    }

    return SHZIN_CONTINUE;
}

int
shzin_help(char** args) {
    printf("Stephan Brennan LSH Tutorial as SHZIN\n"
           "type program name and args, and hit enter\n"
           "the following are built in:\n");

    for (int i = 0; i < shzin_builtins_length(); i++) {
        printf("* %s\n", builtins[i]);
    }

    printf("use the man command for information on other programs\n");

    return SHZIN_CONTINUE;
}

int
shzin_exit(char** args) {
    return SHZIN_STOP;
}

int (*shzin_builtin[])(char**) = {&shzin_cd, &shzin_help, &shzin_exit};

#endif
