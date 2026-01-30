#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct { char name[32]; int value; } Variable;
extern Variable vars[1000];
extern int var_count;

// 優化：如果是數字直接回傳數值指標，如果是變數則從陣列找
int* get_val_or_var_ptr(const char *name, int *temp_const) {
    if (isdigit(name[0]) || (name[0] == '-' && isdigit(name[1]))) {
        *temp_const = atoi(name);
        return temp_const; // 這裡有風險，稍後在 x_exec 處理
    }
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0) return &vars[i].value;
    }
    strncpy(vars[var_count].name, name, 31);
    vars[var_count].value = 0;
    return &vars[var_count++].value;
}

void x_exec(char *line) {
    static char *last_line = NULL;
    static int *v1_p = NULL, *v2_p = NULL, *tar_p = NULL;
    static int const1, const2; // 用於存放常數值
    static char op_c = 0;

    if (line != last_line) {
        char s1[32], s2[32], target[32], op;
        if (sscanf(line, " %s %c %s = %s", s1, &op, s2, target) == 4) {
            v1_p = get_val_or_var_ptr(s1, &const1);
            v2_p = get_val_or_var_ptr(s2, &const2);
            tar_p = get_val_or_var_ptr(target, &(int){0}); // target 必為變數
            op_c = op;
            last_line = line;
        } else { return; }
    }

    if (v1_p && v2_p && tar_p) {
        switch(op_c) {
            case '+': *tar_p = *v1_p + *v2_p; break;
            case '-': *tar_p = *v1_p - *v2_p; break;
            case '*': *tar_p = *v1_p * *v2_p; break;
            case '/': if (*v2_p != 0) *tar_p = *v1_p / *v2_p; break;
        }
    }
}