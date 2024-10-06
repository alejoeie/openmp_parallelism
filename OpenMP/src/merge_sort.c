#ifdef _OPENMP
    #include <omp.h>
#else 
    #define omp_get_thread_num() 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// Merge function (same as before)
static void merge(int* arr, int left, int mid, int right) {
    int first_half_size = mid - left + 1; 
    int second_half_size = right - mid; 
    int i, j;

    int* L = (int*) malloc(first_half_size * sizeof(int));
    int* R = (int*) malloc(second_half_size * sizeof(int));

    for(i = 0; i < first_half_size; i++) {
        L[i] = arr[left + i];
    }
    for(j = 0; j < second_half_size; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int left_iterable = 0, right_iterable = 0;
    int minimum_element_original_array = left;

    while(left_iterable < first_half_size && right_iterable < second_half_size) {
        if (L[left_iterable] <= R[right_iterable]) {
            arr[minimum_element_original_array] = L[left_iterable++];
        } else {
            arr[minimum_element_original_array] = R[right_iterable++];
        }
        minimum_element_original_array++;
    }

    while(left_iterable < first_half_size) {
        arr[minimum_element_original_array++] = L[left_iterable++];
    }

    while(right_iterable < second_half_size) {
        arr[minimum_element_original_array++] = R[right_iterable++];
    }

    free(L);
    free(R);
}

// Recursive Merge Sort with threshold for parallelization
void merge_sort(int* arr, int left, int right, int threshold) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        if (right - left > threshold) {
            #pragma omp task
            merge_sort(arr, left, mid, threshold);

            #pragma omp task
            merge_sort(arr, mid + 1, right, threshold);

            #pragma omp taskwait
            merge(arr, left, mid, right);
        } else {
            // Perform sequential merge sort for smaller subarrays
            merge_sort(arr, left, mid, threshold);
            merge_sort(arr, mid + 1, right, threshold);
            merge(arr, left, mid, right);
        }
    }
}

void print_arr_elems(int* arr, int length) {
    printf("[");
    for(size_t ii = 0; ii < length; ii++) {
        printf("%d ", arr[ii]);
    }
    printf("]\n");
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

int main(int argc, char* argv[]) {    
    int test_arr[] = {24, 25, 26, 7, 14, 15};
    double i_time, f_time;

    int size_test_arr = sizeof(test_arr) / sizeof(test_arr[0]);

    if (argc > 1) {
        int arr_size_by_user = atoi(argv[2]);
        int* usr_arr = (int*)malloc(arr_size_by_user * sizeof(int));

        for (int i = 0; i < arr_size_by_user; i++) {
            usr_arr[i] = rand() % arr_size_by_user;
        }

        omp_set_num_threads(atoi(argv[1]));  // Set the number of threads

        int threshold = 1000;  // Adjust this threshold to control parallelism
        struct timeval start, end;
        // Parallel region starts in main, not inside recursive calls
        printf("Executing parallel section\n");

        gettimeofday(&start, NULL);  // Start timer
        
        #pragma omp parallel
        {
            #pragma omp single
            merge_sort(usr_arr, 0, arr_size_by_user - 1, threshold);
        }


        gettimeofday(&end, NULL);  // End timer
        
        long seconds = end.tv_sec - start.tv_sec;
        long micros = end.tv_usec - start.tv_usec;
        double total_time = seconds + micros * 1e-6;

        printf("Time taken for parallel merge sort: %f seconds\n", total_time);
        write_statistics_on_file("../parallel_statistics.txt", arr_size_by_user, total_time);
        free(usr_arr);
    } else {
        perror("User must enter number of threads for parallelizing algorithm");
    }

    return 0;
}
