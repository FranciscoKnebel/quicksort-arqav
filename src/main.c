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

bool isEqualArray(int* arr1, int *arr2, int size) {
  bool isResultCorrect = true;
  int i = 0;

  while((isResultCorrect == true) && (i < size))
  {
    if(arr1[i] != arr2[i]) {
      isResultCorrect = false;
    }
    i++;
  }

  return isResultCorrect;
}

void initializeArrays(int* arr1, int* arr2, int* arr3, int minimumNumber, int maximumNumber) {
  int i;

  srand(time(NULL));
  printf("Initializing the arrays...\n");
  for (i = 0; i < arrayLength; i++) {
    arr1[i] = minimumNumber + (rand() % maximumNumber);
    arr2[i] = arr1[i];
    arr3[i] = arr1[i];
  }
  printf("Complete.\n");
}

void copyArray(int* src, int* dst, int size) {
  int i;

  for (i = 0; i < size; i++) {
    dst[i] = src[i];
  }
}

int main(int argc, char const *argv[]){
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

	double startTime, stopTime;
  double time_arr1, time_arr2, time_arr3;
  double time_threads[threadsIteratorMAX];

	int* solvedArr = (int*) malloc(arrayLength * sizeof(int));
	int* unsolvedArr = (int*) malloc(arrayLength * sizeof(int));

	int* arr = (int*) malloc(arrayLength * sizeof(int));

  long unsigned int arrayMemorySize = (arrayLength * sizeof(int) / (1024 * 1024));
	printf("\nIndividual array size (aprox): %lu MBytes \n", arrayMemorySize);
	printf("Total memory allocated (aprox): %lu MBytes \n\n", arrayMemorySize * 3);

  initializeArrays(solvedArr, unsolvedArr, arr, 1, arrayLength);

	printf("\nSorting with serial 'qsort' function of 'stdlib.h' ..."); fflush(stdout);
	startTime = clock();
	qsort(solvedArr, arrayLength, sizeof(int), compare);
	stopTime = clock();
  time_arr1 = (double)(stopTime-startTime)/CLOCKS_PER_SEC;
	printf("\nSorted in (aprox.): %f seconds \n", time_arr1);

	printf("\nSorting with sequential QuickSort..."); fflush(stdout);
	startTime = clock();
	quickSort_sequential(arr, 0, arrayLength-1);
	stopTime = clock();
  time_arr2 = (double)(stopTime-startTime)/CLOCKS_PER_SEC;
	printf("\nSorted in (aprox.): %f seconds \n", time_arr2);
  printf("Checking if the results are correct...\n");
  if(isEqualArray(solvedArr, arr, arrayLength) == true) {
    printf("\t'sequential QuickSort' is CORRECT\n");
  } else {
    printf("\t'sequential QuickSort' is INCORRECT\n");
  }

  parallel:
  copyArray(unsolvedArr, arr, arrayLength);

	printf("\nSorting with parallel QuickSort (%d threads)... ", numberOfThreads);
  fflush(stdout);
	startTime = omp_get_wtime();
	quickSort_parallel(arr, arrayLength, numberOfThreads);
	stopTime = omp_get_wtime();
  time_arr3 = (stopTime - startTime);
	printf("\nSorted in (aprox.): %f seconds \n", time_arr3);
  printf("Checking if the results are correct...\n");
  if(isEqualArray(solvedArr, arr, arrayLength) == true) {
    printf("\t'parallel QuickSort' is CORRECT\n");
  } else {
    printf("\t'parallel QuickSort' is INCORRECT\n");
  }

  if(argc == 2) {
    time_threads[threadsIterator] = time_arr3;

    if(threadsIterator < threadsIteratorMAX -1) {
      numberOfThreads = threads[++threadsIterator];
      goto parallel;
    }
  }

	free(solvedArr);
	free(unsolvedArr);
	free(arr);

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
  printf("# STDLib QuickSort: (aprox.): %f seconds   \t#\n", time_arr1);
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

	return 0;
}
