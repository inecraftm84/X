#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

extern int get_var(const char *name);
extern void set_var(const char *name, int val);

int resolve(char *s) {
    if (s[0] == '*') {
        int addr = get_var(s + 1);
        char target_name[32];
        sprintf(target_name, "%d", addr);
        return get_var(target_name);
    }
    if (isdigit(s[0]) || (s[0] == '-' && isdigit(s[1]))) return atoi(s);
    return get_var(s);
}

void x_exec(char *line) {
    char s1[32], s2[32], op, target[32];
    if (sscanf(line, "%s %c %s = %s", s1, &op, s2, target) == 4) {
        int v1 = resolve(s1);
        int v2 = resolve(s2);
        int res = 0;

        if (op == '+') res = v1 + v2;
        else if (op == '-') res = v1 - v2;
        else if (op == '*') res = v1 * v2;
        else if (op == '/') res = (v2 != 0) ? v1 / v2 : 0;

        set_var(target, res);
        printf("[Math] %s(%d) %c %s(%d) -> %s = %d\n", s1, v1, op, s2, v2, target, res);
    }
}