#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#include "support.h"

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

void initializeArrays(int* arr1, int* arr2, int* arr3, int size, int minimumNumber, int maximumNumber) {
  int i;

  srand(time(NULL));
  printf("Initializing the arrays...\n");
  #pragma omp parallel for
  for (i = 0; i < size; i++) {
    arr1[i] = minimumNumber + (rand() % maximumNumber);
    arr2[i] = arr1[i];
    arr3[i] = arr1[i];
  }

  printf("Complete.\n");
}

void copyArray(int* src, int* dst, int size) {
  int i;

  #pragma omp parallel for
  for (i = 0; i < size; i++) {
    dst[i] = src[i];
  }
}
