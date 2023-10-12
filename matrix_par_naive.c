#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define ROWS_A 30
#define COLS_A 20
#define ROWS_B 20
#define COLS_B 30

#define A_FILENAME "A.txt"
#define B_FILENAME "B.txt"
#define RES_FILENAME "result.txt"

int** createMatrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }
    return matrix;
}

// Function for generating a random matrix of size rows x cols
int** generateRandomMatrix(int rows, int cols) {
    int** matrix = createMatrix(rows, cols);
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        unsigned int seed = omp_get_thread_num() + i;
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand_r(&seed) % 100;
        }
    }
    return matrix;
}

// Function for multiplying two matrices A and B
int** multiplyMatrices(int rowsA, int colsA, int** A, int rowsB, int colsB, int** B) {
    int** result = createMatrix(rowsA, colsB);
    if (colsA != rowsB) {
        printf("Multiplication is not possible: the number of columns of matrix A does not match the number of rows of matrix B.\n");
        return NULL;
    }
    #pragma omp parallel for
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            result[i][j] = 0;
            for (int k = 0; k < colsA; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}
void write_matrix_in_file(char* file_name, int rows, int cols, int** matrix){
    FILE *file;
    file = fopen(file_name, "w");
    if (file == NULL) {
        printf("file error\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

int main(int argc, char* argv[]) {

    int rowsA = 0;
    int colsA = 0;
    int rowsB = 0;
    int colsB = 0;

    if (argc != 5){
        printf("4 arguments were expected. Standard dimension values are used\n");
        rowsA = ROWS_A;
        colsA = COLS_A;
        rowsB = ROWS_B;
        colsB = COLS_B;
    } else {
        rowsA = atoi(argv[1]);
        colsA = atoi(argv[2]);
        rowsB = atoi(argv[3]);
        colsB = atoi(argv[4]);
    }

    int** matrixA = generateRandomMatrix(rowsA, colsA);
    int** matrixB = generateRandomMatrix(rowsB, colsB);

    double start_time = omp_get_wtime();
    int** result = multiplyMatrices(rowsA, colsA, matrixA, rowsB, colsB, matrixB);
    double end_time = omp_get_wtime();

    printf("===========================\n");
    printf("||   par time %f   ||\n", end_time-start_time);
    printf("===========================\n");
    
   
    write_matrix_in_file(A_FILENAME, rowsA, colsA, matrixA);
    write_matrix_in_file(A_FILENAME, rowsB, colsB, matrixB);
    write_matrix_in_file(RES_FILENAME, rowsA, colsB, result);

    return 0;
}
