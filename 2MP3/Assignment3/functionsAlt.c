#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "functions.h"

void ReadMMtoCSR(const char *filename, CSRMatrix *matrix) {
    FILE *file = fopen(filename, "r");

    char line[200]; // stores characters of each line
    
    fgets(line, sizeof(line), file); // gets first line character
    if (strstr(line, "symmetric") == NULL) { // looks for word "symmetric" in first line of file
        printf("Invalid input, matrix must be symmetrical.\n");
        fclose(file);
        exit(0);
    }  

    do {
        fgets(line, sizeof(line), file);
    } while (line[0] == '%'); // skips through lines until header is done

    sscanf(line, "%d %d %d", &matrix->num_rows, &matrix->num_cols, &matrix->num_non_zeros); // checks first row of non

    int old_num_non_zeros = matrix->num_non_zeros;
    matrix->num_non_zeros = (matrix->num_non_zeros) * 2 - (matrix->num_rows); // math for new amount of zeros

    // Allocate memory for the CSRMatrix arrays
    matrix->csr_data = (double *)malloc(matrix->num_non_zeros * sizeof(double));
    matrix->row_ptr = (int *)malloc(matrix->num_non_zeros * sizeof(int));
    matrix->col_ind = (int *)malloc(matrix->num_non_zeros * sizeof(int));

    int row, col;
    double val;

    int i = 0;
    int j = 0;  // initialize j for the symmetrical part
    while (fscanf(file, "%d %d %lf\n", &row, &col, &val) == 3) { // fills arrays with values from mtx file 
        matrix->row_ptr[i] = --row;
        matrix->col_ind[i] = --col;
        matrix->csr_data[i] = val;
        i++;

        // Check and add symmetrical elements
        if (matrix->col_ind[i-1] != matrix->row_ptr[i-1]) { // adds to end of arrays
            matrix->row_ptr[old_num_non_zeros+j] = matrix->col_ind[i-1];
            matrix->col_ind[old_num_non_zeros+j] = matrix->row_ptr[i-1];
            matrix->csr_data[old_num_non_zeros+j] = matrix->csr_data[i-1];
            j++;
        }
    }
    fclose(file);

    int temp_row, temp_col;
    double temp_val;
    for (int i = 1; i < matrix->num_non_zeros; i++) {
        // store current value
        int temp_row = matrix->row_ptr[i];
        int temp_col = matrix->col_ind[i];
        double temp_val = matrix->csr_data[i];

        // find correct position to insert current value
        int j = i - 1;
        while (j >= 0 && matrix->row_ptr[j] > temp_row) {
            // shift elements
            matrix->row_ptr[j + 1] = matrix->row_ptr[j];
            matrix->col_ind[j + 1] = matrix->col_ind[j];
            matrix->csr_data[j + 1] = matrix->csr_data[j];
            j--;
        }

        // update value 
        matrix->row_ptr[j + 1] = temp_row;
        matrix->col_ind[j + 1] = temp_col;
        matrix->csr_data[j + 1] = temp_val;
    }
    
    int indices = 1; // fix row_ptr
    for (i = 1; i < matrix->num_non_zeros; i++) {
        if (matrix->row_ptr[i] != matrix->row_ptr[i - 1]) { // checks when val of row changes
            matrix->row_ptr[indices] = i; // update to be indice instead of row number
            indices++;
        }
    }
    matrix->row_ptr[indices] = matrix->num_non_zeros;

    matrix->row_ptr = (int *)realloc(matrix->row_ptr, (matrix->num_rows + 1) * sizeof(int)); // adjust for new size of row_ptr
}

void spmv_csr(const CSRMatrix *A, const double *x, double *y) { 
    for (int i = 0; i < A->num_rows; i++) { // go through each row
        double row_sum = 0;
        for (int j = A->row_ptr[i]; j < A->row_ptr[i+1]; j++) { // go through each col in row
            row_sum += A->csr_data[j] * x[A->col_ind[j]]; // sum the multiplication of col in row of A by row of col in x
        }
        y[i] = row_sum; 
    }
}

void GuassSeidel(CSRMatrix *A, double *b, double *x, int iterations, double tolerance) {    
    for (int k = 0; k < iterations; k++) { // go through interation amount of time
        double difference;
        for (int i = 0; i < A->num_rows; i++) { // go through each row
            double numerator = 0;
            double denominator; 
            for (int j = A->row_ptr[i]; j < A->row_ptr[i+1]; j++) { // go through each column in row
                if (i == A->col_ind[j] && A->csr_data[j] != 0) { // if is part of diagonal is denominator
                    denominator = A->csr_data[j];
                }
                else { // if not diagonal is added to numerator
                    numerator += A->csr_data[j] * x[A->col_ind[j]];
                }
            }
            double x_temp = x[i];   
            x[i] = (b[i]-numerator)/denominator; // update x
            difference  = fabs(x[i] - x_temp); // calculate difference between old and new
        }
        if (difference < tolerance) { // if dif between old and new is less then tolerance end 
            break;
        }
    }
}

void residual(const CSRMatrix *A, const double *b, double *x, double *residual) {
    spmv_csr(A, x, residual); // with solved for x perform matrix mulitiplication
    for (int i = 0; i < A->num_rows; i++) { // residual is calculation minus expected
        residual[i] -= b[i];
    }
}

double norm(const CSRMatrix *A, double *residual) {
    double sumNorm = 0.0;  
    double normVal;
    for (int i = 0; i < A->num_rows; i++) { // norm is sum of each residual squared, then sqrt
        sumNorm += (residual[i]*residual[i]); // sum residual squared
    }
    normVal = sqrt(sumNorm); // sqrt sum residuals
    return normVal;
}