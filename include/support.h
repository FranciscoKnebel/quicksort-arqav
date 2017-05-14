#ifndef __support__
#define __support__

  typedef enum { false, true } bool;

  bool isEqualArray(int* arr1, int *arr2, int size);
  void initializeArrays(int* arr1, int* arr2, int* arr3, int size, int minimumNumber, int maximumNumber);
  void copyArray(int* src, int* dst, int size);
  
#endif
