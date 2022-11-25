struct intpair {
    int a;
    int b;
};

program MATHPROG {
    version MATHVERS {
        int ADD(intpair) = 1;
        int MULT(intpair) = 2;
        int QUAD(int) = 3;
    } = 1;
} = 0x2001000;
