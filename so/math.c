#include <stdio.h>
#include <string.h>

extern int* g_ptr(char *name);

void x_exec(char *line) {
    char s1[32], s2[32], trg[32];
    if (sscanf(line, "%s + %s = %s", s1, s2, trg) == 3) {
        int *p1 = g_ptr(s1);
        int *p2 = g_ptr(s2);
        int *pT = g_ptr(trg);
        
        *pT = (*p1) + (*p2);
    }
}