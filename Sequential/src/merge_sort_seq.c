#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "merge_sort_seq.h"

static void merge(int* arr, int left, int mid, int right) {
    int first_half_size = mid - left + 1; // Holds size of first half
    int second_half_size = right - mid; // Holds size of second half
    int i, j;

    // Left and right arrays copied from original array
    int* L = (int*) malloc(first_half_size * sizeof(int));
    int* R = (int*) malloc(second_half_size * sizeof(int));

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

    free(L);
    free(R);
}

void merge_sort(int* arr, int left, int right) {

    if (left < right) {
        int mid = left + (right - left) / 2;

                merge_sort(arr, left, mid); // first half of the array

                merge_sort(arr, mid+1, right); // second half of the array

                merge(arr, left, mid, right);

    }
}

void write_statistics_on_file(char* file_path, int arr_size, double runtime) {
    FILE* sequential_data;

    sequential_data = fopen(file_path, "a");
    if (sequential_data == NULL) {
        perror("Failed to open file");
    }

    fprintf(sequential_data, "%f %d\n", runtime, arr_size);

    fclose(sequential_data);
}

void print_arr_elems(int* arr, int length){
    printf("[");
    for(size_t ii = 0; ii < length; ii++) {
        printf("%d ", arr[ii]);
    }
    printf("]\n");

}

int main(int argc, char* argv[]) {    
    int test_arr[] = {24, 25, 26, 7, 14, 15};

    int size_test_arr = sizeof(test_arr) / sizeof(test_arr[0]);
    if (argc > 1) {
        int arr_size_by_user = atoi(argv[1]);
        int* usr_arr = (int*)malloc(arr_size_by_user * sizeof(int));

        for (int i = 0; i < arr_size_by_user; i++) {
            usr_arr[i] = rand() % arr_size_by_user;
        }

        struct timeval start, end;
        gettimeofday(&start, NULL);  // Start timer

        // print_arr_elems(usr_arr, arr_size_by_user);
        merge_sort(usr_arr, 0, arr_size_by_user-1);
        // print_arr_elems(usr_arr, arr_size_by_user);
        gettimeofday(&end, NULL);  // End timer

        long seconds = end.tv_sec - start.tv_sec;
        long micros = end.tv_usec - start.tv_usec;
        double total_time = seconds + micros * 1e-6;

        printf("Time taken for sequential merge sort: %f seconds\n", total_time);
        write_statistics_on_file("../sequential_statistics.txt", arr_size_by_user, total_time);
        free(usr_arr);
    } else {
        perror("User must enter number of threads for parallelizing algorithm");
    }

    


    return 0;
}