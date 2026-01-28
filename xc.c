#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

typedef struct {
    char name[32];
    int value;
} Variable;

Variable vars[1000];
int var_count = 0;

int get_var(const char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) return vars[i].value;
    }
    return 0;
}

void set_var(const char *name, int val) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) {
            vars[i].value = val;
            return;
        }
    }
    if (var_count < 1000) {
        strncpy(vars[var_count].name, name, 31);
        vars[var_count].value = val;
        var_count++;
    }
}

typedef void (*exec_fn)(char *);
void *handles[100];
exec_fn funcs[100];
int lib_count = 0;

void load_lib(const char *path) {
    if (lib_count >= 100) return;
    void *h = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
    if (h) {
        exec_fn f = (exec_fn)dlsym(h, "x_exec");
        if (f) {
            handles[lib_count] = h;
            funcs[lib_count++] = f;
        }
    } else {
        fprintf(stderr, "Load Error: %s\n", dlerror());
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    char *inf = argv[argc - 1];

    for (int i = 1; i < argc - 1; i++) load_lib(argv[i]);

    FILE *f = fopen(inf, "r");
    if (!f) return 1;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0 || line[0] == '#') continue;

        if (strncmp(line, "using ", 6) == 0) {
            load_lib(line + 6);
            continue;
        }

        for (int i = 0; i < lib_count; i++) funcs[i](line);
    }

    fclose(f);
    for (int i = 0; i < lib_count; i++) dlclose(handles[i]);
    return 0;
}