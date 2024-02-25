#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

double  sum=0.0;
long iter, nT;

pthread_mutex_t sum_mutex;

double getTime()
{
  const double kMicro = 1.0e-6;
  struct timeval TV;

  const int RC = gettimeofday(&TV, NULL);
  if(RC == -1)
    {
      printf("ERROR: Bad call to gettimeofday\n");
      return(-1);
    }
  return( ((double)TV.tv_sec) + kMicro * ((double)TV.tv_usec) );
}

// Method declarations
void* calculatePartialSum (void* threadNumber);
double sequentialCompute (long iterations);
double parallelCompute(long iterations, int numberOfThreads);
double getDifference(double calculatedPi);
// Main method
int main(int argc, char* argv[])
{
  // Variable declarations
  double sequentialStart, sequentialEnd, sequentialTime;
  double parallelStart, parallelEnd, parallelTime;

  double sequentialPi, parallelPi;
  double sequentialDifference, parallelDifference;
  long iterations; 
  int numberOfThreads;

  // Get number of iterations and number of threads from the command line
  if(argc > 1)
    {
      iterations = strtol(argv[1], NULL, 10);
      numberOfThreads = strtol(argv[2], NULL, 10);
    }
  else
    {
      printf("\nWhen running this program, please include number of iterations and number of threads on command line.\n");
      return 0;
    }

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
  printf("ParallelCalculation: %f\n", parallelPi);
  printf("This is %f away from the correct value of PI.\n\n", parallelDifference);
  printf("Number of iterations: %ld, Number of Threads: %d\n\n", iterations, numberOfThreads);

  // Calculate the validity of the parallel computation
  double difference = parallelDifference - sequentialDifference;

  if (difference < .01 && difference > -.01)
    printf("Parallel calculation is VALID!\n");
  else
    printf("Parallel calculation is INVALID!\n");

  // Calculate and print speedup results
  double speedup = ((double)sequentialTime)/((double)parallelTime);
  double efficiency = speedup/(double)numberOfThreads;
  printf("Sequential Time: %f, Parallel Time: %f, Speedup: %f, Efficiency: %f\n", sequentialTime, parallelTime, speedup, efficiency);
  return 0;
}

// Sequential computation of PI
double sequentialCompute (long iterations)
{
  double factor = 1.0;
  double sum = 0;
  double pi_approximation; 

  long k = 0;
  for (k = 0; k < iterations; k++)
    {
      sum += factor/(2*k+1);
      factor = -factor;
    }

  pi_approximation = 4.0 * sum; 
  return pi_approximation;
}

// Find how close the calculation is to the actual value of PI
double getDifference(double calculatedPi)
{
  return calculatedPi - 3.14159265358979323846;
}

// TODO: You need to implement this function.
// Parallel computation of PI with Pthread
void* calculatePartialSum (void* threadNumber)
{
  long k, end;
  double mysum=0.0, factor;
  k = *((long *)threadNumber) * (iter/nT);
  end = k + (iter/nT);
  if ((k % 2) == 0) {
    factor = 1.0;
  } else {
    factor = -1.0;
  }
  for (; k < end ; k++)
  {
    mysum += factor/(2*k+1);
    factor = -factor;
  }
  /* Lock the mutex and update the global sum, then exit */
  pthread_mutex_lock (&sum_mutex);
  sum = sum + 4.0 * mysum;
  pthread_mutex_unlock (&sum_mutex);
}
double parallelCompute(long iterations, int numberOfThreads)
{
  long i, *tids;
  pthread_t *threads;
  pthread_attr_t attr;
  //Dynamicaly alocate memory for tids, & threads
  tids = malloc(sizeof(long)*numberOfThreads); 
  threads = malloc(sizeof(pthread_t)*numberOfThreads); 
  sum = 0.0;
  // Checking for memory allocation 
  if ((threads == NULL)||(tids == NULL)) { 
      printf("out of memory\n");
      exit(EXIT_FAILURE);
  }
  iter = iterations;
  nT = numberOfThreads;
  /* Pthreads setup: initialize mutex and explicitly create threads in a
     joinable state (for portability).  Pass each thread its loop offset */
  pthread_mutex_init(&sum_mutex, NULL);
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  for (i=0; i<numberOfThreads; i++)
  {
    tids[i] = i;
    pthread_create(&threads[i], &attr, calculatePartialSum, (void *) &tids[i]);
  }
  /* Wait for all threads to complete then print global sum */ 
  for (i=0; i<numberOfThreads; i++) {
    pthread_join(threads[i], NULL);
  }
  /* Clean up and exit */
  pthread_attr_destroy(&attr);
  pthread_mutex_destroy(&sum_mutex);
  return  sum;
}
