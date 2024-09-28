#ifdef _OPENMP
    #include <omp.h>
#else 
    #define omp_get_thread_num() 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <merge_sort.h>

void merge(int* arr, int left, int mid, int right) {
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

void merge_sort(int* arr, int left, int right) {

    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp task shared(arr)
        printf("Executing merge_sort function - filling left half of array, thread %d executing\n", omp_get_thread_num());
        merge_sort(arr, left, mid); // first half of the array

        #pragma omp task shared(arr)
        printf("Executing merge_sort function - filling right half of array, thread %d executing\n", omp_get_thread_num());
        merge_sort(arr, mid+1, right); // second half of the array

        #pragma omp taskwait
        merge(arr, left, mid, right);
    }
}

void print_arr_elems(int* arr, int length){
    printf("[");
    for(size_t ii = 0; ii < length; ii++) {
        printf(" %d,", arr[ii]);
    }
    printf("]\n");

}

int main() {
    // Array should be 14, 15, 24, 25, 26
    omp_set_num_threads(8);
    int test_arr[] = {24, 25, 26, 7, 14, 15};
    double i_time, f_time;

    int size_test_arr = sizeof(test_arr) / sizeof(test_arr[0]);
    print_arr_elems(test_arr, SIZE(test_arr, test_arr[0]));
    #pragma omp parallel 
    {
        i_time = omp_get_wtime();
        #pragma omp single
        printf("Executing parallel section, thread %d executing\n", omp_get_thread_num());
        merge_sort(test_arr, 0, size_test_arr-1);

        f_time = omp_get_wtime();
        printf("Stopping parallel section, thread %d executing, time took: %fs\n", omp_get_thread_num(), f_time-i_time);
    }
    print_arr_elems(test_arr, SIZE(test_arr, test_arr[0]));

    return 0;
}