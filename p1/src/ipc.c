#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define NUMPROC   3
#define SLEEPTIME 1

#define SEMOP_P -1
#define SEMOP_V  1

#define ERR(func) do {                          \
        perror(func);                           \
        exit(EXIT_FAILURE);                     \
    } while (0)

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void sem_init (int sem) {
    union semun arg = { .val = 1 };
    if (semctl (sem, 0, SETVAL, arg) < 0) ERR ("semctl");
}

void sem_p (int sem) {
    struct sembuf sembuf = {
        .sem_num = 0,
        .sem_op = SEMOP_P,
        .sem_flg = ~(IPC_NOWAIT | SEM_UNDO),
    };
    if (semop (sem, &sembuf, 1) < 0) ERR ("semop P()");
}

void sem_v (int sem) {
    struct sembuf sembuf = {
        .sem_num = 0,
        .sem_op = SEMOP_V,
        .sem_flg = ~(IPC_NOWAIT | SEM_UNDO),
    };
    if (semop (sem, &sembuf, 1) < 0) ERR ("semop V()");
}

int main (int argc, char **argv) {
    size_t i, j;
    pid_t pid;
    key_t key;
    int sem;

    (void) argc;

    if ((key = ftok (*argv, '0')) < 0) ERR ("ftok");
    if ((sem = semget(key, 1, IPC_CREAT | 0666)) < 0) ERR ("semget");
    sem_init (sem);

    for (i = 1; i <= NUMPROC; ++i) {
        pid = fork ();
        if (pid == 0) {
            for (j = 0; j < 3; ++j) {
                sem_p (sem);
                printf ("proc %zu: critical enter\n", i);
                sleep (SLEEPTIME);
                printf ("proc %zu: critical leave\n", i);
                sem_v (sem);
                printf ("proc %zu: non-critical enter\n", i);
                sleep (SLEEPTIME);
                printf ("proc %zu: non-critical leave\n", i);
            }
            exit (0);
        } else if (pid > 0) {
            /* parent */
        } else ERR ("fork");
    }

    return EXIT_SUCCESS;
}
