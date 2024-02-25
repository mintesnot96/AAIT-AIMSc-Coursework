#include <stdio.h>
#include <math.h>

#define nMin 10
#define nMax 320
#define pMin 1
#define pMax 128

// Function to calculate the runtime of the serial program (Tserial = n^2)
double calculateSerialRuntime(int n)
{
    return pow(n, 2);
}

// Function to estimate the parallel runtime (Tparallel = n^2 / p + log2(p))
double estimateParallelRuntime(int n, int p)
{
    return calculateSerialRuntime(n) / p + log(p) / log(2);
}

// Function to estimate parallel runtime with an additional overhead term (Tparallel = n^2 / p + Toverhead)
double estimateParallelRuntimeWithOverhead(int n, int p, int Toverhead)
{
    return calculateSerialRuntime(n) / p + Toverhead;
}

// Function to calculate the speedup (Speedup = Tserial / Tparallel)
double calculateSpeedup(int n, int p)
{
    double Tserial = calculateSerialRuntime(n);
    double Tparallel = estimateParallelRuntime(n, p);
    return Tserial / Tparallel;
}

// Function to calculate the parallel efficiency (Efficiency = Tserial / (p * Tparallel))
double calculateParallelEfficiency(int n, int p)
{
    double Tserial = calculateSerialRuntime(n);
    double Tparallel = estimateParallelRuntime(n, p);
    return Tserial / (p * Tparallel);
}

// Function to calculate parallel efficiency with an overhead term (Efficiency = Tserial / (p * TparallelWithOverhead))
double calculateParallelEfficiencyWithOverhead(int n, int p, int Toverhead)
{
    double Tserial = calculateSerialRuntime(n);
    double TparallelWithOverhead = estimateParallelRuntimeWithOverhead(n, p, Toverhead);
    return Tserial / (p * TparallelWithOverhead);
}

// Function to print a horizontal separator in the output
void printHorizontalSeparator(int min, int max)
{
    int i;
    printf("\t\t+");
    for (i = min; i < max; i *= 2)
    {
        printf("----------");
    }
    printf("---+\n");
}

int main()
{
    int n;
    int p;

    // Introduction and explanation
    printf("This program analyzes the speedup and efficiency of a parallel program compared to a serial program.\n");
    printf("The serial program has a runtime of Tserial = n^2.\n");
    printf("The parallel program runtime is estimated as Tparallel = n^2 / p + log2(p).\n");
    printf("It calculates and displays results for different problem sizes (n) and numbers of processes (p).\n\n");

    // Section A: Analyzing efficiency and speedup, the effect of varying p and keeping n fixed, and also p is fixed and n is increased?
    printf("Section A: Analyzing efficiency and speedup, the effect of varying p and keeping n fixed, \n and also p is fixed and n is increased? :\n");
    printf("\n");
    printf("Results show that as p increases with fixed n, efficiency and speedup generally improve,\n");
    printf("except for very small problem sizes (n=10) where the effect is less significant.\n");
    printf("Efficiency and speedup may decrease when using a large number of processes compared to problem size.\n\n");

    // Table Label
    printf("Rows represent the number of processes (p), and columns represent problem size (n)\n\n");

    // Printing the Speedup table
    printf("\t\tSpeedup\n");
    printHorizontalSeparator(nMin, nMax);
    printf("\t\t");
    for (n = nMin; n <= nMax; n *= 2)
    {
        printf("|%8.d", n);
    }
    printf("|\n");
    printHorizontalSeparator(nMin, nMax);

    for (p = pMin; p <= pMax; p *= 2)
    {
        printf("\t%8.d", p);
        for (n = nMin; n <= nMax; n *= 2)
        {
            double speedup = calculateSpeedup(n, p);
            printf("|%8.4f", speedup);
        }
        printf("|\n");
    }

    printHorizontalSeparator(nMin, nMax);
    printf("\n\n");

    // Printing the Efficiency table
    printf("\t\tEfficiency\n");
    printHorizontalSeparator(nMin, nMax);
    printf("\t\t");
    for (n = nMin; n <= nMax; n *= 2)
    {
        printf("|%8.d", n);
    }
    printf("|\n");
    printHorizontalSeparator(nMin, nMax);

    for (p = pMin; p <= pMax; p *= 2)
    {
        printf("\t%8.d", p);
        for (n = nMin; n <= nMax; n *= 2)
        {
            double efficiency = calculateParallelEfficiency(n, p);
            printf("|%8.4f", efficiency);
        }
        printf("|\n");
    }

    printHorizontalSeparator(nMin, nMax);
    printf("\n\n");

    // Section B: Analyzing the effect of an additional overhead term
    printf("Section B: Analyzing the effect of Toverhead on efficiency with fixed p and varying n:\n");
    printf("\n");
    printf("If Toverhead grows more slowly than Tserial, efficiency increases with larger problem sizes.\n");
    printf("If Toverhead grows faster than Tserial, efficiency decreases with larger problem sizes.\n\n");

    // Printing the Efficiency table with Toverhead = kn
    printf("\t\tEfficiency when Toverhead = n\n");
    printHorizontalSeparator(nMin, nMax);
    printf("\t\t");
    for (n = nMin; n <= nMax; n *= 2)
    {
        printf("|%8.d", n);
    }
    printf("|\n");
    printHorizontalSeparator(nMin, nMax);

    for (p = pMin; p <= pMax; p *= 2)
    {
        printf("\t%8.d", p);
        for (n = nMin; n <= nMax; n *= 2)
        {
            double efficiency = calculateParallelEfficiencyWithOverhead(n, p, n);
            printf("|%8.6f", efficiency);
        }
        printf("|\n");
    }

    printHorizontalSeparator(nMin, nMax);
    printf("\n\n");
    // Printing the Efficiency table with Toverhead = n^2
    printf("\t\tEfficiency when Toverhead = n^2\n");
    printHorizontalSeparator(nMin, nMax);
    printf("\t\t");
    for (n = nMin; n <= nMax; n *= 2)
    {
        printf("|%8.d", n);
    }
    printf("|\n");
    printHorizontalSeparator(nMin, nMax);

    for (p = pMin; p <= pMax; p *= 2)
    {
        printf("\t%8.d", p);
        for (n = nMin; n <= nMax; n *= 2)
        {
            double efficiency = calculateParallelEfficiencyWithOverhead(n, p, n * n);
            printf("|%8.6f", efficiency);
        }
        printf("|\n");
    }

    printHorizontalSeparator(nMin, nMax);
    printf("\n\n");


    // Printing the Efficiency table with Toverhead = n^3
    printf("\t\tEfficiency when Toverhead = n^3\n");
    printHorizontalSeparator(nMin, nMax);
    printf("\t\t");
    for (n = nMin; n <= nMax; n *= 2)
    {
        printf("|%8.d", n);
    }
    printf("|\n");
    printHorizontalSeparator(nMin, nMax);

    for (p = pMin; p <= pMax; p *= 2)
    {
        printf("\t%8.d", p);
        for (n = nMin; n <= nMax; n *= 2)
        {
            double efficiency = calculateParallelEfficiencyWithOverhead(n, p, n * n * n);
            printf("|%8.6f", efficiency);
        }
        printf("|\n");
    }

    printHorizontalSeparator(nMin, nMax);
    printf("\n\n");

    return 0;
}
