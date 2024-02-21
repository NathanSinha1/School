#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "functions.h"

int main(int argc, char *argv[]) {
    // <Handle the inputs here>
    if (argc != 2) { // checks if right amount of inputs when executing 
        printf("Usage: %s invalid, must include <mtx file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1]; // second input is file

    FILE *file = fopen(filename, "r"); // opens file
    if (file == NULL) { // if content of file invalid end
        printf("Error Opening File: %s\n", filename);
        return 1;
    }

    fclose(file);

    // Initialize structure
    CSRMatrix A;
    ReadMMtoCSR(filename, &A);

    // Initializing all the vector b, x and residual vec(in Ax=b)
    double *b = (double *)malloc(A.num_rows * sizeof(double));
    double *x = (double *)malloc(A.num_rows * sizeof(double));
    double *residualVec = (double *)malloc(A.num_rows * sizeof(double));
    // Set all elements of b to 1 and x to 0 (technically could be anything)
    for (int i = 0; i < A.num_rows; ++i) {
        b[i] = 1.0;
        x[i] = 0.0;
    }

    clock_t start_time, end_time; // start timer
    double cpu_time_used; 
    start_time = clock();

    int iterations = 1e6;
    int tolerance = 1e-16;
    GuassSeidel(&A, b, x, iterations, tolerance);

    residual(&A, b, x, residualVec);
    
    double normResidual = norm(&A, residualVec);
    
    end_time = clock(); // end timer
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // print statements
    printf("The matrix name: %s\n", argv[1]);
    printf("The dimension of the matrix: %d by %d\n", A.num_rows, A.num_cols);
    printf("Number of non-zeros: %d\n", (((A.num_non_zeros) + (A.num_rows)) / 2));
    printf("CPU time: %lf seconds\n", cpu_time_used);
    printf("Residuals Norm: %.32lf\n", normResidual);

    // free all memory
    free(A.csr_data);
    free(A.col_ind);
    free(A.row_ptr);
    free(x);
    free(b);
    free(residualVec);
}