#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double getTime() {
    const double kMicro = 1.0e-6;
    struct timeval TV;
    const int RC = gettimeofday(&TV, NULL);
    if(RC == -1) {
        printf("ERROR: Bad call to gettimeofday\n");
        return (-1);
    }
    return (((double)TV.tv_sec) + kMicro * ((double)TV.tv_usec));
}

double sequentialCompute(long iterations) {
    double factor = 1.0;
    double sum = 0.0;
    double piApproximation;

    for (long k = 0; k < iterations; k++) {
        sum += factor / (2 * k + 1);
        factor = -factor;
    }

    piApproximation = 4.0 * sum;
    return piApproximation;
}

double getDifference(double calculatedPi) {
    return calculatedPi - 3.14159265358979323846;
}

int main(int argc, char* argv[]) {
    double start, end, computeTime, pi, difference;
    long iterations;

    if (argc != 2) {
        printf("\nUsage: %s <iterations>\n", argv[0]);
        return 1;
    }

    iterations = strtol(argv[1], NULL, 10);

    start = getTime();
    pi = sequentialCompute(iterations);
    end = getTime();

    computeTime = end - start;
    difference = getDifference(pi);
    
    printf("Number of Iterations = %ld\n", iterations);
    printf("Sequential Calculation of PI: %f\n", pi);
    printf("Time taken: %f seconds\n", computeTime);
    printf("Difference from actual PI: %f\n", difference);

    return 0;
}
