#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <unistd.h>

#define NUM_WRITER 2
#define NUM_READER 5
#define NUM_R_OR_W 3
#define SLEEPTIME  1
#define SEMOP_P   -1
#define SEMOP_V    1

#define SEM_READER 0
#define SEM_WRITER 1
#define SEM_MUTEX  2

#define ERR(...) do {                           \
        fprintf (stderr, __VA_ARGS__);          \
        exit (EXIT_FAILURE);                    \
    } while (0);

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void sem_p (int sem, int num) {
    struct sembuf sembuf = {
        .sem_num = num,
        .sem_op = SEMOP_P,
        .sem_flg = ~(IPC_NOWAIT | SEM_UNDO),
    };
    if (semop (sem, &sembuf, 1) < 0) ERR ("semop P()");
}

void sem_v (int sem, int num) {
    struct sembuf sembuf = {
        .sem_num = num,
        .sem_op = SEMOP_V,
        .sem_flg = ~(IPC_NOWAIT | SEM_UNDO),
    };
    if (semop (sem, &sembuf, 1) < 0) ERR ("semop V()");
}

void sem_set (int sem, int num, int val) {
    union semun arg = { .val = val };
    if (semctl (sem, num, SETVAL, arg) < 0) ERR ("semctl");
}

int sem_get (int sem, int num) {
    return semctl (sem, num, GETVAL, NULL);
}

void sem_init (int sem, int num, int val) {
    sem_set (sem, num, val);
}

int main (int argc, char **argv) {
    time_t init_time;
    size_t i, j;
    pid_t pid;
    key_t key;
    int sem, val;

    (void) argc;

    init_time = time (0);

    if ((key = ftok (*argv, '0')) < 0) ERR ("ftok");
    if ((sem = semget (key, 3, IPC_CREAT | 0666)) < 0) ERR ("semget");
    sem_init (sem, SEM_READER, 0);
    sem_init (sem, SEM_WRITER, 1);
    sem_init (sem, SEM_MUTEX, 1);

    /* create writers */
    for (i = 0; i < NUM_WRITER; i++) {
        pid = fork ();
        if (pid == 0) {
            /* child */
            for (j = 0; j < NUM_R_OR_W; j++) {
                /* write */
                sem_p (sem, SEM_WRITER);
                printf ("[%2zus : w%zu] write start\n", time (0) - init_time, i);
                sleep (SLEEPTIME);
                printf ("[%2zus : w%zu] write done\n", time (0) - init_time, i);
                sem_v (sem, SEM_WRITER);
                /* uncritical */
                printf ("[%2zus : w%zu] uncritical start\n", time (0) - init_time, i);
                sleep (SLEEPTIME);
                printf ("[%2zus : w%zu] uncritical done\n", time (0) - init_time, i);
            }
            exit (EXIT_SUCCESS);
        } else if (pid > 0) {
            /* parent */
        } else ERR ("fork");
    }

    /* create readers */
    for (i = 0; i < NUM_READER; i++) {
        pid = fork ();
        if (pid == 0) {
            /* child */
            for (j = 0; j < NUM_R_OR_W; j++) {
                /* read */
                sem_p (sem, SEM_MUTEX);
                val = sem_get (sem, SEM_READER) + 1;
                sem_set (sem, SEM_READER, val);
                if (val == 1)
                    sem_p (sem, SEM_WRITER);
                sem_v (sem, SEM_MUTEX);
                printf ("[%2zus : r%zu] read start\n", time (0) - init_time, i);
                sleep (SLEEPTIME);
                printf ("[%2zus : r%zu] read done\n", time (0) - init_time, i);
                sem_p (sem, SEM_MUTEX);
                val = sem_get (sem, SEM_READER) - 1;
                sem_set (sem, SEM_READER, val);
                if (val == 0)
                    sem_v (sem, SEM_WRITER);
                sem_v (sem, SEM_MUTEX);
                /* uncritical */
                printf ("[%2zus : r%zu] uncritical start\n", time (0) - init_time, i);
                sleep (SLEEPTIME);
                printf ("[%2zus : r%zu] uncritical done\n", time (0) - init_time, i);
            }
            exit (EXIT_SUCCESS);
        } else if (pid > 0) {
            /* parent */
        } else ERR ("fork");
    }
}
