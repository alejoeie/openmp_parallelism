#ifndef _MERGE_SORT_CILK
#define _MERGE_SORT_CILK

#define SIZE(totalBytesCount, firstElemBytes) \
        sizeof(totalBytesCount) / sizeof(firstElemBytes)

void mergeSort(int* arr, int left, int right);

#endif