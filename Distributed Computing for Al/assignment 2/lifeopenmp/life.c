#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <omp.h>

#define MATCH(s) (!strcmp(argv[ac], (s)))

int MeshPlot(int t, int m, int n, char **mesh);

double real_rand();
int seed_rand(long sd);

static char **currWorld = NULL, **nextWorld = NULL, **tmesh = NULL;
static int maxiter = 200; /* number of iteration timesteps */
static int population[2] = {0, 0}; /* number of live cells */

int nx = 100; /* number of mesh points in the x dimension */
int ny = 100; /* number of mesh points in the y dimension */

static int w_update = 0;
static int w_plot = 1;

double getTime();
extern FILE *gnu;

//serial implementation if input thread is 1 otherwise it will go to openMP  implementation
void serialWorkMapUpdate(int disable_display) {
    for (int t = 0; t < maxiter && population[w_plot]; t++) {
        // Perform cell updates serially
        for (int i = 1; i < nx - 1; i++) {
            for (int j = 1; j < ny - 1; j++) {
                int nn = currWorld[i+1][j] + currWorld[i-1][j] +
                         currWorld[i][j+1] + currWorld[i][j-1] +
                         currWorld[i+1][j+1] + currWorld[i-1][j-1] +
                         currWorld[i-1][j+1] + currWorld[i+1][j-1];
                nextWorld[i][j] = currWorld[i][j] ? (nn == 2 || nn == 3) : (nn == 3);
            }
        }
        // Swap pointers
        tmesh = nextWorld;
        nextWorld = currWorld;
        currWorld = tmesh;

        // Plot if enabled
        if (!disable_display) {
            MeshPlot(t, nx, ny, currWorld);
        }
    }
}

int main(int argc, char **argv)
{
    int i, j, ac;

    /* Set default input parameters */

    float prob = 0.5; /* Probability of placing a cell */
    long seedVal = 0;
    int game = 0;
    int s_step = 0;
    int numthreads = 1;
    int disable_display = 0;

    /* Over-ride with command-line input parameters (if any) */

    for (ac = 1; ac < argc; ac++)
    {
        if (MATCH("-n"))
        {
            nx = atoi(argv[++ac]);
        }
        else if (MATCH("-i"))
        {
            maxiter = atoi(argv[++ac]);
        }
        else if (MATCH("-t"))
        {
            numthreads = atof(argv[++ac]);
        }
        else if (MATCH("-p"))
        {
            prob = atof(argv[++ac]);
        }
        else if (MATCH("-s"))
        {
            seedVal = atof(argv[++ac]);
        }
        else if (MATCH("-step"))
        {
            s_step = 1;
        }
        else if (MATCH("-d"))
        {
            disable_display = 1;
        }
        else if (MATCH("-g"))
        {
            game = atoi(argv[++ac]);
        }
        else
        {
            printf("Usage: %s [-n < meshpoints>] [-i <iterations>] [-s seed] [-p prob] [-t numthreads] [-step] [-g <game #>] [-d]\n", argv[0]);
            return (-1);
        }
    }

    int rs = seed_rand(seedVal);
    /* Increment sizes to account for boundary ghost cells */

    nx = nx + 2;
    ny = nx;

    /* Allocate contiguous memory for two 2D arrays of size nx*ny.
     * Two arrays are required because in-place updates are not
     * possible with the simple iterative scheme listed below */

    currWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
    for (i = 0; i < nx; i++)
        currWorld[i] = (char *)(currWorld + nx) + i * ny;

    nextWorld = (char **)malloc(sizeof(char *) * nx + sizeof(char) * nx * ny);
    for (i = 0; i < nx; i++)
        nextWorld[i] = (char *)(nextWorld + nx) + i * ny;

    /* Set the boundary ghost cells to hold 'zero' */
    for (i = 0; i < nx; i++)
    {
        currWorld[i][0] = 0;
        currWorld[i][ny - 1] = 0;
        nextWorld[i][0] = 0;
        nextWorld[i][ny - 1] = 0;
    }
    for (i = 0; i < ny; i++)
    {
        currWorld[0][i] = 0;
        currWorld[nx - 1][i] = 0;
        nextWorld[0][i] = 0;
        nextWorld[nx - 1][i] = 0;
    }

    // Generate a world

    if (game == 0)
    { // Use Random input
        for (i = 1; i < nx - 1; i++)
            for (j = 1; j < ny - 1; j++)
            {
                currWorld[i][j] = (real_rand() < prob);
                population[w_plot] += currWorld[i][j];
            }
    }
    else if (game == 1)
    { //  Block, still life
        printf("2x2 Block, still life\n");
        int nx2 = nx / 2;
        int ny2 = ny / 2;
        currWorld[nx2 + 1][ny2 + 1] = currWorld[nx2][ny2 + 1] = currWorld[nx2 + 1][ny2] = currWorld[nx2][ny2] = 1;
        population[w_plot] = 4;
    }
    else if (game == 2)
    { //  Glider (spaceship)
        printf("Glider (spaceship)\n");
        currWorld[2][1] = currWorld[3][2] = currWorld[1][3] = currWorld[2][3] = currWorld[3][3] = 1;
        population[w_plot] = 5;
    }
    else
    {
        printf("Unknown game %d\n", game);
        exit(-1);
    }

    printf("probability: %f\n", prob);
    printf("Random # generator seed: %d\n", rs);
    int t = 0;
    double t0 = getTime();
    if (numthreads == 1) {
        // Run the simulation serially
        serialWorkMapUpdate(disable_display);
    } else {
    /* Initialize OpenMP */
    omp_set_num_threads(numthreads);

        /* Plot the initial data */
        if (!disable_display)
            MeshPlot(0, nx, ny, currWorld);
//    printf("Master thread (%d) handling plotting at iteration %d\n", omp_get_thread_num(), t);


        /* Perform updates for maxiter iterations */


        for (t = 0; t < maxiter && population[w_plot]; t++) {
//        printf("Iteration %d\n", t);

            /* Parallel computation */
            // 1. Data Parallelization
            // : Parallelize the computation of the next state of the world
            // The loop iterations are distributed across multiple threads
            // Using OpenMP, the work of updating each cell is shared among the threads
            // The reduction clause is used to update population count in a thread-safe manner
            // Private clause ensures each thread has its own instance of the loop variable 'j'
#pragma omp parallel for reduction(+ : population[w_update]) private(j)
//#pragma omp parallel for collapse(2) reduction(+ : population[w_update]) , I tried with this option but did not work
            for (i = 1; i < nx - 1; i++) {
                for (j = 1; j < ny - 1; j++) {
                    int nn = currWorld[i + 1][j] + currWorld[i - 1][j] +
                             currWorld[i][j + 1] + currWorld[i][j - 1] +
                             currWorld[i + 1][j + 1] + currWorld[i - 1][j - 1] +
                             currWorld[i - 1][j + 1] + currWorld[i + 1][j - 1];

                    nextWorld[i][j] = currWorld[i][j] ? (nn == 2 || nn == 3) : (nn == 3);
                    population[w_update] += nextWorld[i][j];
                }
//#pragma omp critical
//            {
//                printf("Thread %d working on row %d\n", omp_get_thread_num(), i);
//            }
            }

            // Synchronize threads before proceeding to the next part
            // Ensures that all threads have completed their computation
            // This barrier is crucial to prevent any thread from moving ahead prematurely
            // Ensures consistency and correctness of data before proceeding to the next iteration
#pragma omp barrier


            /* Pointer Swap : nextWorld <-> currWorld */
            // Pointer Swap: Swap the current and next world
            // This operation should be done after all threads have updated their portion of the world
            // It is crucial to ensure that all computation is complete before swapping
            // This prevents any data inconsistency during the update process
            tmesh = nextWorld;
            nextWorld = currWorld;
            currWorld = tmesh;

            /* Master thread plotting */
            // Task Parallelization: Assign the master thread to handle the plotting task
            // Ensures that only the master thread executes the plotting function
            // This is an example of task parallelization where different tasks are done in parallel
            // The master thread does not participate in the computation but focuses on plotting
            // This approach can improve efficiency when combining compute-intensive and I/O tasks

#pragma omp master
            {
                if (!disable_display)
                    MeshPlot(t, nx, ny, currWorld);
//                printf("Master thread (%d) handling plotting at iteration %d\n", omp_get_thread_num(), t);

            }

            if (s_step) {
                printf("Finished with step %d\n", t);
                printf("Press enter to continue.\n");
                getchar();
            }
        }
    }
    double t1 = getTime();
    printf("Running time for the iterations: %f sec.\n", t1 - t0);
    printf("Press enter to end.\n");
    getchar();

    if (gnu != NULL)
        pclose(gnu);

    /* Free resources */
    free(nextWorld);
    free(currWorld);

    return (0);
}
