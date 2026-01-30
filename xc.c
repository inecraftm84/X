#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#define EXPORT __attribute__((visibility("default")))

typedef struct { char name[32]; int value; } Var;
Var vars[1000];
int v_cnt = 0;

typedef struct {
    char *raw_cmd;
    int *jmp_target;
} Inst;

Inst code[5000];
int pc = 0, t_ln = 0;

typedef void (*ex_f)(char *);
ex_f active_fns[10];
int l_cnt = 0;

EXPORT int* g_ptr(char *n) {
    if (isdigit(n[0]) || (n[0] == '-' && isdigit(n[1]))) {
        for (int i = 0; i < v_cnt; i++) {
            if (!strcmp(vars[i].name, n)) return &vars[i].value;
        }
        int i = v_cnt++;
        strncpy(vars[i].name, n, 31);
        vars[i].value = atoi(n);
        return &vars[i].value;
    }
    
    for (int i = 0; i < v_cnt; i++) {
        if (!strcmp(vars[i].name, n)) return &vars[i].value;
    }
    
    strncpy(vars[v_cnt].name, n, 31);
    vars[v_cnt].value = 0;
    return &vars[v_cnt++].value;
}

EXPORT int get_var(const char *n) { return *g_ptr((char*)n); }
EXPORT void set_pc(int new_pc) { pc = new_pc; }

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
            if (h) {
                ex_f func = (ex_f)dlsym(h, "x_exec");
                if (func) active_fns[l_cnt++] = func;
            }
            continue;
        }
        if (*p == ':') { 
            *g_ptr(p + 1) = raw_cnt; 
            continue; 
        }
        prg[raw_cnt++] = strdup(p);
    }
    fclose(f);

    for (int i = 0; i < raw_cnt; i++) {
        code[t_ln++] = (Inst){prg[i], NULL};
    }

    while (pc < t_ln) {
        char *current_cmd = code[pc].raw_cmd;
        int old_pc = pc;

        for (int j = 0; j < l_cnt; j++) {
            active_fns[j](current_cmd);
        }

        if (pc == old_pc) pc++;
    }

    for (int i = 0; i < raw_cnt; i++) free(prg[i]);
    return 0;
}