#include "matrix_lib/matrixops.h"

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

    omp_set_num_threads(1);
    double lin_naive_time = test_time_naive(repeat, rowsA, colsB, rowsB, colsB);
    omp_set_num_threads(omp_get_num_procs());
    double par_naive_time = test_time_naive(repeat, rowsA, colsB, rowsB, colsB);

    omp_set_num_threads(1);
    double lin_shtrassen_time = test_time_strassen(repeat, rowsA, colsB, rowsB, colsB);
    omp_set_num_threads(omp_get_num_procs());
    double par_shtrassen_time = test_time_strassen(repeat, rowsA, colsB, rowsB, colsB);

    printf("=================================\n");
    printf("||   naive-lin time %f   ||\n", lin_naive_time);
    printf("||   naive-par time %f   ||\n", par_naive_time);
    printf("||   shtrn-lin time %f   ||\n", lin_shtrassen_time);
    printf("||   shtrn-par time %f   ||\n", par_shtrassen_time);
    printf("=================================\n");

}