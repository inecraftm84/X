#include <stdio.h>
#include <string.h>

extern int get_var(const char *name);
extern void set_pc(int new_pc);

void x_exec(char *line) {
    char dest[32];
    if (sscanf(line, "GOTO %s", dest) == 1) {
        set_pc(get_var(dest));
    }
}