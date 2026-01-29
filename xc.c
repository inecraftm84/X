#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <ctype.h>

#define EXPORT __attribute__((visibility("default")))

typedef struct { char name[32]; int value; } Var;
Var vars[1000];
int v_cnt = 0;

typedef struct {
    int *v1, *v2, *trg;
    int op;
    int *jmp_ptr;
} Inst;

Inst code[5000];
int pc = 0, t_ln = 0;

EXPORT int* g_ptr(char *n) {
    if (isdigit(n[0]) || (n[0] == '-' && isdigit(n[1]))) {
        int i = v_cnt++;
        sprintf(vars[i].name, "_c%d", i);
        vars[i].value = atoi(n);
        return &vars[i].value;
    }
    for (int i = 0; i < v_cnt; i++) if (!strcmp(vars[i].name, n)) return &vars[i].value;
    strcpy(vars[v_cnt].name, n);
    vars[v_cnt].value = 0;
    return &vars[v_cnt++].value;
}

EXPORT int get_var(const char *n) { return *g_ptr((char*)n); }

typedef void (*ex_f)(char *);
ex_f fns[10];
int l_cnt = 0;

int main(int ac, char **av) {
    if (ac < 2) return 1;
    FILE *f = fopen(av[ac-1], "r");
    char ln[256], *prg[5000];
    int raw_cnt = 0;

    while (fgets(ln, 256, f)) {
        ln[strcspn(ln, "\r\n")] = 0;
        char *p = ln; while (*p == ' ') p++;
        if (!*p || *p == '#') continue;
        if (!strncmp(p, "using ", 6)) {
            void *h = dlopen(p + 6, RTLD_LAZY | RTLD_GLOBAL);
            if (h) fns[l_cnt++] = (ex_f)dlsym(h, "x_exec");
            continue;
        }
        if (*p == ':') { *g_ptr(p + 1) = raw_cnt; continue; }
        prg[raw_cnt++] = strdup(p);
    }

    for (int i = 0; i < raw_cnt; i++) {
        char s1[32], s2[32], trg[32], op;
        if (sscanf(prg[i], " %s %c %s = %s", s1, &op, s2, trg) == 4) {
            code[t_ln] = (Inst){g_ptr(s1), g_ptr(s2), g_ptr(trg), op, NULL};
        } else if (sscanf(prg[i], " if %s < %s GOTO %s", s1, s2, trg) == 3) {
            code[t_ln] = (Inst){g_ptr(s1), g_ptr(s2), NULL, 'i', g_ptr(trg)};
        } else if (sscanf(prg[i], " print %s", s1) == 1) {
            code[t_ln] = (Inst){g_ptr(s1), NULL, NULL, 'p', NULL};
        } else {
            code[t_ln] = (Inst){(int*)prg[i], NULL, NULL, 'x', NULL};
        }
        t_ln++;
    }

    while (pc < t_ln) {
        Inst *n = &code[pc];
        switch(n->op) {
            case '+': *n->trg = *n->v1 + *n->v2; pc++; break;
            case '-': *n->trg = *n->v1 - *n->v2; pc++; break;
            case '*': *n->trg = *n->v1 * *n->v2; pc++; break;
            case 'i': pc = (*n->v1 < *n->v2) ? *n->jmp_ptr : pc + 1; break;
            case 'p': printf("%d\n", *n->v1); pc++; break;
            case 'x': for (int j = 0; j < l_cnt; j++) fns[j]((char*)n->v1); pc++; break;
            default: pc++;
        }
    }
    return 0;
}