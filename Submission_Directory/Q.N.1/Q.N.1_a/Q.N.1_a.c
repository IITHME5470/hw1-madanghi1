#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_to_file(int n, double **a, int format_flag) {
    char filename[50];
    const char *base = "array";
    const char *suffix; 

    if (format_flag == 0) {
        suffix = "_asc.out";
    } else {
        suffix = "_bin.out";
    }
    sprintf(filename, "%s%06d%s", base, n, suffix);//filename created
    double memorysize_MB=(sizeof(double)*n*n)/(1024*1024);//calculates size on memory
    printf("Memory Size of %s:%.15f MB\n",filename,memorysize_MB); 
    
    FILE *file;
	if (format_flag == 0) {
    		file = fopen(filename, "w");  
	} else {
    		file = fopen(filename, "wb"); 
	}

    if (format_flag == 0) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                fprintf(file, "%.15e ", a[i][j]);
            }
            fprintf(file, "\n");
        }
    } else {
        for (int i = 0; i < n; ++i) {
            fwrite(a[i], sizeof(double), n, file);
        }
    }

    fclose(file);
}

int main() {
    FILE *input_file = fopen("input.in", "r");//reads input where array sizes i.e. n is placed
    int n;
    fscanf(input_file, "%d",&n);
    fclose(input_file);

    printf("%d\n",n);
    double **a = (double **)malloc(n * sizeof(double *));//two dimensional array creation

    for (int i = 0; i < n; ++i) {
        a[i] = (double *)malloc(n * sizeof(double));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            a[i][j] = i + j;
        }
    }

    print_to_file(n, a, 0); 
    print_to_file(n, a, 1); 
    
    for (int i = 0; i < n; ++i) {
        free(a[i]);
    }
    free(a);
    return 0;
}

