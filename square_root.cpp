/**
 *  @file   square_root.cpp
 *  @brief  Try to reproduce a 1 / (square root) hack.
 *  @author Paolo Di Giglio
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/** @brief Structure with binary representation of a number. */
struct bin_rep {
    /** @brief The actual binary digits. */
    bool* digit;
    /** @brief The length of `digit` array. */
    int   length;
};

/**
 * @brief Evaluates the binary representation of a `long int`.
 * @param i The number to convert in binary representation.
 * @return A `struct bin_rep` with the binary representation of the input.
 */
const struct bin_rep binary_representation(long int i) {
    struct bin_rep representation;

    representation.length = 8 * sizeof(long int);
    representation.digit  = (bool *) malloc(representation.length);

    if (!representation.digit)
        exit(EXIT_FAILURE);

    // Convert i into binary
    int j = 0;
    while (i != 0 ) {
        representation.digit[j] = (bool) (i % 2);
        ++j;
        i >>= 1;
    }

    return representation;
}

/**
 * @brief A very evil bit-level hack to evaluate \f$\/\sqrt(x)\f$.
 * @param x The argument of the square root.
 * @return \f$1/\sqrt(x)\f$.
 * @todo Understand this shit!
 */
float hack_sqrt(float x) {
    float x2 = x * .5f;
    float y  = x;

    long int i = *((long int *) &y);
    i = 0x5f3759df - (i >> 1);
    y = *((float *) &i);

    return y * (1.5f - (x2 * y * y));
}

int main() {
    /*
    printf("0x5f3759df = %ld (_10)\n", 0x5f3759df);
    long int i = 0x5f3759df;

    struct bin_rep binary = binary_representation(i);
    for (int j = binary.length ; j != 0; --j)
        printf("%d", (int) binary.digit[j]);

    printf("\n");

    printf("sizeof(long int) = %d\n", sizeof(long int));

    printf("--------------------------------\n");
    free(binary.digit);
    */

    clock_t t_real, t_hack;
    for (float f = 1.f; f < 10000; f += 1.f) {
        // real 1/sqrt
        t_real = clock();
        float real_inv_sqrt = 1 / sqrt(f);
        t_real = clock() - t_real;

        t_hack = clock();
        float hack_inv_sqrt = hack_sqrt(f);
        t_hack = clock() - t_hack;

        fprintf(stdout, "%f %f %f %f\n", f, real_inv_sqrt, hack_inv_sqrt, real_inv_sqrt - hack_inv_sqrt);
        fprintf(stderr, "%d %d\n", t_real, t_hack);
    }


    return 0;
}
