#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <omp.h>

#include "../include/quicksort_arqav.h"

#define threadsIteratorMAX 3

int arrayLength;
int numberOfThreads;
int threads[threadsIteratorMAX] = { 1, 2, 4 };
static int threadsIterator = 0;

int init(int argc, char const *argv[]) {
  if(argc < 2) {
    printf("Invalid arguments passed.\n");
    printf("\targv[1] = size of array\n");

    return -3;
  } else if(argc > 3) {
    printf("Invalid arguments passed.\n");
    printf("\targv[1] = size of array\n");
    printf("\targv[2] = number of threads\n");

    return -1;
  }

  if(argc == 2) {
    numberOfThreads = threads[threadsIterator];
  } else {
    numberOfThreads = strtol(argv[2], NULL, 10);
  }

  arrayLength = strtol(argv[1], NULL, 10);

  if(arrayLength <= 0 && numberOfThreads <= 0) {
    printf("Invalid arguments passed.\n");
    printf("\targv[1] = size of array\n");
    printf("\targv[2] = number of threads\n");

    return -2;
  }

  return 0;
}

void printSummary(int argc, double time_arr1, double time_arr2, double time_arr3, double* time_threads) {
  printf("\n#########################################################\n");
  printf("# EXECUTION SUMMARY \t\t\t\t\t#\n");
  printf("# \t\t\t\t\t\t\t#\n");
  printf("# Number of Elements: %d \t\t\t\t#\n", arrayLength);
  if(argc == 2) {
    int i;
    printf("# Number of Threads: ");
    for (i = 0; i < threadsIteratorMAX - 1; i++) {
      printf("%d, ", threads[i]);
    }
    printf("%d. \t\t\t\t#\n", threads[i]);
  } else {
    printf("# Number of Threads: %d \t\t\t\t\t#\n", numberOfThreads);
  }
  printf("# \t\t\t\t\t\t\t#\n");
  printf("# stdlib QuickSort: (aprox.): %f seconds   \t#\n", time_arr1);
  printf("# Sequential QuickSort: (aprox.): %f seconds   \t#\n", time_arr2);
  if(argc == 2) {
    int i;
    printf("# Parallel QuickSort: (aprox.): \t\t\t#\n");

    for (i = 0; i < threadsIteratorMAX; i++) {
      printf("# \t %f seconds (%d threads)\t\t\t#\n", time_threads[i], threads[i]);
    }
  } else {
    printf("# Parallel QuickSort: (aprox.): %f seconds   \t#\n", time_arr3);
  }
  printf("#########################################################\n");
}

void sort_stdlib(int* arr, double* time_arr) {
  double startTime, stopTime;

  printf("\nSorting with serial 'qsort' function of 'stdlib.h' ...");
  fflush(stdout);
  startTime = clock();
  qsort(arr, arrayLength, sizeof(int), compare);
  stopTime = clock();
  (*time_arr) = (double) (stopTime - startTime) / CLOCKS_PER_SEC;
  printf("\nSorted in (aprox.): %f seconds \n", (*time_arr));
}

void sort_sequential(int* arr, int* compareArr, double* time_arr) {
  double startTime, stopTime;

  printf("\nSorting with sequential QuickSort...");
  fflush(stdout);
  startTime = clock();
  quickSort_sequential(arr, 0, arrayLength - 1);
  stopTime = clock();
  (*time_arr) = (double) (stopTime - startTime) / CLOCKS_PER_SEC;
  printf("\nSorted in (aprox.): %f seconds \n", (*time_arr));
  printf("Checking if the results are correct...\n");
  if(isEqualArray(compareArr, arr, arrayLength) == true) {
    printf("\t'sequential QuickSort' is CORRECT\n");
  } else {
    printf("\t'sequential QuickSort' is INCORRECT\n");
  }
}

void sort_parallel(int argc, int* arr, int* originalArr, int* compareArr, double* time_arr, double* time_threads) {
  double startTime, stopTime;

  copyArray(originalArr, arr, arrayLength);
  printf("\nSorting with parallel QuickSort (%d threads)... ", numberOfThreads);
  fflush(stdout);
  startTime = omp_get_wtime();
  quickSort_parallel(arr, arrayLength, numberOfThreads);
  stopTime = omp_get_wtime();

  (*time_arr) = (stopTime - startTime);
  printf("\nSorted in (aprox.): %f seconds \n", (*time_arr));
  printf("Checking if the results are correct...\n");

  if(isEqualArray(compareArr, arr, arrayLength) == true) {
    printf("\t'parallel QuickSort' is CORRECT\n");
  } else {
    printf("\t'parallel QuickSort' is INCORRECT\n");
  }

  if(argc == 2) {
    time_threads[threadsIterator] = (*time_arr);

    if(threadsIterator < threadsIteratorMAX -1) {
      numberOfThreads = threads[++threadsIterator];
      sort_parallel(argc, arr, originalArr, compareArr, time_arr, time_threads);
    }
  }
}

int main(int argc, char const *argv[]) {
  int initCheck = init(argc, argv);
  if(initCheck != 0) {
    return initCheck;
  }

  double time_arr1, time_arr2, time_arr3;
  double time_threads[threadsIteratorMAX];

	int* solvedArr = (int*) malloc(arrayLength * sizeof(int));
	int* unsolvedArr = (int*) malloc(arrayLength * sizeof(int));
	int* workingArr = (int*) malloc(arrayLength * sizeof(int));

  long unsigned int arrayMemorySize = (arrayLength * sizeof(int) / (1024 * 1024));
	printf("\nIndividual array size (aprox): %lu MBytes \n", arrayMemorySize);
	printf("Total memory allocated (aprox): %lu MBytes \n\n", arrayMemorySize * 3);

  initializeArrays(solvedArr, unsolvedArr, workingArr, arrayLength, 1, arrayLength);

  sort_stdlib(solvedArr, &time_arr1);
  sort_sequential(workingArr, solvedArr, &time_arr2);
  sort_parallel(argc, workingArr, unsolvedArr, solvedArr, &time_arr3, time_threads);

	free(solvedArr);
	free(unsolvedArr);
	free(workingArr);

  printSummary(argc, time_arr1, time_arr2, time_arr3, time_threads);

	return 0;
}
