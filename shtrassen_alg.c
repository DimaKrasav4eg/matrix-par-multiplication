#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

#define ROWS_A 8
#define COLS_A 8
#define ROWS_B 8
#define COLS_B 8

#define MIN_STRASSEN_DIM 65

int** createMatrix(int rows, int cols);
void free_matrix(int rows, int** A);
int** generateRandomMatrix(int rows, int cols, int des);
int** multiplyMatrices(int rowsA, int colsA, int** A, int rowsB, int colsB, int** B);
void write_matrix_in_file(char* file_name, int rows, int cols, int** matrix);
int** add_matrix(int dim, int** A, int** B);
int** subtract_matrix(int dim, int** A, int** B);
int** copy_fragment(int n, int start_row, int start_col, int** A);
int** merge_matrixes(int n, int** C11, int** C12, int** C21, int** C22);
void print_matrix(int rows, int cols, int** A);
int** fill_zeros_to_square(int rowsA, int colsA, int** A, int n);
int** strassen(int rowsA, int colsA, int** A, int rowsB, int colsB, int** B);
int max(int a, int b);
int min(int a, int b);

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

    int** A = generateRandomMatrix(rowsA, colsA, 10);
    int** B = generateRandomMatrix(rowsB, colsB, 10);
    int** C;

    // print_matrix(rowsA, colsA, A);
    // print_matrix(rowsB, colsB, B);

    double start_time = omp_get_wtime();
    C = strassen(rowsA, colsA, A, rowsB, colsB, B);
    double end_time = omp_get_wtime();
    // print_matrix(rowsA, colsB, C);

    printf("===========================\n");
    printf("||   shr time %f   ||\n", end_time-start_time);
    printf("===========================\n");

    return 0;
}

int** createMatrix(int rows, int cols) {
    int** matrix = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
    }
    return matrix;
}

void free_matrix(int rows, int** A){
    for (int i = 0; i < rows; i++){
        free(A[i]);
    }
    free(A);
}

// Function for generating a random matrix of size rows x cols
int** generateRandomMatrix(int rows, int cols, int des) {
    int** matrix = createMatrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() % des;
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

int** add_matrix(int dim, int** A, int** B) {

    int** C = createMatrix(dim, dim);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}
int** subtract_matrix(int dim, int** A, int** B) {
    int** C = createMatrix(dim, dim);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}
int** copy_fragment(int n, int start_row, int start_col, int** A){
    int** new_A = createMatrix(n, n);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            new_A[i][j] = A[start_row + i][start_col + j];
        }
    }
    return new_A;
}

int** multiply_submatrices(int dim, int** A, int** B) {
    int** result = createMatrix(dim, dim);

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            result[i][j] = 0;
            for (int k = 0; k < dim; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return result;
}
int** merge_matrixes(int n, int** C11, int** C12, int** C21, int** C22){
    int new_dim = 2*n; 
    int** result = createMatrix(new_dim, new_dim);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++)
        {
            result[i][j]     = C11[i][j];
            result[i][n+j]   = C12[i][j];
            result[n+i][j]   = C21[i][j];
            result[n+i][n+j] = C22[i][j];
        }
    }
    return result;
}

void print_matrix(int rows, int cols, int** A){
    printf("================\n");
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    printf("================\n");
}


int** strassen_square(int dim, int** A, int** B){

    if (dim < MIN_STRASSEN_DIM){
        return multiply_submatrices(dim, A, B);
    }
    int n = dim / 2;

    int** A11 = copy_fragment(n, 0, 0, A);
    int** A12 = copy_fragment(n, 0, n, A);
    int** A21 = copy_fragment(n, n, 0, A);
    int** A22 = copy_fragment(n, n, n, A);

    // print_matrix(n, n, A12);
    int** B11 = copy_fragment(n, 0, 0, B);
    int** B12 = copy_fragment(n, 0, n, B);
    int** B21 = copy_fragment(n, n, 0, B);
    int** B22 = copy_fragment(n, n, n, B);


    int** M1 = strassen_square(n, add_matrix(n, A11, A22), add_matrix(n, B11, B22));
    int** M2 = strassen_square(n, add_matrix(n, A21, A22), B11);
    int** M3 = strassen_square(n, A11, subtract_matrix(n, B12, B22));
    int** M4 = strassen_square(n, A22, subtract_matrix(n, B21, B11));
    int** M5 = strassen_square(n, add_matrix(n, A11, A12), B22);
    int** M6 = strassen_square(n, subtract_matrix(n, A21, A11), add_matrix(n, B11, B12));
    int** M7 = strassen_square(n, subtract_matrix(n, A12, A22), add_matrix(n, B21, B22));

    free_matrix(n, A11);
    free_matrix(n, A12);
    free_matrix(n, A21);
    free_matrix(n, A22);

    free_matrix(n, B11);
    free_matrix(n, B12);
    free_matrix(n, B21);
    free_matrix(n, B22);

    int** C11 = subtract_matrix(n, add_matrix(n, M1, M4), subtract_matrix(n, M5, M7));
    int** C12 = add_matrix(n, M3, M5);
    int** C21 = add_matrix(n, M2, M4);
    int** C22 = add_matrix(n, subtract_matrix(n, M1, M2), add_matrix(n, M3, M6));

    free_matrix(n, M1);
    free_matrix(n, M2);
    free_matrix(n, M3);
    free_matrix(n, M4);
    free_matrix(n, M5);
    free_matrix(n, M6);
    free_matrix(n, M7);

    return merge_matrixes(n, C11, C12, C21, C22);
}

int** fill_zeros_to_square(int rowsA, int colsA, int** A, int n){
    int** new_A = createMatrix(n, n);
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < colsA; j++){
            new_A[i][j] = A[i][j];
        }
    }
    for (int i = rowsA; i < n; i++)
    {
        for (int j = colsA; j < n; j++){
            new_A[i][j] = 0;
        }
    }
    return new_A;
}
int max(int a, int b){
    if (a > b){
        return a;
    }
    return b;
}
int min(int a, int b){
    if (a < b){
        return a;
    }
    return b;
}
int find_nearest_degree_of_two(int a){
    int n = (int)ceil(log(a) / log(2));
    int near = (int)pow(2, n);
    return near;
}
int** strassen(int rowsA, int colsA, int** A, int rowsB, int colsB, int** B){
    // if (rowsA == colsA && colsA == rowsB && rowsB == colsB && rowsA % 2 == 0){
    //     return strassen_square_submatrix(rowsA, A, B);
    // }
    int n = find_nearest_degree_of_two( max(max(rowsA, colsA), max(rowsB, colsB)) );
    
    int** new_A = fill_zeros_to_square(rowsA, colsA, A, n);
    int** new_B = fill_zeros_to_square(rowsB, colsB, B, n);
    // printf("new A and B\n");
    // print_matrix(n, n, new_A);
    // print_matrix(n, n, new_B);
    // printf("new A and B\n");
    int** C = strassen_square(n, new_A, new_B);
    int** new_C = createMatrix(rowsA, colsB);

    for (int i = 0; i < rowsA; i++){
        for(int j = 0; j < colsB; j++){
            new_C[i][j] = C[i][j];
        }
    }
    free(C);
    free(new_A);
    free(new_B);
    return new_C;
}
