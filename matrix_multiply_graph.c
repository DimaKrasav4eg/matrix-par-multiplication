#include "matrix_lib/matrixops.h"
#include <unistd.h>

#define OUTPUT_FILENAME "graph_points.txt"

void write_in_file(FILE* file, double* nums, int n){
    for (int i = 1; i < n; ++i)
    {
        fprintf(file, "%f ", nums[i]);
    }
    fprintf(file, "\n");
}

int main(int argc, char* argv[]) {

	int repeat = 1;

	int rowsA = 0;
    int colsA = 0;
    int rowsB = 0;
    int colsB = 0;

    if (argc == 1){
    	printf("Standard dimension values and the number of repetitions are used\n");
    } else if (argc == 2){
        printf("Standard dimension values are used\n");
        rowsA = ROWS_A;
        colsA = COLS_A;
        rowsB = ROWS_B;
        colsB = COLS_B;
    } else {
        rowsA = atoi(argv[1]);
        colsA = atoi(argv[2]);
        rowsB = atoi(argv[3]);
        colsB = atoi(argv[4]);
        if (argc == 6){
        	repeat = atoi(argv[5]);
        }
    }

    int max_procs = omp_get_num_procs();

    double* par_naive_time = (double*) calloc(max_procs, sizeof(double));
    double* par_shtrassen_time = (double*) calloc(max_procs, sizeof(double));

    for (int i = max_procs; i > 0; --i)
    {
        omp_set_num_threads(i);
        par_naive_time[i] = test_time_naive(repeat, rowsA, colsB, rowsB, colsB);
        par_shtrassen_time[i] = test_time_strassen(repeat, rowsA, colsB, rowsB, colsB);
    }

    FILE *file;
    file = fopen(OUTPUT_FILENAME, "w");
    if (file == NULL) {
        printf("file error\n");
        return 1;
    }
    write_in_file(file, par_naive_time, max_procs);
    write_in_file(file, par_shtrassen_time, max_procs);

}