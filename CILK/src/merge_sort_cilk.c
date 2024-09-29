#ifdef _OPENMP
    #include <omp.h>
#else 
    #define omp_get_thread_num() 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <merge_sort_cilk.h>

static void merge(int* arr, int left, int mid, int right) {
    int first_half_size = mid - left + 1; // Holds size of first half
    int second_half_size = right - mid; // Holds size of second half
    int i, j;

    // Left and right arrays copied from original array
    int L[first_half_size], R[second_half_size];

    // #pragma omp parallel for shared(arr) private(i, j)
    // Fill in each array a copy of the respective values of arr
    for(i = 0; i < first_half_size; i++) {
        L[i] = arr[left + i];
    }
    for(j = 0; j < second_half_size; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int left_iterable = 0;
    int right_iterable = 0;
    int minimum_element_original_array = left;


    while(left_iterable < first_half_size && right_iterable < second_half_size){
        if (L[left_iterable] <= R[right_iterable]) {
            arr[minimum_element_original_array] = L[left_iterable];
            left_iterable++;
        } else {
            arr[minimum_element_original_array] = R[right_iterable];
            right_iterable++;
        }
        minimum_element_original_array++;
    }

    while(left_iterable < first_half_size) {
        arr[minimum_element_original_array] = L[left_iterable];
        left_iterable++;
        minimum_element_original_array++;
    }

    while(right_iterable < second_half_size) {
        arr[minimum_element_original_array] = R[right_iterable];
        right_iterable++;
        minimum_element_original_array++;
    }
}

void mergeSort(int* arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        cilk_spawn mergeSort(arr, left, mid); 
        mergeSort(arr, mid+1, right);

        cilk_sync;
        merge(arr, left, mid, right);
    }
}
void printArr(int* arr, int length) {
    printf("[");
    for(size_t ii = 0; ii < length; ii++) {
        printf(" %d,", arr[ii]);
    }
    printf("]\n");
}
int main() {
    int test_arr[] = {56, 7, 87, 13, 59, 90};

    int size_test_arr = sizeof(test_arr) / sizeof(test_arr[0]);

    mergeSort(test_arr, 0, SIZE(test_arr, test_arr[0])-1);

    printArr(test_arr, SIZE(test_arr, test_arr[0]));
    return 0;
}