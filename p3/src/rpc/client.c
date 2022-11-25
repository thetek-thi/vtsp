#include <stdlib.h>
#include "math.h"

int main (int argc, char **argv) {
    struct intpair data;
    CLIENT *cl;
    int *res;

    if (argc != 4) {
        fprintf (stderr, "usage: ./client <hostname> <num1> <num2>\n");
        exit (EXIT_FAILURE);
    }

    cl = clnt_create (argv[1], MATHPROG, MATHVERS, "tcp");

    data = (struct intpair) {
        .a = atoi (argv[2]),
        .b = atoi (argv[3]),
    };

    res = add_1 (&data, cl);
    printf ("%d + %d = %d\n", data.a, data.b, *res);
    res = mult_1 (&data, cl);
    printf ("%d * %d = %d\n", data.a, data.b, *res);
    res = quad_1 (&(data.a), cl);
    printf ("%d ^ 2 = %d\n", data.a, *res);
}
