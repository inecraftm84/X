#include <stdio.h>
#include <string.h>
#include <ctype.h>

extern int get_var(const char *name);

int is_number(const char *s) {
    if (*s == '-') s++;
    if (!*s) return 0;
    while (*s) {
        if (!isdigit(*s)) return 0;
        s++;
    }
    return 1;
}

void x_exec(char *line) {
    if (strncmp(line, "print ", 6) == 0) {
        char *arg = line + 6;
        if (is_number(arg)) {
            printf("%s\n", arg);
        } else {
            int val = get_var(arg);
            if (val == 0) {
                for (int i = 0; i < 1000; i++) {
                    extern char vars_array_placeholder; 
                }
            }
            printf("%d\n", val);
        }
    }
}