/**
 * Philosopher Problem - Sketch:
 *
 * Every fork is a semaphor. For taking the fork and putting it back, the
 * standard P and V operations are used. Here, the fact the the IPC semaphors
 * allow for atomic P of multiple semaphors at once comes in handy. If this
 * action would not be atomic, a deadlock might occur (e.g. when each
 * philosopher takes one fork, but cannot take the second one since it is in
 * posession of the next philosopher).
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#define NUM_PHIL   5
#define CYCLES     3
#define TIMERANGE 10
#define SEMOP_P   -1
#define SEMOP_V    1

#define ERR(...) do {                           \
        fprintf (stderr, __VA_ARGS__);          \
        exit (EXIT_FAILURE);                    \
    } while (0);
#define GETFORKS(i) (i % NUM_PHIL), ((i + 1) % NUM_PHIL)

typedef enum {
    PHILSTATE_EAT,
    PHILSTATE_THINK,
    PHILSTATE_WAIT,
} philstate_t;

typedef struct {
    uint32_t eat_time;
    uint32_t think_time;
    philstate_t state;
} phil_t;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void sem_init (int sem, int num) {
    union semun arg = { .val = 1 };
    if (semctl (sem, num, SETVAL, arg) < 0) ERR ("semctl");
}

void sem_p2 (int sem, int n1, int n2) {
    struct sembuf sembuf[2] = {
        {
            .sem_num = n1,
            .sem_op = SEMOP_P,
            .sem_flg = ~(IPC_NOWAIT | SEM_UNDO),
        }, {
            .sem_num = n2,
            .sem_op = SEMOP_P,
            .sem_flg = ~(IPC_NOWAIT | SEM_UNDO),
        }
    };
    if (semop (sem, sembuf, 2) < 0) ERR ("semop P()");
}

void sem_v2 (int sem, int n1, int n2) {
    struct sembuf sembuf[2] = {
        {
            .sem_num = n1,
            .sem_op = SEMOP_V,
            .sem_flg = ~(IPC_NOWAIT | SEM_UNDO),
        }, {
            .sem_num = n2,
            .sem_op = SEMOP_V,
            .sem_flg = ~(IPC_NOWAIT | SEM_UNDO),
        }
    };
    if (semop (sem, sembuf, 2) < 0) ERR ("semop V()");
}

int main (int argc, char **argv) {
    phil_t phils[NUM_PHIL];
    time_t init_time;
    size_t i, j;
    pid_t pid;
    key_t key;
    int sem;

    (void) argc;

    init_time = time (0);
    srand (init_time);   // initialize random number generator

    if ((key = ftok (*argv, '0')) < 0) ERR ("ftok");
    if ((sem = semget (key, NUM_PHIL, IPC_CREAT | 0666)) < 0) ERR ("semget");

    for (i = 0; i < NUM_PHIL; i++)
        sem_init (sem, i);

    for (i = 0; i < NUM_PHIL; i++) {
        phils[i] = (phil_t) {
            .eat_time   = rand () % TIMERANGE + 1,
            .think_time = rand () % TIMERANGE + 1,
            .state      = PHILSTATE_WAIT,
        };
    }

    for (i = 0; i < NUM_PHIL; i++) {
        pid = fork ();
        if (pid == 0) {
            /* child */
            for (j = 0; j < CYCLES; j++) {
                sem_p2 (sem, GETFORKS (i));
                phils[i].state = PHILSTATE_EAT;
                printf ("%3zus: p%zu eat\n", time (0) - init_time, i);
                sleep (phils[i].eat_time);
                sem_v2 (sem, GETFORKS (i));
                phils[i].state = PHILSTATE_THINK;
                printf ("%3zus: p%zu think\n", time (0) - init_time, i);
                sleep (phils[i].think_time);
                phils[i].state = PHILSTATE_WAIT;
                printf ("%3zus: p%zu wait\n", time (0) - init_time, i);
            }
            exit (EXIT_SUCCESS);
        } else if (pid > 0) {
            /* parent */
        } else ERR ("fork");
    }

    return EXIT_SUCCESS;
}
