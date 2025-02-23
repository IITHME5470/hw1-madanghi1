#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void read_matrix(char *filename, double **m, int n) {
    FILE *mptr = fopen(filename, "r");//reads matrix  that are separated by commas
    if (!mptr) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j == n - 1) {
                fscanf(mptr, "%lf", &m[i][j]);
            } else {
                fscanf(mptr, "%lf,", &m[i][j]);
            }
        }
    }
    fclose(mptr);
}

void read_vector(char *filename, double *v, int n) {
    FILE *vptr = fopen(filename, "r");//reads vector that are separated by commas
    if (!vptr) {
        fprintf(stderr, "Error opening file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        if (i == n - 1) {
            fscanf(vptr, "%lf", &v[i]);
        } else {
            fscanf(vptr, "%lf,", &v[i]);
        }
    }
    fclose(vptr);
}

int check_eigen(double **m, double *v, int n, int fieldwidth, double *eigenvalue) {
    double *result = (double *)malloc(n * sizeof(double));
    int is_eigenvector = 1;
    double scale = 0.0;
    int first_non_zero = 1;
    const double tolerance = 1e-15; //tolerance is set to compare and ignore elements smaller than this

    for (int i = 0; i < n; i++) {
        result[i] = 0.0;
        for (int j = 0; j < n; j++) {
            result[i] += m[i][j] * v[j];
        }
    }

    for (int i = 0; i < n; i++) {
        if (fabs(v[i]) > tolerance) { //ignores the element that is smaller than tolerance
            double current_scale = result[i] / v[i];//finds the scale for comparison
            if (first_non_zero) {
                scale = current_scale; // if non_zer0n scale is found, sets the scale as current_scale
                first_non_zero = 0;
            } else if (fabs(current_scale - scale) > tolerance) {
                is_eigenvector = 0;
                break;
            }
        } else if (fabs(result[i]) > tolerance) { 
            is_eigenvector = 0;
            break;
        }
    }

    free(result);

    if (is_eigenvector && !first_non_zero) { 
        *eigenvalue = scale;
        return 1;
    }
    return 0;
}

void append_eigenvalue(char *filename, double eigenvalue) {
    FILE *vptr = fopen(filename, "a");//appends the eigen value to the existing vector input file
    if (!vptr) {
        fprintf(stderr, "Error opening file %s for appending\n", filename);
        exit(EXIT_FAILURE);
    }
    fprintf(vptr, "Eigen Value: %.15e\n", eigenvalue); 
    fclose(vptr);
}

int main() {
    const char inputFolder[] = "input_files/";//input folder where input files are located
    FILE *fptr;
    int n_num = 4;//known from question, can be edited for further use
    int i, k, n[n_num];

    fptr = fopen("input.in", "r");
    if (!fptr) {
        fprintf(stderr, "Error opening input.in\n");
        return EXIT_FAILURE;
    }
    fscanf(fptr, "%d %d %d %d", &n[0], &n[1], &n[2], &n[3]);
    fclose(fptr);

    printf("%d\n%d\n%d\n%d\n", n[0], n[1], n[2], n[3]);

    for (k = 0; k < n_num; k++) {
        double **m = (double **)malloc(n[k] * sizeof(double *));//two dimensional matrix declaration
        for (i = 0; i < n[k]; i++) {
            m[i] = (double *)malloc(n[k] * sizeof(double));
        }

        char matrixname[50];
        sprintf(matrixname, "%smat_%06d.in", inputFolder, n[k]);//gets matrixname
        read_matrix(matrixname, m, n[k]);

        double *v = (double *)malloc(n[k] * sizeof(double));
        char vectorname[50];
	int vecnumsize;
	printf("Please enter the size of vecnum for n=%d: ",n[k]);
	scanf("%d",&vecnumsize);
        int vecnum[vecnumsize];
        double eigenvalue;
	for (i = 0; i < vecnumsize; i++){
	vecnum[i]=i+1;
	}

        for (i = 0; i < vecnumsize; i++) {
            sprintf(vectorname, "%svec_%06d_%06d.in", inputFolder, n[k], vecnum[i]);//gets vectorname
            read_vector(vectorname, v, n[k]);
            if (check_eigen(m, v, n[k], 25, &eigenvalue)) {
                printf("%s : Yes : %.6f\n", vectorname, eigenvalue);
                append_eigenvalue(vectorname, eigenvalue); 
            } else {
                printf("%s : Not an eigenvector\n", vectorname);
            }
        }

        free(v);//deallocate vector memory
        for (i = 0; i < n[k]; i++) {
            free(m[i]);//deallocates matrix memory
        }
        free(m);//deallocates matrix memory
    }

    return 0;
}


