#include <stdio.h>
#include <string.h>

extern int* g_ptr(char *name);

void x_exec(char *line) {
    char var[32];
    if (sscanf(line, "print %s", var) == 1) {
        printf("%d\n", *g_ptr(var));
    }
}