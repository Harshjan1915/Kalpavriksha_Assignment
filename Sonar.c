#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VALUE 255

void generateMatrix(int **matrix, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            *(*(matrix + i) + j) = rand() % (MAX_VALUE + 1);
        }
    }
}

void printMatrix(int **matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d", *(*(matrix + i) + j));
        }
        printf("\n");
    }
}

void rotateMatrixClockwise(int **matrix, int n) {
    // Transpose the matrix
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int temp = *(*(matrix + i) + j);
            *(*(matrix + i) + j) = *(*(matrix + j) + i);
            *(*(matrix + j) + i) = temp;
        }
    }

    // Reverse each row
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n / 2; j++) {
            int temp = *(*(matrix + i) + j);
            *(*(matrix + i) + j) = *(*(matrix + i) + (n - j - 1));
            *(*(matrix + i) + (n - j - 1)) = temp;
        }
    }
}

void applySmoothingFilter(int **matrix, int n) {
    int *tempRow = malloc(n * sizeof(int));  

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int sum = 0, count = 0;

            for (int x = i - 1; x <= i + 1; x++) {
                for (int y = j - 1; y <= j + 1; y++) {
                    if (x >= 0 && x < n && y >= 0 && y < n) {
                        sum += *(*(matrix + x) + y);
                        count++;
                    }
                }
            }

            *(tempRow + j) = sum / count;
        }

        for (int j = 0; j < n; j++) {
            *(*(matrix + i) + j) = *(tempRow + j);
        }
    }

    free(tempRow);
}

int main() {
    int n;
    printf("Enter matrix size (2-10): ");
    scanf("%d", &n);

    if (n < 2 || n > 10) {
        printf("Invalid matrix size.\n");
        return 0;
    }

    int **matrix = malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++)
        *(matrix + i) = malloc(n * sizeof(int));

    printf("\nOriginal Randomly Generated Matrix:\n");
    generateMatrix(matrix, n);
    printMatrix(matrix, n);

    printf("\nMatrix after 90° Clockwise Rotation:\n");
    rotateMatrixClockwise(matrix, n);
    printMatrix(matrix, n);

    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    applySmoothingFilter(matrix, n);
    printMatrix(matrix, n);

    for (int i = 0; i < n; i++)
        free(*(matrix + i));
    free(matrix);

    return 0;
}
