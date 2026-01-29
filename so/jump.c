#include <stdio.h>
#include <string.h>

extern int get_var(const char *name);
extern void set_var(const char *name, int val);

void x_exec(char *line) {
    char label[32], var[32], dest[32];
    int limit;

    if (sscanf(line, "if %s < %d GOTO %s", var, &limit, dest) == 3) {
        if (get_var(var) < limit) {
            set_var("pc", get_var(dest));
        }
    }
}