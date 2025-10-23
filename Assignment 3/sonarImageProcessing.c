#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void rotateMatrix(int *matrix, int size) {
    int row, column, temp;

    for(row = 0; row < size; row++) {
        for(column = row; column < size; column++) {
            temp = *(matrix + row * size + column);
            *(matrix + row * size + column) = *(matrix + column * size + row);
            *(matrix + column * size + row) = temp;
        }
    }

    for(row = 0; row < size; row++) {
        for(column = 0; column < size/2; column++) {
            temp = *(matrix + row * size + column);
            *(matrix + row * size + column) = *(matrix + row * size + (size - column - 1));
            *(matrix + row * size + (size - column - 1)) = temp;
        }
    }
}

void printMatrix(int *matrix, int size) {
    int row, column;

    for(row = 0; row < size; row++) {
        for(column = 0; column < size; column++) {
            printf("%d\t", *(matrix + row * size + column));
        }
        printf("\n");
    }
}

void applySmoothingFilter(int *matrix, int size) {
    int *prevRow = (int *)calloc(size, sizeof(int));
    int *currRow = (int *)calloc(size, sizeof(int));

    int row, column, sum, count, rowOffset, colOffset, neighborRow, neighborCol;

    for(column = 0; column < size; column++) {
        currRow[column] = *(matrix + column);
    }

    for(row = 0; row < size; row++) {
        int *nextRow = (row < size - 1) ? matrix + (row + 1) * size : NULL;

        for(column = 0; column < size; column++) {
            sum = 0;
            count = 0;

            for(rowOffset = -1; rowOffset <= 1; rowOffset++) {
                neighborRow = row + rowOffset;
                if(neighborRow < 0 || neighborRow >= size) continue;

                for(colOffset = -1; colOffset <= 1; colOffset++) {
                    neighborCol = column + colOffset;
                    if(neighborCol < 0 || neighborCol >= size) continue;

                    if(rowOffset == -1) {
                        sum += prevRow[neighborCol];
                    }
                    else if(rowOffset == 0) {
                        sum += *(matrix + row * size + neighborCol);
                    }
                    else if(rowOffset == 1 && nextRow != NULL) {
                        sum += *(nextRow + neighborCol);
                    }
                    else {
                        sum += *(nextRow + neighborCol);
                    }
                    count++;
                }
            }

            currRow[column] = sum / count;
        }

        for(column = 0; column < size; column++) {
            *(matrix + row * size + column) = currRow[column];
        }

        if(row < size - 1) {
            for(column = 0; column < size; column++) {
                prevRow[column] = *(matrix + row * size + column);
            }
        }
    }

    free(prevRow);
    free(currRow);
}

int main() {
    int size;
    printf("Enter the size of matrix (between 2 to 10): ");
    scanf("%d", &size);

    if(size < 2 || size > 10) {
        printf("Invalid matrix size!\n");
        return 1;
    }

    int *matrix = (int *)malloc(size * size * sizeof(int));
    int row, column;

    srand(time(NULL)); // for seeding the random number generator used by the rand() function using the current time

    for(row = 0; row < size; row++) {
        for(column = 0; column < size; column++) {
            *(matrix + row * size + column) = rand() % 256;
        }
    }

    printf("Original Randomly Generated Matrix: \n");
    printMatrix(matrix, size);
    printf("\n");

    printf("Matrix after 90 degree Rotation: \n");
    rotateMatrix(matrix, size);
    printMatrix(matrix, size);
    printf("\n");

    printf("Matrix after Applying Smoothing Filter: \n");
    applySmoothingFilter(matrix, size);
    printMatrix(matrix, size);
    
    free(matrix);
    return 0;
}