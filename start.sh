#!/bin/bash

rowsA=1024
colsA=1024
rowsB=1024
colsB=1024

while [[ "$#" -gt 0 ]]; do
    case "$1" in
        -d|--dim)
			if [[ ! -n "$2" || ! -n "$3" || ! -n "$4" || ! -n "$5" ]]; then
                echo "Expected 4 arguments!"
                exit 1
            fi
            if [[ "$3" != "$4" ]]; then
            	echo "The number of rows and columns does not match!"
                exit 1
            fi
            rowsA=$2
            colsA=$3
            rowsB=$4
            colsB=$5
            shift 4
            ;;
        *) 
            echo "Invalid argument: $1"
            exit 1
            ;;
    esac
    shift
done


gcc -fopenmp -o matrix_mult_lin.out matrix_mult_lin.c || exit 1
./matrix_mult_lin.out $rowsA $colsA $rowsB $colsB || exit 1

gcc -fopenmp -o matrix_par_naive.out matrix_par_naive.c || exit 1
./matrix_par_naive.out $rowsA $colsA $rowsB $colsB || exit 1

gcc -fopenmp -o shtrassen_alg.out shtrassen_alg.c -lm || exit 1
./shtrassen_alg.out $rowsA $colsA $rowsB $colsB || exit 1

gcc -fopenmp -o shtrassen_right_version.out shtrassen_right_version.c -lm || exit 1
./shtrassen_right_version.out $rowsA $colsA $rowsB $colsB || exit 1
