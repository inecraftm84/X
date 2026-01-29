#include <stdio.h>
#include <unistd.h>
void x_exec(char *line) {
    int ms;
    if (sscanf(line, "sleep %d", &ms) == 1) {
        usleep(ms * 1000); 
    }
}
