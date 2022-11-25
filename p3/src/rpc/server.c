#include "math.h"

int *add_1_svc (intpair *pair, struct svc_req *rqstp) {
    static int res;
    (void) rqstp;
    res = pair->a + pair->b;
    return &res;
}

int *mult_1_svc (intpair *pair, struct svc_req *rqstp) {
    static int res;
    (void) rqstp;
    res = pair->a * pair->b;
    return &res;
}

int *quad_1_svc (int *x, struct svc_req *rqstp) {
    static int res;
    (void) rqstp;
    res = (*x) * (*x);
    return &res;
}
