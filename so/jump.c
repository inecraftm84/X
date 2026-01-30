#include <stdio.h>
#include <string.h>
#include <ctype.h>

extern int* g_ptr(char *name);
extern void set_pc(int new_pc);

void x_exec(char *line) {
    char var[32], dest[32];
    int val;

    if (sscanf(line, "set %s %d", var, &val) == 2) {
        *g_ptr(var) = val;
    }

    else if (sscanf(line, "if %s < %d GOTO %s", var, &val, dest) == 3) {
        if (*g_ptr(var) < val) {
            set_pc(*g_ptr(dest));
        }
    }
}