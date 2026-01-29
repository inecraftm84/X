#include <stdio.h>
#include <time.h>
#include <string.h>

static struct timespec start_ts, end_ts;

extern int get_var(const char *name);

void x_exec(char *line) {
    if (strcmp(line, "start_timer") == 0) {
        clock_gettime(CLOCK_MONOTONIC, &start_ts);
        printf("[Timer] Started.\n");
    } 
    else if (strcmp(line, "stop_timer") == 0) {
        clock_gettime(CLOCK_MONOTONIC, &end_ts);
        double elapsed = (end_ts.tv_sec - start_ts.tv_sec) + 
                         (end_ts.tv_nsec - start_ts.tv_nsec) / 1000000000.0;
        
        printf("[Timer] Elapsed: %.6f seconds\n", elapsed);
       }
}
