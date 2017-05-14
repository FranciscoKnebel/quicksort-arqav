#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include <omp.h>

#include "../include/quicksort_arqav.h"

int arrayLength;
int numberOfThreads;

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

int main(int argc, char const *argv[]){
  if(argc < 3) {
    printf("Invalid arguments passed.\n");
    printf("\targv[1] = size of array\n");
    printf("\targv[2] = number of threads\n");

    return -1;
  }

  arrayLength = strtol(argv[1], NULL, 10);
  numberOfThreads = strtol(argv[2], NULL, 10);

	double startTime, stopTime;

	int* arr1 = (int*) malloc(arrayLength * sizeof(int));
	int* arr2 = (int*) malloc(arrayLength * sizeof(int));
	int* arr3 = (int*) malloc(arrayLength * sizeof(int));

  long unsigned int arrayMemorySize = (arrayLength * sizeof(int) / (1024 * 1024));
	printf("\nIndividual array size (aprox): %lu MBytes \n", arrayMemorySize);
	printf("Total memory allocated (aprox): %lu MBytes \n\n", arrayMemorySize * 3);

  initializeArrays(arr1, arr2, arr3, 1, arrayLength);

	printf("\nSorting with serial 'qsort' function of 'stdlib.h' ..."); fflush(stdout);
	startTime = clock();
	qsort(arr1, arrayLength, sizeof(int), compare);
	stopTime = clock();
	printf("\nSorted in (aprox.): %f seconds \n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);

	printf("\nSorting with sequential QuickSort..."); fflush(stdout);
	startTime = clock();
	quickSort_sequential(arr2, 0, arrayLength-1);
	stopTime = clock();
	printf("\nSorted in (aprox.): %f seconds \n", (double)(stopTime-startTime)/CLOCKS_PER_SEC);

	printf("\nSorting with parallel QuickSort... "); fflush(stdout);
	startTime = omp_get_wtime();
	quickSort_parallel(arr3, arrayLength, numberOfThreads);
	stopTime = omp_get_wtime();
	printf("\nSorted in (aprox.): %f seconds \n", (stopTime-startTime));

	printf("\nChecking if the results are correct...\n");
  if(isEqualArray(arr1, arr2, arrayLength) == true) {
    printf("\t'sequential QuickSort' is CORRECT\n");
  } else {
    printf("\t'sequential QuickSort' is INCORRECT\n");
  }

  if(isEqualArray(arr1, arr3, arrayLength) == true) {
    printf("\t'parallel QuickSort' is CORRECT\n");
  } else {
    printf("\t'parallel QuickSort' is INCORRECT\n");
  }

	free(arr1);
	free(arr2);
	free(arr3);

	return 0;
}
