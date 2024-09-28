#ifndef _MERGE_SORT_H
#define _MERGE_SORT_H

#define SIZE(totalBytesCount, firstElemBytes) \
        sizeof(totalBytesCount) / sizeof(firstElemBytes)

void print_arr_elems(int* arr, int length);

void merge_sort(int* arr, int left, int right);

void print_arr_elems(int* arr, int length);

#endif