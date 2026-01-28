#include <stdio.h>
#include <string.h>
extern int x_vars[26];
void x_exec(char *line) {
    int n1, n2; char op, v;
    if (sscanf(line, "%d %c %d = %c", &n1, &op, &n2, &v) == 4) {
        if (op == '+') x_vars[v - 'a'] = n1 + n2;
        if (op == '-') x_vars[v - 'a'] = n1 - n2;
    }
}