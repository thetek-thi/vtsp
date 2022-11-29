/**
 * If it is not possible to P multiple semaphors at once, such a deadlock might
 * occur. However, the atomic P operations for multiple semaphors can be
 * manually implemented by using another semaphor that wraps around the P
 * operation for the forks like this:
 *
 *   P(wrapper)
 *     P(left_fork)
 *     P(right_fork)
 *   V(wrapper)
 *   ...
 *   V(left_fork)
 *   V(right_fork)
 *
 * Problem: the forks are not being used efficiently.
 *
 * Alternative solution: "waiter" that hands out the forks if they are available.
 *
 * Another alternative solution:
 *
 * take_fork (philnum) {
 *   P (mutex)
 *   state[philnum] = HUNGRY
 *   if (state[left] != EATING && state[right] != EATING) {
 *     state[philnum] = EATING
 *     // philosopher is eating
 *   }
 *   V (mutex)
 *   P (philnum)
 * }
 *
 * put_fork (philnum) {
 *   P (mutex)
 *   state[philnum] = THINKING
 *   // philosopher is thinking
 *   if (state[left] == HUNGRY)
 *     V (left)   // "wake up" other philosopher
 *   if (state[right] == HUNGRY)
 *     V (right)   // "wake up" other philosopher
 *   V (mutex)
 * }
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#define NUM_PHIL   5
#define CYCLES  1000
#define TIMERANGE 10
#define SEMOP_P   -1
#define SEMOP_V    1

#define ERR(...) do {                           \
        fprintf (stderr, __VA_ARGS__);          \
        exit (EXIT_FAILURE);                    \
    } while (0);
#define WRAPPER NUM_PHIL

typedef struct {
    uint32_t eat_time;
    uint32_t think_time;
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

void sem_p (int sem, int n) {
    struct sembuf sembuf = {
        .sem_num = n,
        .sem_op = SEMOP_P,
        .sem_flg = ~(IPC_NOWAIT | SEM_UNDO),
    };
    if (semop (sem, &sembuf, 1) < 0) ERR ("semop P()");
}

void sem_v (int sem, int n) {
    struct sembuf sembuf = {
        .sem_num = n,
        .sem_op = SEMOP_V,
        .sem_flg = ~(IPC_NOWAIT | SEM_UNDO),
    };
    if (semop (sem, &sembuf, 1) < 0) ERR ("semop V()");
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
    if ((sem = semget (key, NUM_PHIL + 1, IPC_CREAT | 0666)) < 0) ERR ("semget");

    for (i = 0; i <= NUM_PHIL; i++)
        sem_init (sem, i);

    for (i = 0; i < NUM_PHIL; i++) {
        phils[i] = (phil_t) {
            .eat_time   = 0,//rand () % TIMERANGE + 1,
            .think_time = 0,//rand () % TIMERANGE + 1,
        };
    }

    for (i = 0; i < NUM_PHIL; i++) {
        pid = fork ();
        if (pid == 0) {
            /* child */
            for (j = 0; j < CYCLES; j++) {
                sem_p (sem, WRAPPER);
                printf ("%3zus: p%zu wants to eat\n", time (0) - init_time, i);
                sem_p (sem, i);
                sem_p (sem, (i + 1) % NUM_PHIL);
                sem_v (sem, WRAPPER);
                printf ("%3zus: p%zu eat\n", time (0) - init_time, i);
                sleep (phils[i].eat_time);
                sem_v (sem, i);
                sem_v (sem, (i + 1) % NUM_PHIL);
                printf ("%3zus: p%zu think\n", time (0) - init_time, i);
                sleep (phils[i].think_time);
                printf ("%3zus: p%zu wait\n", time (0) - init_time, i);
                printf("%zu\n", j);
            }
            exit (EXIT_SUCCESS);
        } else if (pid > 0) {
            /* parent */
        } else ERR ("fork");
    }

    return EXIT_SUCCESS;
}
