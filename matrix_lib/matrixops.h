#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

#define ROWS_A 8
#define COLS_A 8
#define ROWS_B 8
#define COLS_B 8

int** create_matrix(int rows, int cols);
void free_matrix(int rows, int** A);
int** generate_random_matrix(int rows, int cols, int des);
int** multiply_matrices(int rowsA, int colsA, int** A, int rowsB, int colsB, int** B);
int** multiply_square_matrices(int dim, int** A, int** B);
void write_matrix_in_file(char* file_name, int rows, int cols, int** matrix);
int** add_matrix(int dim, int** A, int** B);
int** subtract_matrix(int dim, int** A, int** B);
int** copy_fragment(int n, int start_row, int start_col, int** A);
int** merge_matrixes(int n, int** C11, int** C12, int** C21, int** C22);
void print_matrix(int rows, int cols, int** A);
int** fill_zeros_to_square(int rowsA, int colsA, int** A, int n);
int** strassen(int rowsA, int colsA, int** A, int rowsB, int colsB, int** B);
double test_time_strassen(int n, int rowsA, int colsA, int rowsB, int colsB);
double test_time_naive(int n, int rowsA, int colsA, int rowsB, int colsB);