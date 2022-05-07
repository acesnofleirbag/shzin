#ifndef SHZIN_MAIN_H
#define SHZIN_MAIN_H

#define SHZIN_READ_LINE_BUFSIZE 1024
#define SHZIN_TOK_BUFSIZE 64
#define SHZIN_TOK_DELIM " \t\r\n\a"

void allocation_error(void*);
char* shzin_read_line();
char** shzin_split_line(char*);
int shzin_launch(char**);
int shzin_execute(char**);

#endif
