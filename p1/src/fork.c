#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMPROC   3
#define SLEEPTIME 1

int main (void) {
    size_t i, j;
    pid_t pid;

    for (i = 1; i <= NUMPROC; ++i) {
        pid = fork ();
        if (pid == 0) {
            for (j = 0; j < 3; ++j) {
                printf ("proc %zu: critical enter\n", i);
                sleep (SLEEPTIME);
                printf ("proc %zu: critical leave\n", i);
                printf ("proc %zu: non-critical enter\n", i);
                sleep (SLEEPTIME);
                printf ("proc %zu: non-critical leave\n", i);
            }
            exit (0);
        } else if (pid > 0) {
            /* parent */
        } else {
            /* error */
            perror ("fork");
        }
    }
}
