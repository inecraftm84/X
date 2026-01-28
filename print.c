#include <stdio.h>
#include <string.h>

extern int x_vars[26];

void x_exec(char *line) {
    if (strncmp(line, "print ", 6) == 0) {
        char *arg = line + 6;
        
        if (strlen(arg) == 1 && arg[0] >= 'a' && arg[0] <= 'z') {
            printf("%d\n", x_vars[arg[0] - 'a']);
        } else {
            printf("%s\n", arg);
        }
    }
}