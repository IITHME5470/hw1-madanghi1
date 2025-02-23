#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//To write the size of the file on disk and memory on a file named filsizes.out
void filesize(const char *filename, const char *sizeon, double size_MB) {
    FILE *filesize = fopen("filesizes.out", "a");
    if (filesize == NULL) {
        perror("Failed to open log file");
        exit(EXIT_FAILURE);
    }
    fprintf(filesize, "%s %s: %.15f MB\n", sizeon, filename, size_MB);
    fclose(filesize);
}

// Get the filesize on disk using fseek() function
void disksize(const char *filename) {
    const char *disk = "Disk";
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file in disksize");
        return;
    }

    fseek(file, 0, SEEK_END);
    double size = ftell(file);
    fclose(file);

    double size_MB = size / (1024 * 1024); //conversion of filesize in MB
    printf("Disk Size of %s: %.15f MB\n", filename, size_MB);
    filesize(filename, disk, size_MB);
}

void print_to_file(int n, double **a, int format_flag) {
    char filename[50];
    const char *base = "array";
    const char *suffix;
    const char *memory = "Memory";

    if (format_flag == 0) {
        suffix = "_asc.out";
    } else {
        suffix = "_bin.out";
    }
    sprintf(filename, "%s%06d%s", base, n, suffix);//creation of filename

    FILE *file;
    if (format_flag == 0) {
        file = fopen(filename, "w");
    } else {
        file = fopen(filename, "wb");
    }

    if (!file) {
        perror("Failed to open file in print_to_file");
        return;
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

    disksize(filename);

    double memorysize_MB = (sizeof(double) * n * n) / (1024.0 * 1024.0);
    filesize(filename, memory, memorysize_MB);
    printf("Memory Size of %s: %.15f MB\n", filename, memorysize_MB);
}

int main() {
    FILE *input_file = fopen("input.in", "r");//reads input file where n is separated by spaces
    if (!input_file) {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    int n_num = 6; //Inputs to be received
    int n[n_num];
    if (fscanf(input_file, "%d %d %d %d %d %d", &n[0], &n[1], &n[2], &n[3], &n[4], &n[5]) != n_num) {
        perror("Failed to read all array sizes from input file");
        fclose(input_file);
        return EXIT_FAILURE;
    }
    fclose(input_file);

    printf("%d\n%d\n%d\n%d\n%d\n%d\n", n[0], n[1], n[2], n[3], n[4], n[5]);//printing the array sizes

    for (int k = 0; k < n_num; k++) {
        double **a = (double **)malloc(n[k] * sizeof(double *));
        if (!a) {
            perror("Failed to allocate memory for rows");
            return EXIT_FAILURE;
        }

        for (int i = 0; i < n[k]; ++i) {
            a[i] = (double *)malloc(n[k] * sizeof(double));
            if (!a[i]) {
                perror("Failed to allocate memory for columns");
                for (int j = 0; j < i; j++) {
                    free(a[j]);
                }
                free(a);
                return EXIT_FAILURE;
            }
        }

        for (int i = 0; i < n[k]; ++i) {
            for (int j = 0; j < n[k]; ++j) {
                a[i][j] = i + j;
            }
        }

        print_to_file(n[k], a, 0);
        print_to_file(n[k], a, 1);

        for (int i = 0; i < n[k]; ++i) {
            free(a[i]);
        }
        free(a);
    }

    return 0;
}

