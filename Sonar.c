#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VALUE 255

void generateMatrix(int **matrix, int size) {
    srand(time(NULL));
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            *(*(matrix + row) + column) = rand() % (MAX_VALUE + 1);
        }
    }
}

void printMatrix(int **matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            printf("%4d", *(*(matrix + row) + column));
        }
        printf("\n");
    }
}
void transposeMatrix(int **matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int column = row + 1; column < size; column++) {
            int temp = *(*(matrix + row) + column);
            *(*(matrix + row) + column) = *(*(matrix + column) + row);
            *(*(matrix + column) + row) = temp;
        }
    }
}

void reverseRows(int **matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size / 2; column++) {
            int temp = *(*(matrix + row) + column);
            *(*(matrix + row) + column) = *(*(matrix + row) + (size - column - 1));
            *(*(matrix + row) + (size - column - 1)) = temp;
        }
    }
}
void applySmoothingFilter(int **matrix, int size) {
    int *tempRow = malloc(size * sizeof(int));

    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            int sum = 0, count = 0;

            for (int x = row - 1; x <= row + 1; x++) {
                for (int y = column - 1; y <= column + 1; y++) {
                    if (x >= 0 && x < size && y >= 0 && y < size) {
                        sum += *(*(matrix + x) + y);
                        count++;
                    }
                }
            }

            *(tempRow + column) = sum / count;
        }

        for (int column = 0; column < size; column++) {
            *(*(matrix + row) + column) = *(tempRow + column);
        }
    }

    free(tempRow);
}

int main() {
    int size;
    printf("Enter matrix size (2-10): ");
    scanf("%d", &size);

    if (size < 2 || size > 10) {
        printf("Invalid matrix size.\n");
        return 0;
    }

    int **matrix = malloc(size * sizeof(int*));
    for (int row = 0; row < size; row++){
        *(matrix + row) = malloc(size * sizeof(int));
    }

    printf("\nOriginal Randomly Generated Matrix:\n");
    generateMatrix(matrix, size);
    printMatrix(matrix, size);

    printf("\nMatrix after 90° Clockwise Rotation:\n");
    transposeMatrix(matrix, size);
    reverseRows(matrix, size);
    printMatrix(matrix, size);

    printf("\nMatrix after Applying 3x3 Smoothing Filter:\n");
    applySmoothingFilter(matrix, size);
    printMatrix(matrix, size);

    for (int row = 0; row < size; row++) {
        free(*(matrix + row));
    }
    free(matrix);

    return 0;
}
