#!/bin/bash

rowsA=1000
colsA=1000
rowsB=1000
colsB=1000

repeat=1

while [[ "$#" -gt 0 ]]; do
    case "$1" in
        -d|--dim)
			if [[  -z "$2" || -z "$3" ||  -z "$4" || -z "$5" ]]; then
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
        -r|--repeat)
            if [[ -z "$2" ]]; then
                echo "The number of repetitions is not entered!"
                exit 1
            fi
            repeat=$2
            shift 2
            ;;
        *) 
            echo "Invalid argument: $1"
            exit 1
            ;;
    esac
    shift
done

gcc -c matrix_lib/matrixops.c -o matrix_lib/matrixops.o -fopenmp -lm || exit 1
ar rcs matrix_lib/libmatrixops.a matrix_lib/matrixops.o || exit 1

gcc -fopenmp -o matrix_multiply_time_test.out matrix_multiply_time_test.c -I./matrix_lib -L./matrix_lib -lmatrixops -lm || exit 1
./matrix_multiply_time_test.out $rowsA $colsA $rowsB $colsB $repeat || exit 1