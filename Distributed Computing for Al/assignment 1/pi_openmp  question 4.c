#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

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

// Method declarations
double sequentialCompute(long iterations);
double parallelCompute(long iterations, int numberOfThreads);
double getDifference(double calculatedPi);

// Main method
int main(int argc, char* argv[]) {
    // Variable declarations
    double sequentialStart, sequentialEnd, sequentialTime;
    double parallelStart, parallelEnd, parallelTime;

    double sequentialPi, parallelPi;
    double sequentialDifference, parallelDifference;
    long iterations;
    int numberOfThreads;

    // Get number of iterations and number of threads from the command line
    if (argc != 3) {
        printf("\nUsage: %s <iterations> <number of threads>\n", argv[0]);
        return 1;
    }

    iterations = strtol(argv[1], NULL, 10);
    numberOfThreads = strtol(argv[2], NULL, 10);

    // Time sequential calculation
    sequentialStart = getTime();
    sequentialPi = sequentialCompute(iterations);
    sequentialEnd = getTime();
    sequentialTime = sequentialEnd - sequentialStart;

    // Time parallel calculation
    parallelStart = getTime();
    parallelPi = parallelCompute(iterations, numberOfThreads);
    parallelEnd = getTime();
    parallelTime = parallelEnd - parallelStart;

    // How do results compare with PI?
    sequentialDifference = getDifference(sequentialPi);
    parallelDifference = getDifference(parallelPi);

    // Print results
    printf("Sequential Calculation: %f\n", sequentialPi);
    printf("This is %f away from the correct value of PI.\n\n", sequentialDifference);
    printf("Parallel Calculation: %f\n", parallelPi);
    printf("This is %f away from the correct value of PI.\n\n", parallelDifference);
    printf("Number of iterations: %ld, Number of Threads: %d\n\n", iterations, numberOfThreads);

    // Calculate the validity of the parallel computation
    double difference = parallelDifference - sequentialDifference;

    if (difference < .01 && difference > -.01)
        printf("Parallel calculation is VALID!\n");
    else
        printf("Parallel calculation is INVALID!\n");

    // Calculate and print speedup and efficiency results
    double speedup = sequentialTime / parallelTime;
    double efficiency = speedup / numberOfThreads;
    printf("Sequential Time: %f\n", sequentialTime);
    printf("Parallel Time: %f\n", parallelTime);
    printf("Speedup:  %f\n", speedup);
    printf("Efficiency: %f\n", efficiency);
    // printf("Sequential Time: %f, Parallel Time: %f, Speedup: %f, Efficiency: %f\n", sequentialTime, parallelTime, speedup, efficiency);

    return 0;
}

// Sequential computation of PI
double sequentialCompute(long iterations) {
    double factor = 1.0;
    double sum = 0.0;
    double piApproximation;

    for (long k = 0; k < iterations; k++) {
        sum += factor / (2*k + 1);
        factor = -factor;
    }

    piApproximation = 4.0 * sum;
    return piApproximation;
}

// Find how close the calculation is to the actual value of PI
double getDifference(double calculatedPi) {
    return calculatedPi - 3.14159265358979323846;
}

// Parallel computation of PI using OpenMP
double parallelCompute(long iterations, int numberOfThreads) {
    double sum = 0.0;
    double factor;

    omp_set_num_threads(numberOfThreads);

    #pragma omp parallel for private(factor) reduction(+:sum)
    for (long k = 0; k < iterations; k++) {
        factor = (k % 2 == 0) ? 1.0 : -1.0; // Compute the sign
        sum += factor / (2*k + 1);
    }

    return 4.0 * sum;
}
