#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

int x_vars[26] = {0};

typedef void (*exec_fn)(char *);

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    void *handles[10];
    exec_fn funcs[10];
    int lib_count = 0;
    char *inf = NULL;

    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], ".so")) {
            void *h = dlopen(argv[i], RTLD_LAZY | RTLD_GLOBAL);
            if (h) {
                exec_fn f = (exec_fn)dlsym(h, "x_exec");
                if (f) {
                    handles[lib_count] = h;
                    funcs[lib_count++] = f;
                }
            } else {
                fprintf(stderr, "Error: %s\n", dlerror());
            }
        } else {
            inf = argv[i];
        }
    }

    if (!inf || lib_count == 0) return 1;

    FILE *f = fopen(inf, "r");
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;
        for (int i = 0; i < lib_count; i++) funcs[i](line);
    }

    fclose(f);
    for (int i = 0; i < lib_count; i++) dlclose(handles[i]);
    return 0;
}