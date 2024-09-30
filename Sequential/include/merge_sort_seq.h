#ifndef MERGE_SORT_SEQ_
#define MERGE_SORT_SEQ_

#define SIZE(totalBytesCount, firstElemBytes) \
        sizeof(totalBytesCount) / sizeof(firstElemBytes)

void mergeSort(int* arr, int left, int right);

#endif