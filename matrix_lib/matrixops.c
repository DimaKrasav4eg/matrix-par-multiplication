#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>


#define MIN_STRASSEN_DIM 100
#define MIN_PAR 15

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

int** create_matrix(int rows, int cols) {
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
int** generate_random_matrix(int rows, int cols, int des) {
    int** matrix = create_matrix(rows, cols);
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        unsigned int seed = omp_get_thread_num() + i;
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand_r(&seed) % des;
        }
    }
    return matrix;
}

// Function for multiplying two matrices A and B
int** multiply_matrices(int rowsA, int colsA, int** A, int rowsB, int colsB, int** B) {
    int** result = create_matrix(rowsA, colsB);
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

int** add_matrix(int dim, int** A, int** B) {

    int** C = create_matrix(dim, dim);

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}
int** subtract_matrix(int dim, int** A, int** B) {
    int** C = create_matrix(dim, dim);
    
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}
int** copy_fragment(int n, int start_row, int start_col, int** A){
    int** new_A = create_matrix(n, n);
    #pragma omp parallel for
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            new_A[i][j] = A[start_row + i][start_col + j];
        }
    }
    return new_A;
}

int** multiply_square_matrices(int dim, int** A, int** B) {

    return multiply_matrices(dim, dim, A, dim, dim, B);
}
int** merge_matrixes(int n, int** C11, int** C12, int** C21, int** C22){
    int new_dim = 2*n; 
    int** result = create_matrix(new_dim, new_dim);

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
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
}


int** strassen_square(int dim, int** A, int** B){
    if (dim < MIN_STRASSEN_DIM){
        return multiply_square_matrices(dim, A, B);
    }
    int n = dim / 2;

    int** A11;
    int** A12;
    int** A21;
    int** A22;

    int** B11;
    int** B12;
    int** B21;
    int** B22;

    #pragma omp parallel if(n > MIN_PAR)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                A11 = copy_fragment(n, 0, 0, A);
            }
            #pragma omp section
            {
                A12 = copy_fragment(n, 0, n, A);
            }
            #pragma omp section
            {
                A21 = copy_fragment(n, n, 0, A);
            }
            #pragma omp section
            {
                A22 = copy_fragment(n, n, n, A);
            }
            #pragma omp section
            {
                B11 = copy_fragment(n, 0, 0, B);
            }
            #pragma omp section
            {
                B12 = copy_fragment(n, 0, n, B);
            }
            #pragma omp section
            {
                B21 = copy_fragment(n, n, 0, B);
            }
            #pragma omp section
            {
                B22 = copy_fragment(n, n, n, B);
            }
        }
    }

    int** M1;
    int** M2;
    int** M3;
    int** M4;
    int** M5;
    int** M6;
    int** M7;

    #pragma omp parallel if(n > MIN_PAR)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                M1 = strassen_square(n, add_matrix(n, A11, A22), add_matrix(n, B11, B22));
            }
            #pragma omp section
            {
                M2 = strassen_square(n, add_matrix(n, A21, A22), B11);
            }
            #pragma omp section
            {
                M3 = strassen_square(n, A11, subtract_matrix(n, B12, B22));
            }
            #pragma omp section
            {
                M4 = strassen_square(n, A22, subtract_matrix(n, B21, B11));
            }
            #pragma omp section
            {
                M5 = strassen_square(n, add_matrix(n, A11, A12), B22);
            }
            #pragma omp section
            {
                M6 = strassen_square(n, subtract_matrix(n, A21, A11), add_matrix(n, B11, B12));
            }
            #pragma omp section
            {
                M7 = strassen_square(n, subtract_matrix(n, A12, A22), add_matrix(n, B21, B22));
            }
        }
    }

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
    int** result = merge_matrixes(n, C11, C12, C21, C22);
    free_matrix(n, C11);
    free_matrix(n, C12);
    free_matrix(n, C21);
    free_matrix(n, C22);
    return result;
}

int** fill_zeros_to_square(int rowsA, int colsA, int** A, int n){
    int** new_A = create_matrix(n, n);

    #pragma omp parallel for
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < colsA; j++){
            new_A[i][j] = A[i][j];
        }
    }

    #pragma omp parallel for
    for (int i = rowsA; i < n; i++)
    {
        for (int j = colsA; j < n; j++){
            new_A[i][j] = 0;
        }
    }

    return new_A;
}

int find_nearest_degree_of_two(int a){
    int n = (int)ceil(log(a) / log(2));
    int near = (int)pow(2, n);
    return near;
}
int** strassen(int rowsA, int colsA, int** A, int rowsB, int colsB, int** B){
    int n = find_nearest_degree_of_two( max(max(rowsA, colsA), max(rowsB, colsB)) );
    
    int** new_A;
    int** new_B;

    #pragma omp parallel if(n > MIN_PAR)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                new_A = fill_zeros_to_square(rowsA, colsA, A, n);
            }
            #pragma omp section
            {
                new_B = fill_zeros_to_square(rowsB, colsB, B, n);
            }
        }
    }
    
    int** C = strassen_square(n, new_A, new_B);
    int** new_C = create_matrix(rowsA, colsB);

    #pragma omp parallel for
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

double test_time_func(int n, int** (*func)(), int rowsA, int colsA, int rowsB, int colsB){
    double start_full_time = 0.0;
    double end_full_time = 0.0;

    for (int i = 0; i < n; ++i)
    {
        
        int** A = generate_random_matrix(rowsA, colsA, 100);
        int** B = generate_random_matrix(rowsB, colsB, 100);

        int** C;

        start_full_time += omp_get_wtime();

        C = func(rowsA, colsA, A, rowsB, colsB, B);

        end_full_time += omp_get_wtime();

        free_matrix(rowsA, A);
        free_matrix(rowsB, B);
        free_matrix(rowsA, C);
    }
    return (end_full_time - start_full_time) / n;
}

double test_time_strassen(int n, int rowsA, int colsA, int rowsB, int colsB){
    return test_time_func(n, strassen, rowsA, colsA, rowsB, colsB);
}

double test_time_naive(int n, int rowsA, int colsA, int rowsB, int colsB){
    return test_time_func(n, multiply_matrices, rowsA, colsA, rowsB, colsB);
}

