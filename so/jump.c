#include <stdio.h>
#include <string.h>
#include <ctype.h>

<<<<<<< HEAD
extern int* g_ptr(char *name);
extern void set_pc(int new_pc);

void x_exec(char *line) {
    char var[32], dest[32];
    int val;

    if (sscanf(line, "set %s %d", var, &val) == 2) {
        *g_ptr(var) = val;
    }

    else if (sscanf(line, "if %s < %d GOTO %s", var, &val, dest) == 3) {
        if (*g_ptr(var) < val) {
            set_pc(*g_ptr(dest));
=======
extern int get_var(const char *name);
extern void set_var(const char *name, int val);

void x_exec(char *line) {
    char label[32], var[32], dest[32];
    int limit;

    if (sscanf(line, "if %s < %d GOTO %s", var, &limit, dest) == 3) {
        if (get_var(var) < limit) {
            set_var("pc", get_var(dest));
>>>>>>> parent of c8a6105 (web)
        }
    }
}