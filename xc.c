#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

int x_vars[26] = {0};
typedef void (*exec_fn)(char *);

void *handles[100];
exec_fn funcs[100];
int lib_count = 0;

void load_lib(const char *path) {
    if (lib_count >= 100) return;

    char final_path[512];
    if (strchr(path, '/') == NULL) {
        snprintf(final_path, sizeof(final_path), "./%s", path);
    } else {
        strncpy(final_path, path, sizeof(final_path) - 1);
    }

    void *h = dlopen(final_path, RTLD_LAZY | RTLD_GLOBAL);
    if (h) {
        exec_fn f = (exec_fn)dlsym(h, "x_exec");
        if (f) {
            handles[lib_count] = h;
            funcs[lib_count++] = f;
        }
    } else {
        h = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
        if (!h) {
            fprintf(stderr, "Load Error: %s\n", dlerror());
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    char *inf = NULL;

    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], ".so")) {
            load_lib(argv[i]);
        } else {
            inf = argv[i];
        }
    }

    if (!inf) return 1;

    FILE *f = fopen(inf, "r");
    if (!f) return 1;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        if (strncmp(line, "using ", 6) == 0) {
            char *lib_path = line + 6;
            load_lib(lib_path);
            continue;
        }

        for (int i = 0; i < lib_count; i++) {
            funcs[i](line);
        }
    }

    fclose(f);
    for (int i = 0; i < lib_count; i++) dlclose(handles[i]);
    return 0;
}
