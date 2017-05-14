/*
  Quicksort parallel
*/

#ifndef __parallel__
#define __parallel__

  void quickSort_parallel(int* array, int lenArray, int numThreads);
  void quickSort_parallel_internal(int* array, int left, int right, int cutoff);

#endif
