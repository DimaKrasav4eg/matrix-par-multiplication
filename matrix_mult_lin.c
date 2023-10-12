#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define ROWS_A 30
#define COLS_A 20
#define ROWS_B 20
#define COLS_B 30

int** createMatrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }
    return matrix;
}

// Function for generating a random matrix of size rows x cols
int** generateRandomMatrix(int rows, int cols) {
    int** matrix = createMatrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
    return matrix;
}

// Функция для перемножения двух матриц A и B
int** multiplyMatrices(int rowsA, int colsA, int** A, int rowsB, int colsB, int** B) {
    int** result = createMatrix(rowsA, colsB);
    if (colsA != rowsB) {
        printf("Multiplication is not possible: the number of columns of matrix A does not match the number of rows of matrix B.\n");
        return NULL;
    }
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

int main(int argc, char* argv[]) {
    
    srand(time(NULL));

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
    int** matrixB = generateRandomMatrix(rowsA, colsA);

    double start_time = omp_get_wtime();
    int** result = multiplyMatrices(rowsA, colsA, matrixA, rowsB, colsB, matrixB);
    double end_time = omp_get_wtime();
    printf("===========================\n");
    printf("||   lin time %f   ||\n", end_time-start_time);
    printf("===========================\n");

    return 0;
}
